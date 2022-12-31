#ifndef FHENIX_SERVER_H
#define FHENIX_SERVER_H

#include <QMap>

#include <QUdpSocket>

#include "Client.h"
#include "Channel.h"

#include "../Shared/NetworkStructs.h"
#include "../Shared/NetworkUtils.h"




#include <QTcpServer>

namespace Fhenix
{



class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void start();
    void initializeChannels();

    void addClient(Client* client);
    void removeClient(int clientID);
    void sendMessage(QString msg, Shared::TargetType targetType, int targetID, int senderID);
    void sendMessage(QString msg);
    void switchClientToChannel(Client* client, int channelID, QString password);
    void disconnectClient(Client* client);
    Client* findClient(QHostAddress adress, int port = 0);

signals:

public slots:
    void newClient();
    void newConnection();
    void dataReceived();
    void sendChannelList();
    void sendClientList();
    void sendMessage(QString msg, Shared::TargetType targetType, int targetID);
    void sendServerInfo();
    void switchClientToChannel(int channelID, QString password);
    void disconnectClient();
    void readPendingDatagram();
    void sendVoiceData(const QByteArray& data);

private:
    void removeIncomingConnection(QTcpSocket* socket);


    QTcpServer* mServer;
    QUdpSocket* mUdpSocket;

    QMap<int, Client*> mClients;
    QMap<int, Channel*> mChannels;
    QList<QTcpSocket*> mIncomingClients;

    quint16 mPacketSize;
    int mDefaultChannel;


    int mNextClientID;

    QString mName;

public:


    template <typename ...Params>
    void notifyAll(Protocol protocol, Opcodes opcode, Params &&...params)
    {
      QByteArray packet = Shared::makePacket(opcode, params...);

      foreach (Client* client, mClients)
      {
        if(client)
        {
            if(protocol == Protocol::TCP)
            {
                client->sendPacket(packet);
            }
            else if(protocol == Protocol::UDP)
            {
                mUdpSocket->writeDatagram(packet, client->IPAddress(), client->udpPort());
            }
        }

      }
    }

    template <typename ...Params>
    void notifyAllWithBlackList(Client* clientIgnored, Protocol protocol, Opcodes opcode, Params &&...params)
    {
      QByteArray packet = Shared::makePacket(opcode, params...);

      foreach (Client* client, mClients)
      {
         if(client)
         {
            if(client != clientIgnored)
            {
                if(protocol == Protocol::TCP)
                {
                    client->sendPacket(packet);
                }
                else if(protocol == Protocol::UDP)
                {
                    mUdpSocket->writeDatagram(packet, client->IPAddress(), client->udpPort());
                }
            }
         }
      }
    }

    template <typename ...Params>
    void notifyClient(Client* client, Protocol protocol, Opcodes opcode, Params &&...params)
    {
        QByteArray packet = Shared::makePacket(opcode, params...);

        if(protocol == Protocol::TCP)
        {
            client->sendPacket(packet);
        }
        else if(protocol == Protocol::UDP)
        {
            mUdpSocket->writeDatagram(packet, client->IPAddress(), client->udpPort());
        }
    }

    template <typename ...Params>
    void notifyClient(const int clientID, Opcodes opcode, Params &&...params)
    {
        Client* client = mClients[clientID];

        if(!client)
            return;

        QByteArray packet = Shared::makePacket(opcode, params...);

        client->sendPacket(packet);
    }


   template<typename ...Params>
    void notifyChannel(const unsigned int channelID, Protocol protocol, Opcodes opcode, Params &&...params)
    {
        Channel* channel = mChannels[channelID];

        if(!channel)
            return;

        foreach (Client* client, channel->clients())
        {
            QByteArray packet = Shared::makePacket(opcode, params...);

            if(protocol == Protocol::TCP)
            {
                client->sendPacket(packet);
            }
            else if(protocol == Protocol::UDP)
            {
                mUdpSocket->writeDatagram(packet, client->IPAddress(), client->udpPort());
            }
        }

    }

    template<typename ...Params>
     void notifyChannelWithBlackList(Client* clientIgnored, const unsigned int channelID, Protocol protocol, Opcodes opcode, Params &&...params)
     {
         Channel* channel = mChannels[channelID];

         if(!channel)
             return;

         foreach (Client* client, channel->clients())
         {
             if(client != clientIgnored)
             {
                 QByteArray packet = Shared::makePacket(opcode, params...);

                 if(protocol == Protocol::TCP)
                 {
                     client->sendPacket(packet);
                 }
                 else if(protocol == Protocol::UDP)
                 {
                     mUdpSocket->writeDatagram(packet, client->IPAddress(), client->udpPort());
                 }
             }

         }

     }




    /*
    template <typename ...Params>
    void notifyChannelWithBlackList(const unsigned int channelID, const QList<Client*>& blackList, Opcodes opcode, Params &&...params)
    {
        Channel* channel = mChannels[channelID];

        if(!channel)
            return;

        foreach (Client* client,  channel->clients())
        {
            for(int i = 0; i < blackList.size(); i++)
            {
                if(blackList[i] != client)
                {
                    QByteArray packet = makePacket(opcode, params...);

                    client->sendPacket(packet);
                }
            }
        }
    }*/


};

}

#endif // SERVER_H
