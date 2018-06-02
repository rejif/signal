#ifndef MWINDOW_H
#define MWINDOW_H


#include "glass.h"
#include "slib.h"
#include "config.h"

#include <iostream>

#include <sys/time.h>

#include <QDebug>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenuBar>
#include <QDesktopWidget>
#include <QCloseEvent>
#include <QGraphicsScene>
#include <QObject>
#include <QString>
#include <QLabel>
#include <QAction>
#include <QVBoxLayout>
#include <QProcess>
#include <QPushButton>
#include <QStandardPaths>
#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>
#include <QClipboard>

using namespace std;

class MWindow : public QMainWindow{
    Q_OBJECT
public:
    MWindow(QWidget *parent=0);
    virtual ~MWindow();
public slots:
    inline void mwShowHide(){ Slib::showHide(this); }
private:
    Config* config = new Config();
    unsigned long long getEpoc(){
        struct timeval tv;
        gettimeofday(&tv, NULL);
        unsigned long long millisecondsSinceEpoch =
            (unsigned long long)(tv.tv_sec) * 1000 +
            (unsigned long long)(tv.tv_usec) / 1000;
        return millisecondsSinceEpoch;
    }
    QString getUsername(){
        QString name = qgetenv("USER");
        return !name.isEmpty()?name:qgetenv("USERNAME");
    }
protected:
    QPoint p;
    void mousePressEvent(QMouseEvent *event){
        if(event->buttons() == Qt::LeftButton){
            p = event->pos();
        }
        QMainWindow::mousePressEvent(event);
    }
    void mouseMoveEvent(QMouseEvent *event){
        if(event->buttons() == Qt::LeftButton){
            this->move(mapToParent(event->pos() - p));
        }
        QMainWindow::mouseMoveEvent(event);
    }
};

#endif // MWINDOW_H
