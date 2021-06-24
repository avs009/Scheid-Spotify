#ifndef SPOTIFYWRAPPER_H
#define SPOTIFYWRAPPER_H

#include <QObject>
#include <QFile>
#include <QtNetworkAuth/QOAuth2AuthorizationCodeFlow>

class SpotifyWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SpotifyWrapper(QObject *parent = nullptr);

    bool isGranted() const;
    bool isPermanent() const;
    void setPermanent(bool value);
    bool loadTokenAndAuthenticate();
    void saveToken();
    void clearToken();

    enum AuthenticationStatus {
        NotAuthenticated,
        Authenticated,
        Authenticating
    };

    QNetworkReply * getUserInfo();
public slots:
    void grant();
private slots:
    void error(const QString &error, const QString &errorDescription, const QUrl &uri);
    void permanentChanged(bool oldValue);
signals:
    void authenticated();
    void statusChanged(AuthenticationStatus status);
private:
    QOAuth2AuthorizationCodeFlow oauth2;
    bool permanent = false;
    QFile lastFToken;
};

#endif // SPOTIFYWRAPPER_H
