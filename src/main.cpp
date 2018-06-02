#include "mwindow.h"
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QSystemTrayIcon>
#include <iostream>
using namespace std;
int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resource);

    #if defined(_WIN32) || defined(_WIN64)
        if (QSystemTrayIcon::isSystemTrayAvailable()){
            goto ok;
        }
    #else
        cout << "NoSupport" << endl;
    #endif

    QMessageBox::critical(0,
        QObject::tr("Critical Error"),
        QObject::tr("I couldn't detect any system tray on this system."));
    qApp->quit();

    ok:


//    QRect aa(QApplication::desktop()->availableGeometry());
//    MainWindow mw;
//    mw.show();
//    mw.move(aa.width()/2-mw.width()/2,aa.height()/2-mw.height()/2);

    MWindow *mw = new MWindow();

    //SystemTray
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon(":/signal.png"));
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick){
            //qDebug()<<"SystemTrayDoubleClickTrigger";
            mw->mwShowHide();
        }
    });
    //SystemTrayContextMenu
    QMenu *contextMenu = new QMenu();
    contextMenu->addAction(Slib::createSlotAction("Show/Hide",mw,SLOT(mwShowHide())));
    contextMenu->addSeparator();
    contextMenu->addAction(
        Slib::createLambdaAction("Quit",[=]{
            Slib::quitConfirm();
        })
    );
    trayIcon->setContextMenu(contextMenu);
    trayIcon->show();

    mw->show();
    Slib::mv(mw,MVPosition::CENTER);

    return a.exec();
}

