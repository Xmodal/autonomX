#pragma once

#include <QObject>
#include <QVariant>

#include "GeneratorField.h"

class GeneratorMeta : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ getType NOTIFY typeChanged)
    Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged)

    Q_PROPERTY(QVariantMap fieldTree READ getFieldTree NOTIFY fieldTreeChanged)
    Q_PROPERTY(QVariantMap helpRacks READ getHelpRacks NOTIFY helpRacksChanged)
    Q_PROPERTY(QVariantMap enumLabels READ getEnumLabels NOTIFY enumLabelsChanged)

    // essential labels
    // name: used as a front-end label
    // type: used to ID what generator to create
    QString name;
    QString type;
    QString description;

    // meta trees
    QVariantMap fieldTree;
    QVariantMap helpRacks;
    QVariantMap enumLabels;

    // where the actual data is read and registered
    void registerMeta();

public:
    // used by GeneratorMetaModel
    enum GeneratorMetaRoles {
        FieldTreeRole = Qt::UserRole + 1,
        HelpRacksRole,
        EnumLabelsRole,
    };

    // role map used by GeneratorMetaModel
    static inline const QHash<int, QByteArray> roleMap = {
        {FieldTreeRole, "fieldTree"},
        {HelpRacksRole, "helpRacks"},
        {EnumLabelsRole, "enumLabels"}
    };

    // field defaults, used when constructing field tree
    static inline const QVariantMap fieldDefaults = {
        {"propName", "prop"},
        {"label", "Field"},
        {"type", "text"},
        {"restartOnEdit", false},
        {"flagged", false},
        {"placeholder", "Text"},
        {"min", 0},
        {"max", 1},
        {"enumName", "none"}
    };

    // constructor/destructor
    GeneratorMeta(QString type);

    // property getters
    QString getName() const;
    QString getType() const;
    QString getDescription() const;

    QVariantMap getFieldTree() const;
    QVariantMap getHelpRacks() const;
    QVariantMap getEnumLabels() const;

signals:
    void nameChanged(QString name);
    void typeChanged(QString type);
    void descriptionChanged(QString description);

    void fieldTreeChanged(QVariantMap fieldTree);
    void helpRacksChanged(QVariantMap helpRacks);
    void enumLabelsChanged(QVariantMap enumLabels);
};
