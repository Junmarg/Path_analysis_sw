#ifndef SETTING_H
#define SETTING_H

#include <QSettings>

class Setting
{
public:
    Setting();
    QString getValue(QString key);
};

#endif // SETTING_H
