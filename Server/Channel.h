#ifndef FHENIX_CHANNEL_H
#define FHENIX_CHANNEL_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QObject>
#include <QList>
#include <QFile>

namespace Fhenix
{

class Client;
class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(QObject *parent = 0);

    QString name();
    int id();
    Channel* parent();
    QByteArray iconData();

    void setName(QString name);
    void setID(int id);
    void setParent(Channel* parent);
    void addClient(Client* client);
    void removeClient(Client* client);
    void setIconFileName(const QString& filename);
    QList<Client*>& clients() { return mClients; }

signals:

private:
    int mID;
    QString mName;
    //int mMaxClientClient;
    //int mClientCount;
    Channel* mParent;
    QList<Fhenix::Client*> mClients;
    QByteArray mIconData;


};

}

#endif // CHANNEL_H
