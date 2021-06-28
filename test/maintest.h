#ifndef MAINTEST_H
#define MAINTEST_H

#include <QObject>
#include <QTest>

class MainTest : public QObject
{
    Q_OBJECT
public:
    explicit MainTest(QObject *parent = nullptr);

signals:

};

#endif // MAINTEST_H
