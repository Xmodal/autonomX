#pragma once

#include <QObject>
#include <QVariant>

#include "Generator.h"
#include "GeneratorField.h"

class GeneratorMeta : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap fieldTree READ getFieldTree NOTIFY fieldTreeChanged)
    Q_PROPERTY(QVariantMap helpRacks READ getHelpRacks NOTIFY helpRacksChanged)
    Q_PROPERTY(QVariantMap enumLabels READ getEnumLabels NOTIFY enumLabelsChanged)

    // for posterity, you never know
    QString generatorType;

    // prop members
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
    GeneratorMeta(QString generatorType);

    // property getters
    QVariantMap getFieldTree() const;
    QVariantMap getHelpRacks() const;
    QVariantMap getEnumLabels() const;

signals:
    // signals
    void fieldTreeChanged(QVariantMap fieldTree);
    void helpRacksChanged(QVariantMap helpRacks);
    void enumLabelsChanged(QVariantMap enumLabels);
};
