#include "exception.h"

Exception::Exception()
{

}

Exception::Exception(QString message) : message(message)
{

}

const QString &Exception::getMessage() const
{
    return message;
}

void Exception::setMessage(const QString &newMessage)
{
    message = newMessage;
}
