#ifndef PAGINGCONTROLLER_H
#define PAGINGCONTROLLER_H

#include <QMetaType>
#include <QJsonValue>

#include <functional>

#include "../model/paging.hpp"
#include "../model/track.h"


/*!
 * \brief Classe responsável por realizar a(s) conversão(ões) de uma página contendo faixas de música.
 */
class PagingController
{
public:
    /*!
     * \brief Método que realiza o parser de um objeto paginado a partir de um JSON cujo atributo items seja do tipo Track
     * \param pagingInJson Objeto Paginado em JSON
     * \param itemParser Função a ser chamada para realizar o parser dos objetos do atributo item para o tipo Track
     */
    static PagingForTrack convertPagingTracksFromJSON(const QJsonValue& pagingInJson, std::function<Track (const QJsonValue&)> itemParser);
};

Q_DECLARE_METATYPE(PagingController);

#endif // PAGINGCONTROLLER_H
