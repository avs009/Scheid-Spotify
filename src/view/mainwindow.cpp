#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include<QDebug>

// Temp includes
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&wrapper, &SpotifyWrapper::authenticated, this, &MainWindow::authenticated);

    ui->chkPermToken->setChecked(wrapper.isPermanent());
}

void MainWindow::authenticated() {
    QMessageBox msgBox;
    msgBox.setText("Successful Authentication!");
    msgBox.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnGrant_clicked()
{
    wrapper.grant();
}


void MainWindow::on_chkPermToken_stateChanged(int arg1)
{
    wrapper.setPermanent(ui->chkPermToken->isChecked());
}


void MainWindow::on_pushButton_clicked()
{
    //TODO Remove this test code
    auto reply = wrapper.getUserInfo();
    connect(reply, &QNetworkReply::finished, [=]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            qCritical() << "Spotify error:" << reply->errorString();
            return;
        }

        const auto json = reply->readAll();

        const auto document = QJsonDocument::fromJson(json);
        Q_ASSERT(document.isObject());
        const auto rootObject = document.object();
        const auto display_name = rootObject.value("display_name");
        if(display_name.isString())
            qDebug() << "Welcome " << display_name.toString();
    });
}

