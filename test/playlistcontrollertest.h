#ifndef PLAYLISTCONTROLLERTEST_H
#define PLAYLISTCONTROLLERTEST_H

#include <QObject>
#include <QTest>

// Definições de testes
#define TEST_PLAYLIST_FILE "playlist.json"
#define TEST_PLAYLIST_NAME "Lista de testes"
#define TEST_PLAYLIST_TRACK_NAME "Girl From Rio"

class PlayListControllerTest : public QObject
{
    Q_OBJECT
public:
    explicit PlayListControllerTest(QObject *parent = nullptr);

private slots:
    /*!
     * \brief testSavePlayList Testa se o salvamento de uma lista de música e suas músicas está funcionando.
     */
    void testSavePlayList();

    /*!
     * \brief testLoadPlayList Teste se o carregamento da lista salva anteriormente está funcionando.
     */
    void testLoadPlayList();

    /*!
     * \brief testSavePlayListWriteError Testa escrever em um arquivo somente leitura para testar a exceção.
     */
    void testSavePlayListWriteError();

    /*!
     * \brief testLoadPlayListFileError Testa ler de um arquivo inexistente ou vazio.
     */
    void testLoadPlayListFileError();

    /*!
     * \brief testLoadPlayListJSONError Testa a leitura de um JSON inválido.
     */
    void testLoadPlayListJSONError();

    /*!
     * \brief cleanup Limpa os residuos gerados pelos testes
     */
    void cleanup();
};

#endif // PLAYLISTCONTROLLERTEST_H
