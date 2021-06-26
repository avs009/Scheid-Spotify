#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "src/spotifywrapper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void authenticated();

    void on_btnGrant_clicked();

    void on_chkPermToken_stateChanged(int arg1);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    SpotifyWrapper wrapper;
};
#endif // MAINWINDOW_H
