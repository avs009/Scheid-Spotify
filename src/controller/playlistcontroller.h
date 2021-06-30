#ifndef PLAYLISTCONTROLLER_H
#define PLAYLISTCONTROLLER_H

#include <QMetaType>
#include <QJsonValue>
#include <QSemaphore>
#include <QNetworkReply>
#include <QFile>

#include "../model/playlist.h"

/*!
 * \brief Classe para realizar a leitura e escrita de uma lista de músicas em arquivo, bem como sua conversão de/para JSON.
 */
class PlayListController
{
public:

    /*!
     * \brief convertPlayListFromJson Método para converter um JSON no objeto PlayList.
     * \param playlistInJSON JSON contendo a playlist
     * \return
     */
    static PlayList convertPlayListFromJson(const QJsonValue & playlistInJSON);

    /**
     * @brief convertPlayListToJson Método para converter um objeto de PlayList em JSON.
     * @param playlist Objeto da PlayList a ser convertida
     * @return
     */
    static QJsonValue convertPlayListToJson(const PlayList & playlist);

    /*!
     * \brief loadPlaylist Método para carregar a lista de faixas de músicas a partir de um arquivo.
     * \param tracksFile
     */
    static PlayList loadPlayList(QFile & playlist);

    /*!
     * \brief savePlaylist Método para salvar a lista de faixas de músicas em um arquivo.
     * \param playlist
     */
    static void savePlayList(const PlayList & playlist);
};

Q_DECLARE_METATYPE(PlayListController);

#endif // PLAYLISTCONTROLLER_H
