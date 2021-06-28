#include "track.h"

#include <QException>

Track::Track()
{
}

Track::~Track()
{

}

Track::Track(const Track & org) {
    this->href = org.href;
    this->id = org.id;
    this->name = org.name;
    this->preview_url = org.preview_url;
    this->uri = org.uri;
}

const QString &Track::getHref() const
{
    return href;
}

void Track::setHref(const QString &newHref)
{
    href = newHref;
}

const QString &Track::getId() const
{
    return id;
}

void Track::setId(const QString &newId)
{
    id = newId;
}

const QString &Track::getName() const
{
    return name;
}

void Track::setName(const QString &newName)
{
    name = newName;
}

const QString &Track::getPreview_url() const
{
    return preview_url;
}

void Track::setPreview_url(const QString &newPreview_url)
{
    preview_url = newPreview_url;
}

const QString &Track::getUri() const
{
    return uri;
}

void Track::setUri(const QString &newUri)
{
    uri = newUri;
}
