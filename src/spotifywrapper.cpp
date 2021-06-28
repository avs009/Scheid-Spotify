#include "spotifywrapper.h"

#include <QProcessEnvironment>
#include <QDesktopServices>
#include <QtNetworkAuth/QOAuth2AuthorizationCodeFlow>
#include <QtNetworkAuth/QOAuthHttpServerReplyHandler>
#include <QFile>

#include "src/definitions.h"

SpotifyWrapper::SpotifyWrapper(QObject *parent) : QObject(parent), TokenFile("token.txt")
{
    // Instância um servidor local para receber o token de autorização do usuário após o login no spotify
    auto replyHandler = new QOAuthHttpServerReplyHandler(1337, this);
    replyHandler->setCallbackText("<h1>Autenthication done! Closing this page in 120 seconds!</h1><script>setTimeout(window.close(), 120000);</script>");
    this->oauth2.setReplyHandler(replyHandler);

    // Ajusta para abrir um navegador ao realizar a socilicitação de login
    connect(&this->oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);

    // Ajusta os endereços de acesso a api de autenticação do spotify
    this->oauth2.setAuthorizationUrl(QUrl(SPOTIFY_URL_AUTHORIZE));
    this->oauth2.setAccessTokenUrl(QUrl(SPOTIFY_URL_API_TOKEN));

    // Ajusta os identificadores de acesso desta aplicação a api do spotify
    this->oauth2.setClientIdentifier(SPOTIFY_CLIENT_KEY);
    this->oauth2.setClientIdentifierSharedKey(SPOTIFY_CLIENT_SECRET);

    // Ajusta o escopo de acesso das informações da api do spotify
    this->oauth2.setScope(SPOTIFY_CLIENT_SCOPES);

    // Tratamento de mudança de estado da autenticação e envio de sinalização para a aplicação
    connect(&this->oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged, this, [=](QAbstractOAuth::Status status) {
        switch (status)
        {
        case QAbstractOAuth::Status::NotAuthenticated:
            emit statusChanged(NotAuthenticated);
            break;       
        case QAbstractOAuth::Status::Granted:
            // Se o acesso permanete está ativo, salva automaticamente o token obito após o login
            if(isPermanent())
               saveToken();
            emit authenticated();
        case QAbstractOAuth::Status::TemporaryCredentialsReceived:
            emit statusChanged(Authenticated);
            break;
        case QAbstractOAuth::Status::RefreshingToken:
            emit statusChanged(Authenticating);
            break;        
        }
    });

    // Modifica os parametros das requisições para atender a api de autenticação do spotify
    this->oauth2.setModifyParametersFunction([&](QAbstractOAuth::Stage stage, QVariantMap *parameters) {
          if (stage == QAbstractOAuth::Stage::RequestingAuthorization && isPermanent())
            parameters->insert("duration", "permanent");
          else if (stage == QAbstractOAuth::Stage::RefreshingAccessToken) {
              parameters->insert("client_id", this->oauth2.clientIdentifier());
              parameters->insert("client_secret", this->oauth2.clientIdentifierSharedKey());
          }
    });

    // Tramento ao ocorrer algum erro no login ou autenticação
    connect(&this->oauth2, &QOAuth2AuthorizationCodeFlow::error, this, &SpotifyWrapper::error);
}

bool SpotifyWrapper::isGranted() const
{
    auto curStatus = this->oauth2.status();
    // Se o estado da autenticação está garantido ou com credenciais temporárias, retorne como acesso permitido
    return curStatus == QAbstractOAuth::Status::Granted || curStatus == QAbstractOAuth::Status::TemporaryCredentialsReceived;
}

bool SpotifyWrapper::isPermanent() const
{
    return permanent;
}

void SpotifyWrapper::setPermanent(bool value)
{
    bool oldValue = permanent;
    permanent = value;

    // Se houve alteração no estado
    if(oldValue != permanent)
       permanentChanged(oldValue);
}

void SpotifyWrapper::grant()
{
    if(this->oauth2.status() == QAbstractOAuth::Status::NotAuthenticated)
    {
        // Se há um token salvo, tente logar por ele se já não foi carregado
        if(this->permanent && this->oauth2.refreshToken().isEmpty()) {
            // Se não carregar o token, autentique normalmente
            if(!loadTokenAndAuthenticate())
                this->oauth2.grant();
        }
        // Caso contrário efetue o login normalmente
        else
            this->oauth2.grant();
    }
}

QNetworkReply * SpotifyWrapper::getUserInfo()
{
    return this->oauth2.get(QUrl("https://api.spotify.com/v1/me"));
}

QNetworkReply * SpotifyWrapper::searchForAnItem(QString query, int offset, int limit) {
    QVariantMap paramters;
    paramters.insert("q", QUrl::toPercentEncoding(query));
    if(offset > 0)
        paramters.insert("offset", offset);
    if(limit > 1 && limit < 51)
        paramters.insert("limit", limit);
    paramters.insert("type", "track");

    return this->oauth2.get(QUrl("https://api.spotify.com/v1/search"), paramters);
}

void SpotifyWrapper::loadTokenAndAuthenticate(QString refreshToken) {
    this->oauth2.setRefreshToken(refreshToken);
    this->oauth2.refreshAccessToken();
    qDebug() << "New status: "<< (int)this->oauth2.status();
}

bool SpotifyWrapper::loadTokenAndAuthenticate() {
    if(this->TokenFile.exists() && this->TokenFile.size() > 0 && this->TokenFile.open(QIODevice::ReadOnly)) {
        this->permanent = true;
        QByteArray lastToken = this->TokenFile.readAll();
        this->TokenFile.close();
        if(lastToken.size() > 0) {
            this->loadTokenAndAuthenticate(QString::fromStdString(lastToken.toStdString()));
            return true;
        }
    }
    return false;
}

void SpotifyWrapper::saveToken() {
    if(this->oauth2.status() == QAbstractOAuth::Status::Granted) {
        qDebug() << "Writing current token...";
        QString curToken = this->oauth2.refreshToken();
        this->TokenFile.open(QIODevice::WriteOnly);
        this->TokenFile.write(QByteArray::fromStdString(curToken.toStdString()));
        this->TokenFile.close();
    }
}

void SpotifyWrapper::removeSavedToken() {
    if(this->TokenFile.exists())
        this->TokenFile.remove();
}

void SpotifyWrapper::clearToken() {
    this->oauth2.setToken("");
    this->oauth2.setRefreshToken("");
}

void SpotifyWrapper::permanentChanged(bool oldValue) {
    if(permanent) {
       saveToken();
    } else if(this->TokenFile.exists()) {
       removeSavedToken();
    }
}

void SpotifyWrapper::error(const QString &error, const QString &errorDescription, const QUrl &uri) {
    qDebug() <<  error << "\nError desc: " << errorDescription;
}
