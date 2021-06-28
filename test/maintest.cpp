#include "maintest.h"

#include <QLoggingCategory>

#include "searchcontrollertest.h"
#include "spotifywrappertest.h"

MainTest::MainTest(QObject *parent) : QObject(parent)
{
    QLoggingCategory::setFilterRules("qt.netowrk.*=true\nqt.networkauth.*=true");

    SpotifyWrapperTest wrapperTest;
    QTest::qExec(&wrapperTest);

    SearchControllerTest searcTest;
    QTest::qExec(&searcTest);
}

QTEST_MAIN(MainTest);
#include "maintest.moc"
