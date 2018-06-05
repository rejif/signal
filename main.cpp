
#include "ui_twitterdialog.h"
#include "twittertimelinemodel.h"
#include "widget.h"
#include <functional>

#include <QUrl>
#include <QApplication>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char **argv){
    QApplication app(argc, argv);
    app.setApplicationName("signal");
    app.setApplicationDisplayName("Signal");
    app.setOrganizationDomain("redobj.com");
    app.setOrganizationName("RedObj");

    Widget *w=new Widget();
    w->show();

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

    return app.exec();
}
