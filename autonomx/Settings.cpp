#include "Settings.h"

Settings::Settings(QObject* parent) : QSettings(parent) {}
Settings::~Settings() {}

QVariant Settings::value(const QString &key, const QVariant &defaultValue)
{
    return QSettings::value(key, defaultValue);
}
