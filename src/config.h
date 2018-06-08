#ifndef CONFIG_H
#define CONFIG_H

#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>
class Config{
public:
    QString path = qApp->applicationDirPath()+"/conf.ini";
    bool develop_hidden;
    QString clientID,clientSecret;
    QDateTime loaded;
    Config(){}
    ~Config();
    bool check(){
        return QFileInfo(path).exists();
    }
    void save(){
        remove();
        QSettings settings(path, QSettings::IniFormat );
        settings.beginGroup("Develop");
        settings.setValue("hidden", develop_hidden);
        settings.endGroup();
        settings.beginGroup("Shortcut");
        settings.setValue("ClientID",clientID);
        settings.setValue("ClientSecret",clientSecret);
        settings.endGroup();
        list();
    }
    void load(){
        QSettings settings(path, QSettings::IniFormat);
        settings.beginGroup("Develop");
        develop_hidden = settings.value("hidden").toBool();
        settings.endGroup();
        settings.beginGroup("Shortcut");
        clientID = settings.value("ClientID").toString();
        clientSecret = settings.value("ClientSecret").toString();
        settings.endGroup();
        loaded = QDateTime::currentDateTime();
        list();
    }
    void remove(){
        QFile(path).remove();
    }
    void list(){
        QSettings settings(path, QSettings::IniFormat );
        for(QString key : settings.allKeys()) {
            qDebug() << key << settings.value(key);
        }
    }
};

#endif // CONFIG_H
