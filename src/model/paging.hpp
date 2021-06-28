#ifndef PAGINGOBJECT_H
#define PAGINGOBJECT_H

#include <QVector>
#include <QString>
#include <QMetaType>

#include "track.h"

/*!
 * \brief Classe para suportar a paginação ao efetuar uma busca no spotify
 */
template<class T>
class Paging
{
public:
    /*!
     * \brief Paging Construtor padrão
     */
    explicit Paging()
    {

    }

    /*!
     * \brief ~Paging Destrutor padrão
     */
    virtual ~Paging()
    {
        items.clear();
    }

    /*!
     * \brief Paging Construtor de cópia da classe
     * \param org Objeto original a ser copiado
     */
    Paging(const Paging & org)
    {
        href = org.getHref();
        items = org.getItems();
        limit = org.getLimit();
        next = org.getNext();
        offset = org.getOffset();
        previous = org.getPrevious();
        total = org.getTotal();
    }

    /*!
     * \brief getHref Retorna o link de referência ao qual foi obtido este objeto
     * \return
     */
    const QString &getHref() const
    {
        return href;
    }

    /*!
     * \brief setHref Ajusta o link de referência ao qual foi obtido este objeto
     * \param newHref Novo link
     */
    void setHref(const QString &newHref)
    {
        href = newHref;
    }

    /*!
     * \brief getItems Retorna os items obtidos de acordo com o deslocamento e limite
     * \return
     */
    const QVector<T> &getItems() const
    {
        return items;
    }

    /*!
     * \brief setItems Ajusta os itens obtidos
     * \param newItems Novos itens
     */
    void setItems(const QVector<T> &newItems)
    {
        items = newItems;
    }

    /*!
     * \brief addItem Adiciona um novo item a lista de items obitidos
     * \param newItem Novo Item
     */
    void addItem(T newItem)
    {
        items.push_back(newItem);
    }

    /*!
     * \brief getLimit Retorna o limite de Resultados solicitados
     * \return
     */
    const int getLimit() const
    {
        return limit;
    }

    /*!
     * \brief setLimit Ajusta o limite de resultados solicitados
     * \param newLimit Novo limite
     */
    void setLimit(int newLimit)
    {
        limit = newLimit;
    }

    /*!
     * \brief getNext
     * \return
     */
    const QString &getNext() const
    {
        return next;
    }

    /*!
     * \brief setNext
     * \param newNext
     */
    void setNext(const QString &newNext)
    {
        next = newNext;
    }

    /*!
     * \brief getOffset Retorn o número de deslocamentos até chegar nesta página
     * \return
     */
    const int getOffset() const
    {
        return offset;
    }

    /*!
     * \brief setOffset Ajusta o número de deslocamentos até chegar nesta página
     * \param newOffset Número de deslocamentos
     */
    void setOffset(int newOffset)
    {
        offset = newOffset;
    }

    /*!
     * \brief getPrevious
     * \return
     */
    const QString getPrevious() const
    {
        return previous;
    }

    /*!
     * \brief setPrevious
     * \param newPrevious
     */
    void setPrevious(const QString &newPrevious)
    {
        previous = newPrevious;
    }

    /*!
     * \brief getTotal Retorna o total de resultados encontrados na busca
     * \return
     */
    const int getTotal() const
    {
        return total;
    }

    /*!
     * \brief setTotal Ajusta o total de resultados encontrados na busca
     * \param newTotal
     */
    void setTotal(int newTotal)
    {
        total = newTotal;
    }

    /*!
     * \brief operator QString Converte o objeto atual para string, de modo a ver as informações armazenadas.
     */
    operator QString() const
    {
        QString ret;

        QTextStream(&ret) << "Object: Paging - \n"
                          << "href='" << href << "'\n"
                          << "limit='" << limit << "'\n"
                          << "next='" << next << "'\n"
                          << "previous='" << previous << "'\n"
                          << "offset='" << offset << "'\n"
                          << "total='" << total << "'"
                          << "items:" << "\n";

        foreach(auto i, items)
            QTextStream(&ret) << "\t" << i << "\n";

        return ret;
    }
private:
    /*!
     * \brief href Link utilizado para obter esta paginação.
     */
    QString href;

    /*!
     * \brief items Items desta página de acordo com o offset e limite.
     */
    QVector<T> items;

    /*!
     * \brief limit Limite de itens por página.
     */
    int limit;

    /*!
     * \brief next
     */
    QString next;

    /*!
     * \brief offset Deslocamento utilizado até chegar nesta página.
     */
    int offset;

    /*!
     * \brief previous
     */
    QString previous;

    /*!
     * \brief total Total de resultados encontrados na busca.
     */
    int total;
};

//Q_DECLARE_METATYPE_TEMPLATE_1ARG(Paging);

typedef Paging<Track> PagingForTrack;
Q_DECLARE_METATYPE(PagingForTrack);

#endif // PAGINGOBJECT_H
