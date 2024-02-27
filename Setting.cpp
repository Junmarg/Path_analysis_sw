#include "Setting.h"

Setting::Setting()
{
    QSettings settings("HBrain", "Fire System");

    // set DB
    settings.setValue("server/host", "183.99.41.239");
    settings.setValue("server/user", "root");
    settings.setValue("server/pass", "hbrain0372!");
    settings.setValue("server/name", "fs");
    settings.setValue("server/port", "23306");

    //qDebug() << settings.allKeys();
    //qDebug() << settings.value("server/user").toString();
}

QString Setting::getValue(QString key)
{
    QSettings settings("HBrain", "Fire System");

    return settings.value(key).toString();
}
