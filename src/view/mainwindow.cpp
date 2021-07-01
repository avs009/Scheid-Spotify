#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QAudioProbe>
#include <QMediaMetaData>
#include <QFileDialog>

#include "playercontrols.h"
#include "../exceptions/exception.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , searchController(wrapper)
{
    ui->setupUi(this);

    // Instância o reprodutor de midias e sua playlist
    m_player = new QMediaPlayer(this);
    m_player->setAudioRole(QAudio::MusicRole);
    m_playlist = new QMediaPlaylist();
    m_player->setPlaylist(m_playlist);

    // Conecta os eventos do reprodutor de midia a esta classe
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &MainWindow::playlistPositionChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::statusChanged);
    connect(m_player, &QMediaPlayer::bufferStatusChanged, this, &MainWindow::bufferingProgress);
    connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &MainWindow::displayErrorMessage);

    // Conecta o evento ao ser alterado o estado do login
    connect(&wrapper, &SpotifyWrapper::statusChanged, this, &MainWindow::loginStatusChanged);

    // Instância os controles de reprodução da mídia provenientes do exemplo do qt Player
    PlayerControls *controls = new PlayerControls(this);
    controls->setState(m_player->state());
    controls->setVolume(m_player->volume());
    controls->setMuted(controls->isMuted());

    // Conecta os eventos dos controles de reprodução ao reprodutor de mídia
    connect(controls, &PlayerControls::play, m_player, &QMediaPlayer::play);
    connect(controls, &PlayerControls::pause, m_player, &QMediaPlayer::pause);
    connect(controls, &PlayerControls::stop, m_player, &QMediaPlayer::stop);
    connect(controls, &PlayerControls::next, m_playlist, &QMediaPlaylist::next);
    connect(controls, &PlayerControls::previous, m_playlist, &QMediaPlaylist::previous);
    connect(controls, &PlayerControls::changeVolume, m_player, &QMediaPlayer::setVolume);
    connect(controls, &PlayerControls::changeMuting, m_player, &QMediaPlayer::setMuted);
    connect(controls, &PlayerControls::changeRate, m_player, &QMediaPlayer::setPlaybackRate);

    // Conecta os eventos do reprodutor de mídia aos controles
    connect(m_player, &QMediaPlayer::stateChanged, controls, &PlayerControls::setState);
    connect(m_player, &QMediaPlayer::volumeChanged, controls, &PlayerControls::setVolume);
    connect(m_player, &QMediaPlayer::mutedChanged, controls, &PlayerControls::setMuted);

    // Adiciona os controles ao seu respectivo local
    ui->widgetAudio->layout()->addWidget(controls);

    // Ajuda o tamanho inicial da barra de reprodução
    ui->m_slider->setRange(0, m_player->duration() / 1000);

    // Conecta o evento se segurar a barra de reprodução e alterar se local
    connect(ui->m_slider, &QSlider::sliderMoved, this, &MainWindow::seek);

    // Conecta o evento para tratar o fim da busca
    connect(&searchController, &SearchController::searchEnd, this, &MainWindow::searchEnd);

    // Conecta o evento ao ser atigindo o tempo máximo para busca/paginação
    connect(&searchTimer, &QTimer::timeout, this, &MainWindow::searchTimeout);

    // Atribui o estado inicial do login permanete ao mesmo estado da tela
    wrapper.setPermanent(ui->chkPermanentLogin->isChecked());

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loginStatusChanged(SpotifyWrapper::AuthenticationStatus newStatus)
{
    switch (newStatus) {
        case SpotifyWrapper::Authenticated:
            ui->statusbar->showMessage("Login efetuado com sucesso!");
            ui->btnLogin->setEnabled(false);
            ui->btnSearch->setEnabled(true);
            break;
        case SpotifyWrapper::NotAuthenticated:
            ui->statusbar->showMessage("Falha no login!");
            ui->btnLogin->setEnabled(true);
            ui->btnSearch->setEnabled(false);
            break;
        case SpotifyWrapper::Authenticating:
            ui->statusbar->showMessage("Autenticando...");
            ui->btnLogin->setEnabled(false);
            ui->btnSearch->setEnabled(false);
            break;
    }
}

void MainWindow::on_btnLogin_clicked()
{
    this->wrapper.grant();
}

void MainWindow::on_btnSearch_clicked()
{
    auto filterAny = ui->txtAny->text();
    auto filterName = ui->txtName->text();
    auto filterArtist = ui->txtArtist->text();
    auto filterAlbum = ui->txtAlbum->text();
    int limit = ui->searchLimit->value();

    QVector<SearchController::SearchFilter> filters;

    if(!filterAny.isEmpty())
        filters.push_back({filterAny, SearchController::Any});

    if(!filterName.isEmpty())
        filters.push_back({filterName, SearchController::Name});

    if(!filterArtist.isEmpty())
        filters.push_back({filterArtist, SearchController::Artist});

    if(!filterAlbum.isEmpty())
        filters.push_back({filterAlbum, SearchController::Album});

    try {
        searchController.search(filters, 0, limit);

        // Se não ocorreu uma exceção a busca foi iniciada, portanto bloquei o botão e atualize os filtros        
        this->searchTimer.start(35000); // Espera 35 seg antes de finalizar a busca com timeout
        this->ui->btnSearch->setEnabled(false);
        this->searchOffset = 0;
        this->searchFilters = filters;
        this->searchLimit = limit;

    }  catch (Exception & e) {
        this->setStatusInfo(e.getMessage());
    }
}

void MainWindow::on_btnClear_clicked()
{
    ui->txtAny->clear();
    ui->txtName->clear();
    ui->txtArtist->clear();
    ui->txtAlbum->clear();
}

void MainWindow::on_filtersGroup_toggled(bool arg1)
{
    ui->lblAny->setVisible(arg1);
    ui->txtAny->setVisible(arg1);

    ui->lblName->setVisible(arg1);
    ui->txtName->setVisible(arg1);

    ui->lblArtitst->setVisible(arg1);
    ui->txtArtist->setVisible(arg1);

    ui->lblAlbum->setVisible(arg1);
    ui->txtAlbum->setVisible(arg1);

    ui->btnClear->setVisible(arg1);
}

void MainWindow::searchEnd(PagingForTrack tracks, int responseStatus, QString error, QString errorMessage)
{
    // Cancela o evento de timeout
    this->searchTimer.stop();

    // Verifia se houve algum erro na requisição
    if(responseStatus != 200 || !error.isEmpty() || !errorMessage.isEmpty())
    {
        this->ui->tableSearch->clearContents();
        this->ui->tableSearch->setRowCount(0);
        this->ui->btnPrevious->setEnabled(false);
        this->ui->btnNext->setEnabled(false);
        this->ui->lblPages->setText("");

        // Se ocorreu algum erro de autorização, force refazer o login
        if(responseStatus == 401)
            loginStatusChanged(SpotifyWrapper::NotAuthenticated);

        QString message;
        QTextStream(&message) << "Falha ao buscar: " << error;
        this->setStatusInfo(message);
        qDebug() << "Falha ao buscar: respStatus=" << responseStatus << " error=" << error << " errorMessage=" << errorMessage;
    }
    else
    {
        // Copia o resultado
        this->searchTracks = tracks;

        auto items = this->searchTracks.getItems();

        // Desabilita a tabela para atualizar
        this->ui->tableSearch->setEnabled(false);

        // Limpa a tabela
        ui->tableSearch->clearContents();
        ui->tableSearch->setRowCount(0);

        // Atualiza o novo número de músicas encontradas
        this->ui->tableSearch->setRowCount(items.size());

        int row = 0;
        foreach(auto t, items)
        {
            QTableWidgetItem *itemName = new QTableWidgetItem(t.getName());
            this->ui->tableSearch->setItem(row, 0, itemName);

            QTableWidgetItem *itemPreview = new QTableWidgetItem(t.getPreview_url().isEmpty() ? "Não" : "Sim");
            this->ui->tableSearch->setItem(row, 1, itemPreview);

            row++;
        }

        int total = this->searchTracks.getTotal();
        if(total > 0)
        {
            int limite = this->searchTracks.getLimit();
            int paginas = (total / limite) + 1;
            int pagAtual = this->searchOffset / limite + 1;

            if(pagAtual < paginas && paginas > 1)
                ui->btnNext->setEnabled(true);
            else
                ui->btnNext->setEnabled(false);

            if(pagAtual > 1)
                ui->btnPrevious->setEnabled(true);
            else
                ui->btnPrevious->setEnabled(false);

            ui->btnAdd->setEnabled(true);
            ui->tableSearch->clearSelection();

            QString paginacao;
            QTextStream(&paginacao) << pagAtual << " / " << paginas;
            ui->lblPages->setText(paginacao);
        }
        else
        {
            // Atualiza o novo número de músicas encontradas
            ui->lblPages->setText("");
            ui->btnNext->setEnabled(false);
            ui->btnPrevious->setEnabled(false);
            ui->btnAdd->setEnabled(false);
        }

        this->ui->tableSearch->setEnabled(true);

        QString message;
        QTextStream(&message) << "Busca concluida! Resultados encontrados: " << tracks.getTotal();
        this->setStatusInfo(message);
    }

    // Ao final do processamento habilite novamente o botão de busca
    this->ui->btnSearch->setEnabled(true);
}

void MainWindow::searchTimeout()
{
    searchEnd(PagingForTrack(), 408, "Tempo limite atingido!", "");
}

void MainWindow::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
    ui->m_slider->setMaximum(m_duration);
}

void MainWindow::positionChanged(qint64 progress)
{
    if (!ui->m_slider->isSliderDown())
        ui->m_slider->setValue(progress / 1000);

    updateDurationInfo(progress / 1000);
}

void MainWindow::playlistPositionChanged(int currentItem)
{
    //m_playlistView->setCurrentIndex(m_playlistModel->index(currentItem, 0));
    ui->tablePlayList->setCurrentIndex(ui->tablePlayList->model()->index(currentItem,0));
}

void MainWindow::seek(int seconds)
{
    m_player->setPosition(seconds * 1000);
}

void MainWindow::statusChanged(QMediaPlayer::MediaStatus status)
{
    //handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
        setStatusInfo(QString());
        break;        
    case QMediaPlayer::LoadedMedia:
        setStatusInfo(tr("Loaded!"));
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::BufferingMedia:
        setStatusInfo(tr("Buffering %1%").arg(m_player->bufferStatus()));
        break;
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(tr("Buffering 100%"));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Stalled %1%").arg(m_player->bufferStatus()));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void MainWindow::bufferingProgress(int progress)
{
    if (m_player->mediaStatus() == QMediaPlayer::StalledMedia)
        setStatusInfo(tr("Stalled %1\%").arg(progress));
    else
        setStatusInfo(tr("Buffering %1\%").arg(progress));
}

void MainWindow::setStatusInfo(const QString &info)
{
    ui->statusbar->showMessage(info, 5000);
}

void MainWindow::displayErrorMessage()
{
    setStatusInfo(m_player->errorString());
}

void MainWindow::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
            currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
            m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    ui->m_labelDuration->setText(tStr);
}


void MainWindow::on_chkPermanentLogin_stateChanged(int arg1)
{
    wrapper.setPermanent(arg1 != 0);
}


void MainWindow::on_btnPrevious_clicked()
{
    try {
        searchController.search(this->searchFilters, this->searchOffset-this->searchLimit, this->searchLimit);

        // Se não ocorreu uma exceção a busca foi iniciada, portanto bloquei os botões e atualize os filtros
        this->searchTimer.start(35000); // Espera 35 seg antes de finalizar a busca com timeout
        this->ui->btnSearch->setEnabled(false);
        this->ui->btnNext->setEnabled(false);
        this->ui->btnPrevious->setEnabled(false);
        this->ui->btnAdd->setEnabled(false);
        this->searchOffset -= this->searchLimit;
    }  catch (Exception & e) {
        this->setStatusInfo(e.getMessage());
    }
}


void MainWindow::on_btnNext_clicked()
{
    try {
        searchController.search(this->searchFilters, this->searchOffset+this->searchLimit, this->searchLimit);

        // Se não ocorreu uma exceção a busca foi iniciada, portanto bloquei os botões e atualize os filtros
        this->searchTimer.start(35000); // Espera 35 seg antes de finalizar a busca com timeout
        this->ui->btnSearch->setEnabled(false);
        this->ui->btnNext->setEnabled(false);
        this->ui->btnPrevious->setEnabled(false);
        this->ui->btnAdd->setEnabled(false);
        this->searchOffset += this->searchLimit;
    }  catch (Exception & e) {
        this->setStatusInfo(e.getMessage());
    }
}

void MainWindow::on_btnLoadPlayList_clicked()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Save Files"));
    QStringList supportedMimeTypes = m_player->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("application/json");
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }

    if (fileDialog.exec() == QDialog::Accepted)
    {
        auto files = fileDialog.selectedFiles();
        QFile file(files.at(0));
        try
        {
            auto playlist = PlayListController::loadPlayList(file);

            ui->txtPlayListName->setText(playlist.getName());

            auto tracks = playlist.getTracks();

            ui->tablePlayList->setDisabled(true);
            ui->tablePlayList->clearContents();
            ui->tablePlayList->setRowCount(0);

            if(tracks.size() > 0)
            {

                m_playlist->clear();

                this->playlist = playlist;

                foreach(auto track, tracks) {
                    auto preview = QUrl(track.getPreview_url());
                    if(preview.isValid())
                    {
                        m_playlist->addMedia(preview);
                        int rowCount = ui->tablePlayList->rowCount();
                        ui->tablePlayList->setRowCount(rowCount+1);
                        ui->tablePlayList->setItem(rowCount, 0, new QTableWidgetItem(track.getName()));
                    }
                }

                ui->tablePlayList->setDisabled(false);
                ui->btnDelete->setEnabled(true);
                ui->btnSave->setEnabled(true);

                QString message;

                QTextStream(&message) << "Play list carregada com " << tracks.size() << " faixas de musica.";

                setStatusInfo(message);
            }
            else {
                setStatusInfo("Playlist vazia carregada.");
                ui->btnDelete->setEnabled(false);
                ui->btnSave->setEnabled(false);
            }
        } catch(Exception & e) {
            setStatusInfo(e.getMessage());
        }
    }
}

void MainWindow::on_btnAdd_clicked()
{
    auto items = ui->tableSearch->selectedRanges();
    auto tracks = searchTracks.getItems();

    int insertedRows = 0, invalidRows = 0;

    if(items.size() > 0)
        ui->tablePlayList->setDisabled(true);

    foreach(auto i, items)
    {
        for(auto j=i.topRow(); j <= i.bottomRow(); j++)
        {
            auto track = tracks[j];
            auto preview = QUrl(track.getPreview_url());
            if(preview.isValid())
            {
                m_playlist->addMedia(preview);
                playlist.addTrack(track);
                int rowCount = ui->tablePlayList->rowCount();
                ui->tablePlayList->setRowCount(rowCount+1);
                ui->tablePlayList->setItem(rowCount, 0, new QTableWidgetItem(track.getName()));
                insertedRows++;
            }
            else
                invalidRows++;
        }
    }

    QString message;
    QTextStream(&message) << "Faixa(as) inseridas " << insertedRows << " / Faixa(as) com erro " << invalidRows;

    this->setStatusInfo(message);

    if(playlist.getTracks().size() > 0)
    {
        ui->btnSave->setEnabled(true);
        ui->btnDelete->setEnabled(true);
    }

    ui->tablePlayList->setDisabled(false);
}


void MainWindow::on_tableSearch_cellDoubleClicked(int row, int column)
{
    ui->tablePlayList->setDisabled(true);
    auto tracks = searchTracks.getItems();
    auto track = tracks[row];
    auto preview = QUrl(track.getPreview_url());
    if(preview.isValid())
    {
        m_playlist->addMedia(preview);
        playlist.addTrack(track);
        int rowCount = ui->tablePlayList->rowCount();
        ui->tablePlayList->setRowCount(rowCount+1);
        ui->tablePlayList->setItem(rowCount, 0, new QTableWidgetItem(track.getName()));
        this->setStatusInfo("Faixa inserida com sucesso!");
    }
    else
        this->setStatusInfo("Faixa inválida ou sem amostra de música.");

    if(playlist.getTracks().size() > 0)
    {
        ui->btnSave->setEnabled(true);
        ui->btnDelete->setEnabled(true);
    }

    ui->tablePlayList->setDisabled(false);
}


void MainWindow::on_btnSave_clicked()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setWindowTitle(tr("Save Files"));
    QStringList supportedMimeTypes = m_player->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("application/json");
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }

    if (fileDialog.exec() == QDialog::Accepted)
    {
        auto files = fileDialog.selectedFiles();
        playlist.setName(ui->txtPlayListName->text());
        playlist.setFile(files.at(0));
        try {
            PlayListController::savePlayList(playlist);
            setStatusInfo("Playlist salva com sucesso!");
        }  catch (Exception & e) {
            setStatusInfo(e.getMessage());
        }
    }
}


void MainWindow::on_btnDelete_clicked()
{
    auto items = ui->tablePlayList->selectedRanges();
    auto tracks = playlist.getTracks();

    if(items.size() > 0)
        ui->tablePlayList->setDisabled(true);

    for(auto i=items.size()-1; i >= 0; i--)
    {
        auto it=items[i];
        for(auto j=it.bottomRow(); j >= it.topRow(); j--)
        {
            tracks.removeAt(j);
            ui->tablePlayList->removeRow(j);
            m_playlist->removeMedia(j);
        }
    }

    ui->tablePlayList->setDisabled(false);
}


void MainWindow::on_tablePlayList_cellDoubleClicked(int row, int column)
{
    m_playlist->setCurrentIndex(row);
    m_player->play();
}

