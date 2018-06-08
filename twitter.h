
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
    bool tweet(const QString& text, const QString& inReplyToStatusId =""){
        // https://dev.twitter.com/rest/reference/post/statuses/update
        QUrl url("https://api.twitter.com/1.1/statuses/update.json");
        QUrlQuery query(url);

        QByteArray text_ = QUrl::toPercentEncoding(text, "", "~");

        QUrl url_path(text);
        qDebug() << "[Original String]:" << text;
        qDebug() << "--------------------------------------------------------------------";
        qDebug() << "(QUrl::toEncoded)          :" << url_path.toEncoded(QUrl::FullyEncoded);
        qDebug() << "(QUrl::toString)          :" << url_path.toString(QUrl::FullyEncoded);
        qDebug() << "(QUrl::url)                :" << url_path.url();
        qDebug() << "(QUrl::toString)           :" << url_path.toString();
        qDebug() << "(QUrl::toDisplayString)    :" << url_path.toDisplayString(QUrl::FullyDecoded);
        //qDebug() << "(QUrl::fromPercentEncoding):" << url_path.fromPercentEncoding(url_path.toEncoded(QUrl::FullyEncoded));

        qDebug() << "text=" << text << text_;

        QVariantMap data;
        data.insert("status", text);
        if (!inReplyToStatusId.isEmpty()) {
            data.insert("in_reply_to_status_id", inReplyToStatusId);
        }

        url.setQuery(query);

        QNetworkReply *reply = post(url, data);

    //    QNetworkReply_ *reply__ = (QNetworkReply_ *)reply;
    //    reply__->setHeader("accept", "*/*"); // accept: */*
    //    reply__->setHeader("accept-language", "ja-jp"); // accept-language: ja-jp
    //    reply__->setHeader("user-agent", "Jugemutter"); // user-agent: YoruFukurou

        connect(reply, &QNetworkReply::finished, this, [=](){
            auto reply_ = qobject_cast<QNetworkReply*>(sender());

            qDebug() << "finished tweet";

            // {\n    \"errors\": [\n        {\n            \"code\": 170,\n            \"message\": \"Missing required parameter: status.\"\n        }\n    ]\n}\n

            QJsonParseError parseError;
            const auto resultJson = reply_->readAll();
            const auto resultDoc = QJsonDocument::fromJson(resultJson, &parseError);
            if (parseError.error) {
                qDebug() << QString(resultJson);
                qCritical() << "Twitter::tweet() finished Error at:" << parseError.offset
                            << parseError.errorString();
                return;
            }
            else if (!resultDoc.isObject()) {
                qDebug() << QString(resultJson).replace(QRegExp(" +"), " ");
                return;
            }

            const auto result = resultDoc.object();

            if (result.value("id_str").isUndefined()) {
                QList<QPair<int, QString> > errors;
                const QJsonArray errorsInfo
                        = result.contains("errors") ? result.value("errors").toArray()
                                                    : QJsonArray();
                foreach (auto errorInfo, errorsInfo) {
                    const QJsonObject& errorInfo_ = errorInfo.toObject();
                    errors.push_back( qMakePair(!errorInfo_.contains("code") ? -1 : errorInfo_.value("code").toInt(),
                                               !errorInfo_.contains("message") ? QString() : errorInfo_.value("message").toString() ) );
                }
                //qDebug() << "***>>" << (result.contains("errors") &&
                //                        !result.value("errors").toArray().isEmpty() &&
                //                        result.value("errors").toArray().at(0).toObject().contains("code")
                //                         ? result.value("errors").toArray().at(0).toObject().value("code").toInt() : -1);
                qDebug() << resultDoc.toJson();
//                Q_EMIT tweetFailure(errors);
                return;
            }

            qDebug() << "****\n" << QString(resultDoc.toJson()).replace(QRegExp(" +"), " ");

            const auto tweetId = result.value("id_str").toString();

//            Q_EMIT tweeted(tweetId);
        });

        return true;
    }
signals:
    void authenticated();
private:
    Q_DISABLE_COPY(Twitter)
    QOAuthHttpServerReplyHandler *replyHandler = nullptr;
};
