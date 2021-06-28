#include "searchcontroller.h"

#include <QRegularExpression>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

#include "../exceptions/exception.h"
#include "pagingcontroller.h"
#include "trackcontroller.h"

SearchController::SearchController(SpotifyWrapper & wrapper, QObject *parent) :
    QObject(parent)
  , wrapper(wrapper)
  , searchSem(1)
{

}


void SearchController::search(QVector<SearchFilter> filters, int offset, int limit)
{
    // Verifica através do semáforo se alguma busca já esta em andamento
    if(!searchSem.tryAcquire())
        throw Exception("Busca já em andamento!");

    QString query;

    auto i=filters.begin();
    while(i != filters.end())
    {
        // Adiciona o campo de busca na query conforme o tipo do filtro
        switch (i->type)
        {
            case Name:
               QTextStream(&query) << "track: ";
            break;
            case Album:
                QTextStream(&query) << "album: ";
            break;
            case Artist:
                QTextStream(&query) << "artist: ";
            break;
            default:
            break;
        }
        //QTextStream(&query) << "\"" << i->item << "\"";
        QTextStream(&query) << i->item;

        // Se houver outros filtros adicione um espaço ao final da query
        if(++i != filters.end())
            QTextStream(&query) << " ";
    }

    this->searchReply = wrapper.searchForAnItem(query, offset, limit);
    connect(this->searchReply, &QNetworkReply::finished, this, &SearchController::searchFinished);
}

void SearchController::searchFinished()
{
    this->searchReply->deleteLater();

    int retCode = 400; // Valor padrão como Bad Request
    QString error;
    QString errorDescription;

    // Obtém o código de erro se possível
    auto statusCodeV = this->searchReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCodeV.isValid())
        retCode = statusCodeV.toInt();

    // Se houve algum erro na requisição, tente ler os descritivos de erro
    if (this->searchReply->error() != QNetworkReply::NoError)
    {
        error = this->searchReply->errorString();

        if(this->searchReply->isReadable())
        {
            const auto response = this->searchReply->readAll();
            const auto document = QJsonDocument::fromJson(response);
            // Se o parser de JSON retornou algo válido
            if(!document.isNull()) {
                if(document.isObject())
                {
                    auto obj = document.object();

                    auto errorValue = obj.value("error");
                    // Se o atributo error for objeto, a resposta é do tipo Regular Error Object
                    if(errorValue.isObject())
                    {
                        auto errorObj = errorValue.toObject();
                        errorDescription = errorObj.value("message").toString();
                    }
                    // Caso contrário a descrição do erro estará no campo error_description
                    else
                        errorDescription = obj.value("error_description").toString();
                }
                else
                {
                    //TODO Fazer algo, o tipo é desconhecido.
                }
            }
            else
                errorDescription = response;
        }

        // A partir deste momento outra requisição ja poderá ser feita
        this->searchReply = nullptr;
        this->searchSem.release();

        emit searchEnd(PagingForTrack(), retCode, error, errorDescription);
        return;
    }

    const auto json = this->searchReply->readAll();

    // A partir deste momento outra requisição já pode ser feita
    this->searchReply = nullptr;
    this->searchSem.release();

    const auto document = QJsonDocument::fromJson(json);
    Q_ASSERT(document.isObject());
    const auto searchObject = document.object();

    auto tracksValue = searchObject.value("tracks");
    if(tracksValue.isObject())
    {
        auto trackResults = PagingController::convertPagingTracksFromJSON(tracksValue, &TrackController::convertTrackFromJSON);
        emit searchEnd(trackResults,retCode, error, errorDescription);
    }
}
