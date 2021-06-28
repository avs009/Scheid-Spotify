#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QException>

/*!
 * \brief Classe de exceção genérica contendo uma mensagem
 */
class Exception : private QException
{
public:
    /*!
     * \brief Exception Construtor padrão.
     */
    Exception();

    /*!
     * \brief Exception Construtor com a mensagem da exceção
     * \param message Mensagem com a causa da exceção
     */
    Exception(QString message);

    /*!
     * \brief getMessage Obtém a mensagem com a causa da exceção.
     * \return Texto com a causa
     */
    const QString &getMessage() const;

    /*!
     * \brief setMessage Ajusta a mensagem com a causa da exceção
     * \param newMessage Texto com a nova causa
     */
    void setMessage(const QString &newMessage);

private:
    QString message;
};

#endif // EXCEPTION_H
