#include "playlist.h"

PlayList::PlayList()
{

}

PlayList::~PlayList()
{

}

PlayList::PlayList(const PlayList & org) {

}

const QString &PlayList::getName() const
{
    return name;
}

void PlayList::setName(const QString &newName)
{
    name = newName;
}

const QVector<Track> &PlayList::getTracks() const
{
    return tracks;
}

void PlayList::addTrack(const Track &newTrack)
{
    tracks.push_back(newTrack);
}
