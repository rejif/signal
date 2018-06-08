#include "mwindow.h"

#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QSystemTrayIcon>

#include <iostream>

#include "ui_twitterdialog.h"
#include "twittertimelinemodel.h"
#include <functional>
#include <QUrl>
#include <QApplication>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "config.h"

using namespace std;
int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    app.setApplicationName("signal");
    app.setApplicationDisplayName("Signal");
    app.setOrganizationDomain("redobj.com");
    app.setOrganizationName("RedObj");

    Q_INIT_RESOURCE(resource);

    //forTwitter
    QCommandLineParser parser;
    QCommandLineOption token(QStringList() << "k" << "consumer-key","Application consumer key", "key");
    QCommandLineOption secret(QStringList() << "s" << "consumer-secret","Application consumer secret", "secret");
    QCommandLineOption connect(QStringList() << "c" << "connect","Connects to twitter. Requires consumer-key and consumer-secret");
    parser.addOptions({ token, secret, connect });
    parser.process(app);
    struct TwitterDialog : QDialog, Ui::TwitterDialog {
        TwitterTimelineModel model;
        TwitterDialog():QDialog(){
            setupUi(this);
            view->setModel(&model);
            view->horizontalHeader()->hideSection(0);
            view->horizontalHeader()->hideSection(1);
        }
    } twitterDialog;
    const auto authenticate = [&]() {
        const auto clientIdentifier = twitterDialog.clientIdLineEdit->text();
        const auto clientSharedSecret = twitterDialog.clientSecretLineEdit->text();
        twitterDialog.model.authenticate(qMakePair(clientIdentifier, clientSharedSecret));
    };
    const auto buttonSlot = [&]() {
        if (twitterDialog.model.status() == Twitter::Status::Granted){
            twitterDialog.model.updateTimeline();
        }else
            authenticate();
    };
    twitterDialog.clientIdLineEdit->setText(parser.value(token));
    twitterDialog.clientSecretLineEdit->setText(parser.value(secret));
    if (parser.isSet(connect)) {
        if (parser.value(token).isEmpty() || parser.value(secret).isEmpty()) {
            parser.showHelp();
        } else {
            authenticate();
            twitterDialog.view->setFocus();
        }
    }
    QObject::connect(twitterDialog.pushButton, &QPushButton::clicked, buttonSlot);
    QObject::connect(&twitterDialog.model, &TwitterTimelineModel::authenticated,std::bind(&QPushButton::setText, twitterDialog.pushButton, "&Update"));
    twitterDialog.show();
    //forTwitter


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

    MWindow *mw = new MWindow();

    //SystemTray
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon(":/signal.png"));
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger){
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

    return app.exec();
}

