#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QMetaType>
#include <QVector>

#include "track.h"

/*!
 * \brief Classe contendo as informaçõe de uma lista de músicas criada pelo usuário
 */
class PlayList
{
public:
    /*!
     * \brief PlayList Construtor padrão
     */
    explicit PlayList();

    /*!
     * \brief PlayList Construtor de cópia
     * \param org Objeto a ser copiado
     */
    PlayList(const PlayList & org);

    /*!
     * \brief ~PlayList Destrutor padrão
     */
    virtual ~PlayList();

    /*!
     * \brief getName Retorna o nome desta playlist
     * \return
     */
    const QString &getName() const;

    /*!
     * \brief setName Ajusta o nome desta playlist
     * \param newName Novo Nome
     */
    void setName(const QString &newName);

    /*!
     * \brief getTracks Retorna um vetor com as músicas desta playlist
     * \return
     */
    const QVector<Track> &getTracks() const;

    /*!
     * \brief addTrack Adiciona uma nova faixa de música a esta playlist
     * \param newTrack Nova faixa de música
     */
    void addTrack(const Track &newTrack);

private:
    /*!
     * \brief name Nome da lista de músicas
     */
    QString name;

    /*!
     * \brief tracks Lista de músicas adicionadas
     */
    QVector<Track> tracks;

    /*!
     * \brief Arquivo no qual a lista de músicas será salva
     */
    QString file;
};

Q_DECLARE_METATYPE(PlayList);

#endif // PLAYLIST_H
