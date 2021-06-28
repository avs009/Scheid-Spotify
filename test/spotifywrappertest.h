#ifndef SPOTIFYWRAPPERTEST_H
#define SPOTIFYWRAPPERTEST_H

#include <QObject>
#include <QTest>

#include "src/spotifywrapper.h"

/*!
 * \brief Classe para realizar os testes automáticos de login e autenticação.
 */
class SpotifyWrapperTest : public QObject
{
    Q_OBJECT
public:
    explicit SpotifyWrapperTest(QObject *parent = nullptr);
private:
    SpotifyWrapper * spotifyWrapper;
private slots:
    void init() {
        spotifyWrapper = new SpotifyWrapper();
        spotifyWrapper->setPermanent(false);
    }
    void cleanup() {
        delete spotifyWrapper;
        spotifyWrapper = nullptr;
    }

    /*!
     * \brief testGrant Testa o login
     */
    void testGrant();

    /*!
     * \brief testSaveAndLoadToken Teste realizar o login salvando o token de autenticação e autenticar através do token salvo.
     */
    void testSaveAndLoadToken();

    /*!
     * \brief testLoadSavedToken Testa autenticar através de um token obtido através de varável de ambiente
     */
    void testLoadSavedToken();
};

#endif // SPOTIFYWRAPPERTEST_H
