#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>

#include "../spotifywrapper.h"
#include "../controller/searchcontroller.h"
#include "../controller/playlistcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /*
     * Eventos extraídos do exemplo Player
     */

    /*!
     * \brief durationChanged Evento ao ser alterada a duração da musica que está sendo reproduzida.
     * \param duration
     */
    void durationChanged(qint64 duration);

    /*!
     * \brief positionChanged Evento ao ser alterado o progresso da música que está sendo reprodiza.
     * \param progress
     */
    void positionChanged(qint64 progress);

    /*!
     * \brief seek Evento ao ser precionado e movido o cursor da música.
     * \param seconds
     */
    void seek(int seconds);

    /*!
     * \brief playlistPositionChanged Evento ao ser trocada a música ao terminar, ou ao apertar os botões de faixa anterior / próxima.
     */
    void playlistPositionChanged(int);

    /*!
     * \brief statusChanged Evento ao ser alterado o estado de reprodução.
     * \param status
     */
    void statusChanged(QMediaPlayer::MediaStatus status);

    /*!
     * \brief bufferingProgress Evento para reportar o progresso do carregamento de alguma mídia.
     * \param progress
     */
    void bufferingProgress(int progress);

    /*!
     * \brief displayErrorMessage Evento para exibir a mensagem de erro encontrada ao carregar/reproduzir alguma mídia.
     */
    void displayErrorMessage();

    /*
     * Fim dos eventos extraídos do exemplo Player
     */

    /*!
     * \brief loginStatusChanged Evento ao ser alterado o estado do login através de OAUTH2.
     * \param newStatus Novo estado do login
     */
    void loginStatusChanged(SpotifyWrapper::AuthenticationStatus newStatus);

    /*!
     * \brief searchEnd Evento ao ser concuída alguma busca ou troca de página.
     * \param tracks Objeto contendo o resultado da busca
     * \param responseStatus Código HTTP do resultado da busca
     * \param error Mensagem de
     * \param errorMessage
     */
    void searchEnd(PagingForTrack tracks, int responseStatus, QString error, QString errorMessage);

    /*!
     * \brief searchTimeout Evento ao ser atingido o tempo máximo para a busca e/ou paginação.
     */
    void searchTimeout();

    /*!
     * \brief on_btnLogin_clicked Evento ao clicar no botão de realizar login.
     */
    void on_btnLogin_clicked();

    /*!
     * \brief on_btnSearch_clicked Evento ao clicar no botão de buscar.
     */
    void on_btnSearch_clicked();

    /*!
     * \brief on_btnClear_clicked Evento clicar no botão de limpar os filtros de busca.
     */
    void on_btnClear_clicked();

    /*!
     * \brief on_btnLoadPlayList_clicked Evento ao clicar no botão para carregar uma playlist.
     */
    void on_btnLoadPlayList_clicked();

    /*!
     * \brief on_filtersGroup_toggled Evento para exibir/esconder os filtros de busca.
     * \param arg1
     */
    void on_filtersGroup_toggled(bool arg1);

    /*!
     * \brief on_chkPermanentLogin_stateChanged Evento ao marcar/desmarcar o botão para salvar o login em disco.
     * \param arg1
     */
    void on_chkPermanentLogin_stateChanged(int arg1);


    /*!
     * \brief on_btnPrevious_clicked Evento ao clicar no botão para voltar a página a ser exibida na busca de faixas.
     */
    void on_btnPrevious_clicked();

    /*!
     * \brief on_btnNext_clicked Evento ao clicar no botão para avançar a página a ser exibida na busca de faixas.
     */
    void on_btnNext_clicked();

    /*!
     * \brief on_btnAdd_clicked Evento ao clicar no botão para adicionar as músicas selecionadas na busca a playlist atual.
     */
    void on_btnAdd_clicked();

    /*!
     * \brief on_tableSearch_cellDoubleClicked Evento ao clicar duas vezes em alguma linha dos resultados da busca de faixas.
     * \param row
     * \param column
     */
    void on_tableSearch_cellDoubleClicked(int row, int column);

    /*!
     * \brief on_btnSave_clicked Evento ao clicar no botão para salvar a playlist atual em disco.
     */
    void on_btnSave_clicked();

    /*!
     * \brief on_btnDelete_clicked Evento para excluir uma música da playlist atual.
     */
    void on_btnDelete_clicked();

    /*!
     * \brief on_tablePlayList_cellDoubleClicked Evento ao clicar duas vezes em uma linha da playlist.
     * \param row
     * \param column
     */
    void on_tablePlayList_cellDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui;

    /*!
     * \brief wrapper Instância da api com o spotify.
     */
    SpotifyWrapper wrapper;

    /*!
     * \brief searchController Instância do controlador de busca de faixas usando a api do spotify.
     */
    SearchController searchController;

    /*!
     * \brief playlist Instância da lista de músicas em exibição na tela.
     */
    PlayList playlist;

    /*!
     * \brief searchFilters Último filtro ultilizado na busca, a ser reutilizado na paginação.
     */
    QVector<SearchController::SearchFilter> searchFilters;

    /*!
     * \brief searchOffset Último deslocamento utilizado na busca, a ser reuilizado na paginação.
     */
    int searchOffset;

    /*!
     * \brief searchLimit Último limite de páginas utilizado na busca, a ser reutilizado na paginação.
     */
    int searchLimit;

    /*!
     * \brief searchTracks Último resultado da busca de faixas de músicas.
     */
    PagingForTrack searchTracks;

    /*!
     * \brief searchTimer Timer para cancelar a busca ou paginação se demorar mais que um determinado tempo.
     */
    QTimer searchTimer;

    /*
     * Variáveis e métodos extraídos do exemplo Player
     */

    /*!
     * \brief setStatusInfo Método para exibir uma mensagem na barra de estado.
     * \param info
     */
    void setStatusInfo(const QString &info);

    /*!
     * \brief updateDurationInfo Método para alterar a duração da música que está sendo reproduzida.
     * \param currentInfo
     */
    void updateDurationInfo(qint64 currentInfo);

    /*!
     * \brief m_player Instância do reprodutor de mídias.
     */
    QMediaPlayer *m_player = nullptr;

    /*!
     * \brief m_playlist Instância da lista de mídias a serem reproduzidas.
     */
    QMediaPlaylist *m_playlist = nullptr;

    /*!
     * \brief m_duration Duração da faixa atual de música.
     */
    qint64 m_duration;
};
#endif // MAINWINDOW_H
