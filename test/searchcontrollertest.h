#ifndef SEARCHCONTROLLERTEST_H
#define SEARCHCONTROLLERTEST_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include "src/spotifywrapper.h"

class SearchControllerTest : public QObject
{
    Q_OBJECT
private:
    SpotifyWrapper * spotifyWrapper;
public:
    explicit SearchControllerTest(QObject *parent = nullptr);

signals:
private slots:
    void initTestCase() {
        spotifyWrapper = new SpotifyWrapper();
        QSignalSpy spy(spotifyWrapper, &SpotifyWrapper::authenticated);
        spotifyWrapper->setPermanent(false);
        spotifyWrapper->grant();
        if(!spotifyWrapper->isGranted())
            QVERIFY2(spy.wait(), "Falha ao efetuar login!");
    }
    void init() {
        QSignalSpy spy(spotifyWrapper, &SpotifyWrapper::authenticated);
        spotifyWrapper->grant();
        if(!spotifyWrapper->isGranted())
            QVERIFY2(spy.wait(), "Falha ao manter login!");
    }
    void cleanupTestCase() {
        delete spotifyWrapper;
        spotifyWrapper = nullptr;
    }
    void testSearchAny();
    void testSearchName();
    void testSearchArtist();
    void testSearchAlbum();
    void testSearchNameAndArtist();
    void testSearchAlbumAndArtist();
    void testSearchAll();
    void testSearchPartAndArtist();
    void testSearchOnProgress();
    void testSearchEmptyFilter();
};

#endif // SEARCHCONTROLLERTEST_H
