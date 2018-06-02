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
    bool develop_hidden,explorer;
    QString token;
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
        settings.setValue("Explorer",explorer);
        settings.setValue("Token",token);
        settings.endGroup();
        list();
    }
    void load(){
        QSettings settings(path, QSettings::IniFormat);
        settings.beginGroup("Develop");
        develop_hidden = settings.value("hidden").toBool();
        settings.endGroup();
        settings.beginGroup("Shortcut");
        explorer = settings.value("Explorer").toBool();
        token = settings.value("Token").toString();
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
