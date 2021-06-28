#ifndef TRACKCONTROLLER_H
#define TRACKCONTROLLER_H

#include <QVector>
#include <QJsonValue>
#include <QSemaphore>
#include <QNetworkReply>

#include "../spotifywrapper.h"
#include "../model/paging.hpp"

/*!
 * \brief The TrackController class
 */
class TrackController : public QObject
{
    Q_OBJECT
public:
    explicit TrackController(SpotifyWrapper & wrapper, QObject *parent = nullptr);

    /*!
     * \brief loadTracks Busca a informação das faixas de músicas através dos seus identificadores.
     * \param tracks Lista com os identificadores das faixas de música
     * \return Vetor contendo as músicas encontradas
     */
    QVector<Track> loadTracks(QVector<QString> tracks);

    /*!
     * \brief convertTrackFromJSON Método para converter um objeto JSON na classe de faixa de música.
     * \param trackInJson
     * \return
     */
    static Track convertTrackFromJSON(const QJsonValue& trackInJson);

signals:
    /*!
     * \brief searchEnd Evento ao receber a resposta de uma busca efetuada previamente pelo método search.
     * \param tracks Músicas encontradas a partir da busca, ou um vetor vazio caso não encontre nenhuma
     * \param errors Texto contento os erros ocorridos ou vazio caso não ocorra erro algum
     */
    void loadTracksEnd(QVector<Track> tracks, QString errors);

private slots:

    void loadTracksFinished();

    void loadTracksError(QNetworkReply::NetworkError error);
private:
    /*!
     * \brief wrapper
     */
    SpotifyWrapper & wrapper;

    /*!
     * \brief loadTracksSem
     */
    QSemaphore loadTracksSem;

    /*!
     * \brief loadTracksReply
     */
    QNetworkReply * loadTracksReply;
};

#endif // TRACKCONTROLLER_H
