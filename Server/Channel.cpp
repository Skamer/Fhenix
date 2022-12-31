#include "Channel.h"
#include <QDebug>

namespace Fhenix
{

Channel::Channel(QObject *parent) :
    QObject(parent)
{
    mParent = nullptr;
    mClients.clear();
}

void Channel::setID(int id)
{
    mID = id;
}

void Channel::setName(QString name)
{
    mName = name;
}

void Channel::setParent(Channel *parent)
{
    mParent = parent;
}

QString Channel::name()
{
    return mName;
}

QByteArray Channel::iconData()
{
    return mIconData;
}

int Channel::id()
{
    return mID;
}

Channel* Channel::parent()
{
    return mParent;
}

void Channel::setIconFileName(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);

    if(file.isOpen())
    {
       char* buffer = new char[file.size()];
       file.read(buffer, file.size());

       mIconData = QByteArray(buffer, file.size());

       qDebug() << "Image Size " << mIconData.size();
    }
    else
        qDebug() << "File not found : " << filename;
}


void Channel::addClient(Client *client)
{
    if(!client)
        return;

    for(int i = 0; i < mClients.size(); i++)
    {
        if(mClients.at(i) == client)
            return;
    }

    mClients.push_back(client);
}

void Channel::removeClient(Client *client)
{
    if(!client)
        return;

    if(mClients.isEmpty())
        return;

    qDebug() << QString("Client Size : %1").arg(mClients.size());
    for(int i = 0; i < mClients.size(); i++)
    {
        if(mClients.at(i) == client)
        {
          mClients.removeAt(i);
          return;
        }

    }
}

}
