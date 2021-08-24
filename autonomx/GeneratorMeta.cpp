#include <QDebug>

#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QGuiApplication>

#include "GeneratorMeta.h"

GeneratorMeta::GeneratorMeta(QString type)
{
    this->type = type;
    registerMeta();
}

QString GeneratorMeta::getName() const
{
    return name;
}

QString GeneratorMeta::getType() const
{
    return type;
}

QString GeneratorMeta::getDescription() const
{
    return description;
}

QList<GeneratorField*> GeneratorMeta::getFields() const
{
    return fields;
}

QVariantMap GeneratorMeta::getFieldTree() const
{
    return fieldTree;
}

QVariantMap GeneratorMeta::getHelpRacks() const
{
    return helpRacks;
}

QVariantMap GeneratorMeta::getEnumLabels() const
{
    return enumLabels;
}

QMap<QString, QString> GeneratorMeta::getGeneratorsParameterList() const {
    return generatorsParameterList;
}

void GeneratorMeta::registerMeta() {
    // the previous way of doing it,
    // via a static reference to the generators folder
//    #ifdef Q_OS_MAC
//        QString basePath = QCoreApplication::applicationDirPath() + "/generators/" + this->type;
//    #else
//        QString basePath = QDir::currentPath() + "/generators/" + this->type;
//    #endif

    // the clutch way of doing it,
    // by importing assets in the .qrc and reading them from there
    QString basePath = ":/generators/" + this->type + "/";

    // load meta file and open
    QFile loadFile{basePath + "meta.json"};
    if (!loadFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "GeneratorMeta::registerMeta - Couldn't open field table of" << this->type << "type at" << loadFile.fileName();
        return;
    }

    // read and convert to interpretable JSON
    QByteArray fieldData = loadFile.readAll();
    QJsonDocument fieldDoc(QJsonDocument::fromJson(fieldData));


    // -- STEP ZERO:
    // the easy properties :-)
    // ----------------
    name = fieldDoc["name"].toString();
    type = fieldDoc["type"].toString();
    description = fieldDoc["description"].toString();


    // -- STEP ONE:
    // convert rack data to QML-readable field structure
    // ----------------
    QJsonArray racks = fieldDoc["racks"].toArray();

    // skim thru sub-racks
    for (int i = 0; i < racks.size(); i++) {
        QJsonArray fieldsData = racks[i].toObject()["fields"].toArray();
        QVariantList rackStr;

        // skim thru field list
        for (auto it = fieldsData.constBegin(); it != fieldsData.constEnd(); ++it) {
            QJsonObject fieldData = (*it).toObject();

            // get defaults
            QVariantMap fieldProps(GeneratorMeta::fieldDefaults);
            // get property keys
            QStringList keys = fieldData.keys();

            // iterate through defined keys and replace default values where applicable
            for (int k = 0; k < keys.size(); k++) {
                fieldProps[keys[k]] = fieldData[keys[k]].toVariant();
            }

            // apply all default and new values to a new instance of GeneratorField
            GeneratorField* newField = new GeneratorField(fieldProps);

            // append to single-leveled field list
            fields.append(newField);
            // append to multi-leveled field tree
            rackStr.append(QVariant::fromValue(*newField));
        }

        // insert SubRack vector into the field tree
        // notice that we prepend an index to the rack title
        // that's so the QML generator code can get the keys in the order they were defined in the JSON
        QString index = QStringLiteral("%1").arg(i, 2, 10, QLatin1Char('0'));
        fieldTree.insert(index + "_" + racks[i].toObject()["title"].toString(), QVariant::fromValue(rackStr));
    }


    // -- STEP TWO:
    // register help racks
    // ---------------------

    QJsonArray help = fieldDoc["help"].toArray();

    for (int i = 0; i < help.size(); i++) {
        // retrieve necessary props from JSON file
        QString title = help[i].toObject()["title"].toString();
        QString htmlPath = help[i].toObject()["html_path"].toString();

        // open file
        QFile helpHtml(basePath + htmlPath);
        if (!helpHtml.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "GeneratorMeta::registerMeta - Couldn't register help file" << htmlPath << "at" << helpHtml.fileName();
            continue;
        }

        // read from file
        QString htmlContent = helpHtml.readAll();
        // insert as entry in rack
        // again, index to order
        QString index = QStringLiteral("%1").arg(i, 2, 10, QLatin1Char('0'));
        helpRacks.insert(index + "_" + title, htmlContent);

        // close file
        helpHtml.close();
    }


    // -- STEP THREE:
    // register enum types to QML and associate
    // ---------------------

    QJsonObject enums = fieldDoc["enumLabels"].toObject();

    for (QString key : enums.keys()) {
        QVariantList labelValues = enums[key].toArray().toVariantList();
        enumLabels.insert(key, labelValues);
        qDebug() << endl << "enumLabels >> key: " << key << " labelValues: " << labelValues;
    }

    for(int i = 0; i < fields.length(); i++) {
        generatorsParameterList[(fields[i])->propName] = fields[i]->enumName;
        qDebug() << "input param: " << fields[i]->propName;
        qDebug() << "enum name: " << fields[i]->enumName;
    }

    // close main JSON file when done !
    loadFile.close();
}
