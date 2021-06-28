#ifndef SPOTIFYWRAPPER_H
#define SPOTIFYWRAPPER_H

#include <QObject>
#include <QFile>
#include <QtNetworkAuth/QOAuth2AuthorizationCodeFlow>

/*!
 * \brief Classe para realizar o login e requisições com a autenticação através de oauth2 a api do spotify.
 */
class SpotifyWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SpotifyWrapper(QObject *parent = nullptr);

    /*!
     * \brief isGranted Verifica se o acesso a api do spotify foi permitido.
     * \return true: caso o acesso tenha sido concedido, false: caso contrário
     */
    bool isGranted() const;

    /*!
     * \brief isPermanent Verifica se o acesso permamente (sem nova autenticação) a api está ativado.
     * \return true: caso esteja ativado, false: caso contrário
     */
    bool isPermanent() const;

    /*!
     * \brief setPermanent Ajusta o acesso permamente (sem nova autenticação) e caso o acesso já esteja ativo,
     * salva em disco o token para acesso posterior.
     * \param value true: ativa o acesso permanente, false: desativa o acesso permanente e exclui o token já salvo
     */
    void setPermanent(bool value);

    /*!
     * \brief loadTokenAndAuthenticate Carrega o token de acesso permanente salvo em arquivo e tenta efetuar login
     * na api do spotify.
     */
    bool loadTokenAndAuthenticate();

    /*!
     * \brief loadTokenAndAuthenticate Dado um token de acesso permamente tenta efetuar o login na api do spotify.
     * \param refresh_token Token de acesso permamente
     */
    void loadTokenAndAuthenticate(QString refresh_token);

    /*!
     * \brief saveToken Salva o token de acesso permamente em arquivo.
     */
    void saveToken();

    /*!
     * \brief removeSavedToken Remove o token de acesso permamente já salvo.
     */
    void removeSavedToken();

    /*!
     * \brief grant Realiza o login na api do spotify, será aberto um navegador para o usuário autenticar e autorizar
     * a aplicação a acessar seus dados pessoais.
     */
    void grant();

    /*!
     * \brief clearToken Limpa os tokens de acesso já obitdos, de modo a fazer logoff.
     */
    void clearToken();

    /*!
     * \brief Enumerador com os estados do login
     */
    enum AuthenticationStatus {
        NotAuthenticated,
        Authenticated,
        Authenticating
    };

    /*!
     * \brief getUserInfo Requisição para realizar um GET na api do spotify dos dados de usuário.
     * \return Resposta da requisição realizada.
     */
    QNetworkReply * getUserInfo();

    /*!
     * \brief searchForAnItem Requisição para realizar um GET na api do spotify buscando algum album, artista ou música.
     * \param query Query já formatada da requisição de acordo com a api do spotify
     * \param offset Deslocamento caso a busca seja paginada
     * \param limit Limite de valores por página
     * \return Resposta da requisição realizada.
     */
    QNetworkReply * searchForAnItem(QString query, int offset=0, int limit=20);

    /*!
     * \brief permanentChanged Método para tratamento ao ser habilitado/desabilitado o login permanente.
     * \param oldValue Valor anterior do estado
     */
    void permanentChanged(bool oldValue);

private slots:
    /*!
     * \brief error Método para tratamento de erros ocorridos ao tentar autenticar o usuário.
     * \param error Erro ocorrido ao tentar autenticar.
     * \param errorDescription Descrição do erro ao tentar autenticar
     * \param uri Url relacionada ao erro
     */
    void error(const QString &error, const QString &errorDescription, const QUrl &uri);

signals:

    /*!
     * \brief authenticated Sinal para avisar que o login e/ou autenticação foi realizada com sucesso.
     */
    void authenticated();

    /*!
     * \brief statusChanged Sinal para avisar que o status do login e/ou autenticação foi alterado.
     * \param status Novo status
     */
    void statusChanged(AuthenticationStatus status);
private:
    /*!
     * \brief oauth2 Instância da classe que realiza o login e a authenticação para acesso a api do
     * spotify.
     */
    QOAuth2AuthorizationCodeFlow oauth2;

    /*!
     * \brief permanent Indicador se o acesso permanente está ativado ou não.
     */
    bool permanent = false;

    /*!
     * \brief lastFToken Arquivo a ser usado para guardar o acesso permanente.
     */
    QFile TokenFile;
};

#endif // SPOTIFYWRAPPER_H
