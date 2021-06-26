#include "spotifywrappertest.h"

#include <QSignalSpy>
#include <QProcessEnvironment>

SpotifyWrapperTest::SpotifyWrapperTest(QObject *parent) : QObject(parent), spotifyWrapper(nullptr)
{

}

void SpotifyWrapperTest::testGrant() {
   QSignalSpy spy(spotifyWrapper, SIGNAL(authenticated()));

   spotifyWrapper->grant();

   QVERIFY2(spy.isValid() && spy.wait(), "Fail on grant!");
}

void SpotifyWrapperTest::testSaveAndLoadToken() {
    QSignalSpy spy(spotifyWrapper, SIGNAL(authenticated()));
\
    spotifyWrapper->grant();
    QVERIFY2(spy.isValid() && spy.wait(), "Fail on grant!");

    spotifyWrapper->saveToken();
    spotifyWrapper->clearToken();

    spotifyWrapper->loadTokenAndAuthenticate();

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on grant from saved token!");

    spotifyWrapper->removeSavedToken();
}

void SpotifyWrapperTest::testLoadSavedToken() {
    QSignalSpy spy(spotifyWrapper, SIGNAL(authenticated()));

    QString refreshToken = QProcessEnvironment::systemEnvironment().value("SPOTIFY_CLIENT_REFRESHTOKEN", "");

    spotifyWrapper->loadTokenAndAuthenticate(refreshToken);

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on grant from saved token!");
}

QTEST_MAIN(SpotifyWrapperTest)
#include "spotifywrappertest.moc"
