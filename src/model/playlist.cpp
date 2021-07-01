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

const QVector<Track> & PlayList::getTracks() const
{
    return this->tracks;
}

void PlayList::addTrack(const Track &newTrack)
{
    tracks.push_back(newTrack);
}

bool PlayList::removeTrack(const QString &trackID)
{
    int trackToRemove = -1;
    int i=0;
    foreach(auto t, tracks) {
        if(t.getId() == trackID)
        {
            trackToRemove = i;
            break;
        }
        i++;
    }

    if(trackToRemove > -1)
    {
        tracks.remove(trackToRemove);
        return true;
    }
    return false;
}

const QString &PlayList::getFile() const
{
    return file;
}

void PlayList::setFile(const QString &newFile)
{
    file = newFile;
}
