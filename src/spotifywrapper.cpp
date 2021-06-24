#include "spotifywrapper.h"

#include <QProcessEnvironment>
#include <QDesktopServices>
#include <QtNetworkAuth/QOAuth2AuthorizationCodeFlow>
#include <QtNetworkAuth/QOAuthHttpServerReplyHandler>
#include <QFile>

#include "src/definitions.h"

SpotifyWrapper::SpotifyWrapper(QObject *parent) : QObject(parent), lastFToken("token.txt")
{
    auto replyHandler = new QOAuthHttpServerReplyHandler(1337, this);
    replyHandler->setCallbackText("<h1>Autenthication done! Closing this page in 120 seconds!</h1><script>setTimeout(window.close(), 120000);</script>");
    this->oauth2.setReplyHandler(replyHandler);
    this->oauth2.setAuthorizationUrl(QUrl(SPOTIFY_URL_AUTHORIZE));
    this->oauth2.setAccessTokenUrl(QUrl(SPOTIFY_URL_API_TOKEN));
    this->oauth2.setScope(SPOTIFY_CLIENT_SCOPES);
    this->oauth2.setClientIdentifier(SPOTIFY_CLIENT_KEY);
    this->oauth2.setClientIdentifierSharedKey(SPOTIFY_CLIENT_SECRET);

    // Handle status changed from Oauth2
    connect(&this->oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged, this, [=](QAbstractOAuth::Status status) {
        switch (status) {
            case QAbstractOAuth::Status::NotAuthenticated:
                emit statusChanged(NotAuthenticated);
                break;
            case QAbstractOAuth::Status::TemporaryCredentialsReceived:
                emit authenticated();
                emit statusChanged(Authenticated);
                break;
            case QAbstractOAuth::Status::Granted:
                if(isPermanent())
                   saveToken(); // Automatic save current token after sucessfull authentication with granted
            case QAbstractOAuth::Status::RefreshingToken:
                emit statusChanged(Authenticating);
                break;
        }
    });

    // Modify resquest paramters to handle spotify this->oauth2 requirements
    this->oauth2.setModifyParametersFunction([&](QAbstractOAuth::Stage stage, QVariantMap *parameters) {
          if (stage == QAbstractOAuth::Stage::RequestingAuthorization && isPermanent())
            parameters->insert("duration", "permanent");
          else if (stage == QAbstractOAuth::Stage::RefreshingAccessToken) {
              parameters->insert("client_id", this->oauth2.clientIdentifier());
              parameters->insert("client_secret", this->oauth2.clientIdentifierSharedKey());
          }
    });

    // Connect authorization request to local http server callback
    connect(&this->oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);

    connect(&this->oauth2, &QOAuth2AuthorizationCodeFlow::error, this, &SpotifyWrapper::error);
}

bool SpotifyWrapper::isGranted() const
{
    return this->oauth2.status() == QAbstractOAuth::Status::Granted;
}

bool SpotifyWrapper::isPermanent() const
{
    return permanent;
}

void SpotifyWrapper::setPermanent(bool value)
{
    bool oldValue = permanent;
    permanent = value;
    if(oldValue != permanent)
        permanentChanged(oldValue);
}

void SpotifyWrapper::grant()
{
    if(this->oauth2.status() == QAbstractOAuth::Status::NotAuthenticated)
        this->oauth2.grant();
}

QNetworkReply * SpotifyWrapper::getUserInfo()
{
    return this->oauth2.get(QUrl("https://api.spotify.com/v1/me"));
}

bool SpotifyWrapper::loadTokenAndAuthenticate() {
    if(lastFToken.exists() && lastFToken.size() > 0 && lastFToken.open(QIODevice::ReadOnly)) {
        QString refreshToken;
        this->permanent = true;
        QByteArray lastToken = lastFToken.readAll();
        refreshToken = QString::fromStdString(lastToken.toStdString());
        this->oauth2.setRefreshToken(refreshToken);
        lastFToken.close();
        this->oauth2.refreshAccessToken();
        qDebug() << "New status: "<< (int)this->oauth2.status();
        return true;
    }
    return false;
}

void SpotifyWrapper::saveToken() {
    if(this->oauth2.status() == QAbstractOAuth::Status::Granted) {
        qDebug() << "Writing current token...";
        QString curToken = this->oauth2.refreshToken();
        lastFToken.open(QIODevice::WriteOnly);
        lastFToken.write(QByteArray::fromStdString(curToken.toStdString()));
        lastFToken.close();
    }
}

void SpotifyWrapper::clearToken() {
    this->oauth2.setToken("");
    this->oauth2.setRefreshToken("");
}

void SpotifyWrapper::permanentChanged(bool oldValue) {
    if(permanent) {
       saveToken();
       loadTokenAndAuthenticate();
    } else if(lastFToken.exists()) {
       lastFToken.remove();
    }
}

void SpotifyWrapper::error(const QString &error, const QString &errorDescription, const QUrl &uri) {
    qDebug() <<  error << "\nError desc: " << errorDescription;
}
