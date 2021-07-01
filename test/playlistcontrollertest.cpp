#include "playlistcontrollertest.h"

#include "src/controller/playlistcontroller.h"
#include "src/exceptions/exception.h"


PlayListControllerTest::PlayListControllerTest(QObject *parent) : QObject(parent)
{

}

void PlayListControllerTest::testSavePlayList()
{
    PlayList playlist;
    playlist.setName(TEST_PLAYLIST_NAME);
    playlist.setFile(TEST_PLAYLIST_FILE);

    Track track;
    track.setHref("https://api.spotify.com/v1/tracks/2plRom0urixt6BE8t7kOhQ");
    track.setId("2plRom0urixt6BE8t7kOhQ");
    track.setName(TEST_PLAYLIST_TRACK_NAME);
    track.setPreview_url("https://p.scdn.co/mp3-preview/3ec977426d0c9458c79cc54774a2460f9923bced?cid=774b29d4f13844c495f206cafdad9c86");
    track.setUri("spotify:track:2plRom0urixt6BE8t7kOhQ");
    playlist.addTrack(track);

    // Se o método abaixo não retornar nenhuma exceção, o salvamento foi um sucesso.
    PlayListController::savePlayList(playlist);
}

void PlayListControllerTest::testLoadPlayList()
{
    //Se o teste anterior falhar, este também não irá funcionar
    testSavePlayList();

    QFile file(TEST_PLAYLIST_FILE);

    auto playlist = PlayListController::loadPlayList(file);

    QVERIFY2(playlist.getFile() == TEST_PLAYLIST_FILE, "Falha ao verificar o nome do arquivo de origem!");
    QVERIFY2(playlist.getName() == TEST_PLAYLIST_NAME, "Falha ao verificar o nome da playlist");
    QVERIFY2(playlist.getTracks().at(0).getName() == TEST_PLAYLIST_TRACK_NAME, "Falha ao verificar a música adicionada a playlist");
}

void PlayListControllerTest::testSavePlayListWriteError()
{
    PlayList playlist;

    // Se o método abaixo retornar uma exceção, o test foi um sucesso.
    QVERIFY_EXCEPTION_THROWN(PlayListController::savePlayList(playlist),Exception);
}

void PlayListControllerTest::testLoadPlayListFileError()
{
    QFile testFile(TEST_PLAYLIST_FILE);
    if(testFile.exists())
        testFile.remove();

    // Deve retornar exceção já que o arquivo não existe
    QVERIFY_EXCEPTION_THROWN(PlayListController::loadPlayList(testFile), Exception);
}

void PlayListControllerTest::testLoadPlayListJSONError()
{
    QFile testFile(TEST_PLAYLIST_FILE);

    QVERIFY2(testFile.open(QFile::WriteOnly), "Falha ao abrir o arquivo para teste!");

    QString invalidJson = "{#}";

    QVERIFY2(testFile.write(invalidJson.toUtf8()) > 0, "Falha ao escrever JSON de teste!");
    testFile.close();

    // Deve retornar exceção já que o arquivo não tem o objeto playlist
    QVERIFY_EXCEPTION_THROWN(PlayListController::loadPlayList(testFile), Exception);
}

void PlayListControllerTest::testRemoveTrack()
{
    PlayList playlist;
    playlist.setName(TEST_PLAYLIST_NAME);
    playlist.setFile(TEST_PLAYLIST_FILE);

    Track track;
    track.setHref("https://api.spotify.com/v1/tracks/2plRom0urixt6BE8t7kOhQ");
    track.setId("2plRom0urixt6BE8t7kOhQ");
    track.setName(TEST_PLAYLIST_TRACK_NAME);
    track.setPreview_url("https://p.scdn.co/mp3-preview/3ec977426d0c9458c79cc54774a2460f9923bced?cid=774b29d4f13844c495f206cafdad9c86");
    track.setUri("spotify:track:2plRom0urixt6BE8t7kOhQ");

    const QVector<Track> & tracks = playlist.getTracks();

    playlist.addTrack(track);

    QVERIFY2(playlist.removeTrack(track.getId()) && playlist.getTracks().size()==0 , "Falha ao remover uma faixa de música da playlist");

    QVERIFY2(tracks.size() == 0, "Falha ao remover uma faixa de música da playlist da referência");
}

void PlayListControllerTest::testEditPlaylistTrack()
{
    PlayList playlist;
    playlist.setName(TEST_PLAYLIST_NAME);
    playlist.setFile(TEST_PLAYLIST_FILE);

    Track track;
    track.setHref("https://api.spotify.com/v1/tracks/2plRom0urixt6BE8t7kOhQ");
    track.setId("2plRom0urixt6BE8t7kOhQ");
    track.setName(TEST_PLAYLIST_TRACK_NAME);
    track.setPreview_url("https://p.scdn.co/mp3-preview/3ec977426d0c9458c79cc54774a2460f9923bced?cid=774b29d4f13844c495f206cafdad9c86");
    track.setUri("spotify:track:2plRom0urixt6BE8t7kOhQ");

    const QVector<Track> & tracks = playlist.getTracks();

    playlist.addTrack(track);

    QVERIFY2(tracks.size() == 1, "Falha ao verificar faixa adicionada na referencia de uma playlist");
}

void PlayListControllerTest::cleanup()
{
    QFile testFile(TEST_PLAYLIST_FILE);
    if(testFile.exists())
        testFile.remove();
}

QTEST_GUILESS_MAIN(PlayListControllerTest);
#include "playlistcontrollertest.moc"
