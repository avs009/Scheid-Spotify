#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H

#include <QObject>
#include <QSemaphore>
#include <QNetworkReply>

#include "../spotifywrapper.h"
#include "../model/paging.hpp"

class SearchController : public QObject
{
    Q_OBJECT
public:
    explicit SearchController(SpotifyWrapper & wrapper, QObject *parent = nullptr);

    /*!
     * \brief Emumerado com os tipos de itens que podem ser usados na busca de uma música.
     */
    enum SearchFilterType
    {
        Any,
        Name,
        Artist,
        Album
    };

    /*!
     * \brief Estrutura com o item a ser buscado e seu tipo.
     */
    struct SearchFilter
    {
        QString item;
        SearchFilterType type;
    };

    /*!
     * \brief search Busca uma ou mais músicas a partir de um conjunto de itens como nome, artista ou album.
     * \param items Filtros a serem usados para encontrar alguma música
     */
    void search(QVector<SearchFilter> filters, int offset=0, int limit=20);

signals:

    /*!
     * \brief searchEnd Evento ao receber a resposta de uma busca efetuada previamente pelo método search.
     * \param tracks Músicas encontradas a partir da busca, ou um vetor vazio caso não encontre nenhuma
     * \param errors Texto contento os erros ocorridos ou vazio caso não ocorra erro algum
     */
    void searchEnd(PagingForTrack tracks, int responseStatus, QString error, QString errorMessage);

public slots:
    /*!
     * \brief searchFinished
     */
    void searchFinished();

private:
    /*!
     * \brief wrapper
     */
    SpotifyWrapper & wrapper;

    /*!
     * \brief searchSem
     */
    QSemaphore searchSem;

    /*!
     * \brief searchRepy
     */
    QNetworkReply * searchReply;
};

#endif // SEARCHCONTROLLER_H
