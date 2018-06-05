
#include <QtGui>
#include <QtCore>
#include <QtNetwork>
#include <QtNetworkAuth>

class Twitter : public QOAuth1{
    Q_OBJECT
public:
    Twitter(QObject *parent = nullptr):Twitter(QString(), qMakePair(QString(), QString()), parent){}
    Twitter(const QPair<QString, QString> &clientCredentials, QObject *parent = nullptr):Twitter(QString(), clientCredentials, parent){}
    Twitter(const QString &screenName,const QPair<QString, QString> &clientCredentials,QObject *parent = nullptr):QOAuth1(clientCredentials.first, clientCredentials.second, nullptr, parent){
        replyHandler = new QOAuthHttpServerReplyHandler(this);
        replyHandler->setCallbackPath("callback");
        setReplyHandler(replyHandler);
        setTemporaryCredentialsUrl(QUrl("https://api.twitter.com/oauth/request_token"));
        setAuthorizationUrl(QUrl("https://api.twitter.com/oauth/authenticate"));
        setTokenCredentialsUrl(QUrl("https://api.twitter.com/oauth/access_token"));
        connect(this, &QAbstractOAuth::authorizeWithBrowser, [=](QUrl url) {
            QUrlQuery query(url);
            // Forces the user to enter their credentials to authorize the correct
            // user account
            query.addQueryItem("force_login", "true");
            if (!screenName.isEmpty())
                query.addQueryItem("screen_name", screenName);
            url.setQuery(query);
            QDesktopServices::openUrl(url);
        });
        connect(this, &QOAuth1::granted, this, &Twitter::authenticated);
        if (!clientCredentials.first.isEmpty() && !clientCredentials.second.isEmpty()){
            grant();
        }
    }
signals:
    void authenticated();
private:
    Q_DISABLE_COPY(Twitter)
    QOAuthHttpServerReplyHandler *replyHandler = nullptr;
};
