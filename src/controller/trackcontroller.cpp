#include "trackcontroller.h"

#include <QRegularExpression>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

#include "../exceptions/exception.h"

TrackController::TrackController(SpotifyWrapper & wrapper, QObject *parent) :
    QObject(parent)
  , wrapper(wrapper)
  , loadTracksSem(1)
{
}

Track TrackController::convertTrackFromJSON(const QJsonValue& trackInJson)
{
    if(!trackInJson.isObject())
        throw Exception("JSON de faixa de música inválido!");
    auto object = trackInJson.toObject();
    auto objectType = object.value("type");
    // Verifica se o objeto a ser feito o parser é do tipo track
    if(objectType.isString() && !(objectType.toString() == "track"))
        throw Exception("Tipo de objeto inválido como faixa de música!");

    Track track;

    track.setHref(object.value("href").toString());
    track.setId(object.value("id").toString());
    track.setName(object.value("name").toString());
    track.setPreview_url(object.value("preview_url").toString());
    track.setUri(object.value("uri").toString());

    return track;
}

void TrackController::loadTracksFinished()
{
    this->loadTracksReply->deleteLater();
    if (this->loadTracksReply->error() != QNetworkReply::NoError)
    {
        this->loadTracksSem.release();
        emit loadTracksEnd(QVector<Track>(),this->loadTracksReply->errorString());
        return;
    }

    const auto json = this->loadTracksReply->readAll();

    const auto document = QJsonDocument::fromJson(json);
    Q_ASSERT(document.isObject());
    const auto pagingObject = document.object();


}

void TrackController::loadTracksError(QNetworkReply::NetworkError error)
{

}

QVector<Track> TrackController::loadTracks(QVector<QString> tracks)
{
    QVector<Track> ret;

    return ret;
}
