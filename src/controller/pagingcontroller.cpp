#include "pagingcontroller.h"

#include <QJsonArray>
#include <QJsonObject>

#include "../exceptions/exception.h"

PagingController::PagingController(QObject *parent) : QObject(parent)
{

}

PagingForTrack PagingController::convertPagingTracksFromJSON(const QJsonValue& pagingInJson, std::function<Track (const QJsonValue&)> itemParser)
{
    if(!pagingInJson.isObject())
        throw new Exception("JSON de Paginação inválido.");

    // Converte o valor JSON para um objeto em JSON
    auto object = pagingInJson.toObject();

    Paging<Track> paging;

    // Faz o parser de cada um dos atributos
    paging.setHref(object.value("href").toString());
    paging.setLimit(object.value("limit").toInt());
    paging.setNext(object.value("next").toString());
    paging.setOffset(object.value("offset").toInt());
    paging.setPrevious(object.value("previous").toString());
    paging.setTotal(object.value("total").toInt());

    // Faz o parser dos items inclusos no objeto
    auto items = object.value("items");

    // Verifica se o valor encontrado é do tipo array como esperado
    if(items.isArray())
    {
        // Converte o valor em JSON para uma array de valores JSON
        auto itemsArray = items.toArray();

        // Para cada valor JSON tentar realizar o parser pela função de acordo com o tipo do item
        foreach(const QJsonValue & value, itemsArray)
            // Se foi especificada uma função para fazer o parser dos itens
            if(itemParser != nullptr)
                // Realiza o parser de cada um dos itens como objeto json
                paging.addItem(itemParser(value));
    }

    return paging;
}
