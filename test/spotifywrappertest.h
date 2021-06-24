#ifndef SPOTIFYWRAPPERTEST_H
#define SPOTIFYWRAPPERTEST_H

#include <QObject>
#include <QTest>
#include <QThread>

#include "src/spotifywrapper.h"

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
    }
    void cleanup() {
        delete spotifyWrapper;
        spotifyWrapper = nullptr;
    }
    void testGrant();
    void testLoadSavedToken();
};

#endif // SPOTIFYWRAPPERTEST_H
