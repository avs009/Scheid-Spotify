#ifndef TRACK_H
#define TRACK_H

#include <QMetaType>
#include <QString>
#include <QTextStream>

/*!
 * \brief Classe que representa uma faixa de música da api do spotify
 */
class Track
{
public:
    explicit Track();

    /*!
     * \brief Track Construtor de cópia de uma faixa de música
     * \param org Objeto a ser copiado
     */
    Track(const Track& org);

    virtual ~Track();

    /*!
     * \brief getHref Retorna o link de referência desta música
     * \return
     */
    const QString &getHref() const;

    /*!
     * \brief setHref Ajusta o link de renferência desta música
     * \param newHref Novo link
     */
    void setHref(const QString &newHref);

    /*!
     * \brief getId Retorna o identificador da faixa de música
     * \return
     */
    const QString &getId() const;

    /*!
     * \brief setId Ajusta o identificador da faixa de música
     * \param newId Novo indetificador
     */
    void setId(const QString &newId);

    /*!
     * \brief getName Retorna o nome da faixa de música
     * \return
     */
    const QString &getName() const;

    /*!
     * \brief setName Ajusta o nome da faixa de música
     * \param newName Novo nome
     */
    void setName(const QString &newName);

    /*!
     * \brief getPreview_url Retorna o endereço de pré visualização da faixa de música.
     * \return
     */
    const QString &getPreview_url() const;

    /*!
     * \brief setPreview_url Ajusta o endereço de pré visualização da faixa de música
     * \param newPreview_url Novo endereço
     */
    void setPreview_url(const QString &newPreview_url);

    /*!
     * \brief getUri Retorna o URI desta faixa de música
     * \return
     */
    const QString &getUri() const;

    /*!
     * \brief setUri Ajusta o URI desta faixa de música.
     * \param newUri
     */
    void setUri(const QString &newUri);

    /*!
     * \brief operator QString Converte o objeto atual para string, de modo a ver as informações armazenadas.
     */
    operator QString() const
    {
        QString ret;
        QTextStream(&ret) << "Object: Track"
                          << "href='" << href << "'\n"
                          << "id='" << id << "'\n"
                          << "name='" << name << "'\n"
                          << "preview='" << preview_url << "'\n"
                          << "uri='" << uri << "'";
        return ret;
    }

private:
    /*!
     * \brief href Link com os detalhes da musica
     */
    QString href;

    /*!
     * \brief id Identificador da música
     */
    QString id;

    /*!
     * \brief name Nome da faixa de música
     */
    QString name;

    /*!
     * \brief preview_url Link com o preview da música
     */
    QString preview_url;

    /*!
     * \brief uri Identificador da música no spotify
     */
    QString uri;
};

Q_DECLARE_METATYPE(Track);

#endif // TRACK_H
