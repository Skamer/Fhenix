#include "Server.h"
#include <qdebug.h>

namespace Fhenix
{

Server::Server(QObject *parent) :
    QObject(parent)
{
    mServer = new QTcpServer;
    mUdpSocket = new QUdpSocket;
    mNextClientID = 0;
    mPacketSize = 0;
    mDefaultChannel = 0;
    mName = "Fhenix Test Server";
}

void Server::start()
{
    if(mServer->listen(QHostAddress::Any, 20020))
    {
    qWarning() << "The server listen on 6112 port";

    initializeChannels();
    connect(mServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    mUdpSocket->bind(QHostAddress::Any, 20021, QUdpSocket::ShareAddress);
    connect(mUdpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagram()));


    }
    else
        qWarning() << "Error while the server lauch";
}

void Server::initializeChannels()
{
    Channel* channelGame = new Channel;
    channelGame->setID(0);
    channelGame->setName("Game");

    Channel* channelWoW = new Channel;
    channelWoW->setID(1);
    channelWoW->setName("World of warcraft");
    channelWoW->setParent(channelGame);
    channelWoW->setIconFileName("WoW.png");

    Channel* channelLoL = new Channel;
    channelLoL->setID(2);
    channelLoL->setName("League of Legends");
    channelLoL->setIconFileName("LoL.png");
    channelLoL->setParent(channelGame);

    mChannels[0] = channelGame;
    mChannels[1] = channelWoW;
    mChannels[2] = channelLoL;
}

void Server::newConnection()
{
    QTcpSocket* socket = mServer->nextPendingConnection();

    mIncomingClients << socket;
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

}

void Server::removeIncomingConnection(QTcpSocket *socket)
{

    for(int i = 0; i < mIncomingClients.size(); i++)
    {
        if(mIncomingClients[i] == socket)
        {
            disconnect(socket, 0, this, 0);
            mIncomingClients.removeAt(i);
            return;
        }
    }
}

void Server::dataReceived()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    if(!socket)
        return;

    QDataStream in(socket);
        // if we don't know the packet's size
        if(!mPacketSize)
        {
            if(socket->bytesAvailable() < (int)sizeof(quint16))
                return;

            in >> mPacketSize;
            qWarning() << "Packet Size received :" << mPacketSize;
        }

        if(socket->bytesAvailable() < mPacketSize)
            return;

        qWarning() << "Packet content reveived";

        quint8 opcode;
        in >> opcode;


        if(opcode == Opcodes::CMSG_CONNECT)
        {
           QString uid, name, serverPassword;

           in >> uid >> name >> serverPassword;

           qWarning() << QString("Client wants to connect : %1 %2 %3 %4").arg(uid).arg(name).arg(serverPassword);
           qWarning() << "TCP Sender Information (PEER) : " << socket->peerAddress() << " / "  << socket->peerPort();
           qWarning() << "TCP Sender Information (local) : " << socket->localAddress() << " / "  << socket->localPort();

            Client* client = new Client(socket, mChannels[mDefaultChannel]);
            client->setName(name);
            //client->setUdpPort(socket->peerPort());

            removeIncomingConnection(socket);
            addClient(client);
        }

        mPacketSize = 0;


}

void Server::readPendingDatagram()
{
    while(mUdpSocket->hasPendingDatagrams())
    {
        // Create the datagram
        QHostAddress sender;
        quint16 port;

        QByteArray datagram;
        datagram.resize(mUdpSocket->pendingDatagramSize());

        quint16 size = mUdpSocket->pendingDatagramSize();

        // read the socket then transfert the data into our datagram
        mUdpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &port);


        // calculate the lenght size content packet
        size = (quint16)(sizeof(datagram.size()) - sizeof(quint16));

        // remove the lenght content size into the datagram
        datagram = datagram.remove(0, size);

        //qDebug() << "Port For Find : " << port;

        QDataStream in(datagram);
        quint8 opcode;
        in >> opcode;

        if(opcode == CMSG_CREATE_UDP_CONNECTION)
        {
            quint16 clientID;
            in >> clientID;

            if(!mClients.contains(clientID))
                return;

                Client* clientSender = mClients[clientID];

            if(!clientSender)
                return;

            clientSender->incrementUdpPacketReceived();

            qDebug() << "Received UDP Connection : Local Port : " << port <<  "/" << clientID;

            if((clientSender->udpPacketReceived() >= 20) && !clientSender->udpAvailable())
            {
                clientSender->setUdpPort(port);
                //clientSender->setLocalUdpPort(port);

                clientSender->setUdpAvailable(true);
                notifyClient(clientSender, Protocol::TCP, Opcodes::SMSG_UDP_CONNECION_OK);
                //qDebug() << "Connexion OK send to " << clientSender->udpPort();

                clientSender = nullptr;

            }

            return;

        }

        Client* clientSender = findClient(sender, port);

        /*if(clientSender)
            qDebug() << "Client Found";
        else
            qDebug() << "Client not found";*/

        if(clientSender)
            clientSender->handlePacket(datagram);

    }
}

void Server::newClient()
{
    /*QTcpSocket* socket = mServer->nextPendingConnection();
    Client* client = new Client(socket);

    mClients[0] = client;

    QList<ChannelInfo> channelInfoList;

    for(int i = 0; i < mChannels.size(); i++)
    {
        Channel* channel = mChannels[i];

        if(channel)
        {
            ChannelInfo channelInfo;
            channelInfo.id = channel->id();
            channelInfo.name = channel->name();
            channelInfo.iconData = channel->iconData();

            if(channel->parent())
                channelInfo.parent = channel->parent()->id();
            else
                channelInfo.parent = -1;

            channelInfoList << channelInfo;
        }
    }

    notifyAll(Opcodes::SMSG_CHANNEL_LIST, channelInfoList);

    //notifyAll(Opcodes::CMSG_CONNECT, QString("Skamer"), 1);

    //while(socket->waitForBytesWritten(300)) {}
    //notifyAll(Opcodes::CMSG_CONNECT, QString("Skamer"), 1);*/

}


void Server::addClient(Client *client)
{
    client->setID(mNextClientID);
    mClients[mNextClientID] = client;

    mNextClientID++;

    qDebug() << "Client size currently " << mClients.size();


    // client signal connect
    connect(client, SIGNAL(ChannelListRequested()), this, SLOT(sendChannelList()));
    connect(client, SIGNAL(ClientListRequested()), this, SLOT(sendClientList()));
    connect(client, SIGNAL(messageReceived(QString,Shared::TargetType,int)), this, SLOT(sendMessage(QString,Shared::TargetType,int)));
    connect(client, SIGNAL(serverInfoRequested()), this, SLOT(sendServerInfo()));
    connect(client, SIGNAL(switchChannelRequested(int,QString)), this, SLOT(switchClientToChannel(int,QString)));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnectClient()));
    connect(client, SIGNAL(voiceDataReceived(QByteArray)), this, SLOT(sendVoiceData(QByteArray)));

    // notify the client that the server accept the connection
    notifyClient(client, Protocol::TCP, Opcodes::SMSG_CONNECT_RESULT, static_cast<qint16>(client->id()), static_cast<qint16>(client->udpPort()));

    Shared::ClientInfo clientInfo;
    clientInfo.id = client->id();
    clientInfo.name = client->name();
    clientInfo.uid = "";
    clientInfo.channel = client->channel()->id();

    notifyAllWithBlackList(client, Protocol::TCP, Opcodes::SMSG_NEW_CLIENT_CONNECTED, clientInfo);

}

void Server::removeClient(int clientID)
{

    if(!mClients.contains(clientID))
        return;

    Client* client = mClients[clientID];

    if(!client)
        return;

    Channel* channel = client->channel();

    channel->removeClient(client);

    qDebug() << "Remove client " << mClients.remove(clientID);
    qDebug() << "Currently client size " << mClients.size();

    delete client;
    client = nullptr;


}


void Server::sendMessage(QString msg)
{

}

void Server::sendMessage(QString msg, Shared::TargetType targetType, int targetID)
{
    Client* client = qobject_cast<Client*>(sender());

    if(!client)
        return;

    if(targetType == Shared::TargetType::Channel)
        sendMessage(msg, targetType, client->channel()->id(), client->id());
    else
        sendMessage(msg, targetType, targetID, client->id());

}

void Server::sendMessage(QString msg, Shared::TargetType targetType, int targetID, int senderID)
{
    if(targetType == Shared::TargetType::Channel)
        notifyChannel(targetID, Protocol::TCP, Opcodes::MSG_SEND_MESSAGE, msg, static_cast<qint8>(targetType), static_cast<qint16>(targetID), static_cast<qint16>(senderID));
    else if(targetType == Shared::TargetType::Server)
        notifyAll(Protocol::TCP, Opcodes::MSG_SEND_MESSAGE, msg, static_cast<qint8>(targetType), static_cast<qint16>(targetID), static_cast<qint16>(senderID));
}

void Server::sendChannelList()
{
   Client* client = qobject_cast<Client*>(sender());

   if(!client)
       return;

   QList<Shared::ChannelInfo> channelInfoList;

   for(int i = 0; i < mChannels.size(); i++)
   {
       Channel* channel = mChannels[i];

       if(channel)
       {
           Shared::ChannelInfo channelInfo;
           channelInfo.id = channel->id();
           channelInfo.name = channel->name();
           channelInfo.icon = channel->iconData();
           qDebug() << "Writing icon size ready to sent" << channelInfo.icon.size();

           if(channel->parent())
               channelInfo.parent = channel->parent()->id();
           else
               channelInfo.parent = -1;

           channelInfoList << channelInfo;
       }
   }

   notifyClient(client, Protocol::TCP, Opcodes::SMSG_CHANNEL_LIST, channelInfoList);
   //notifyClient(client, Protocol::UDP, Opcodes::MSG_UDP_TEST, QString("L'udp ça marche !!"));

}

void Server::sendClientList()
{
    Client* client = qobject_cast<Client*>(sender());

    if(!client)
        return;

    QList<Shared::ClientInfo> clientInfoList;

    foreach (Client* client, mClients)
    {
        Shared::ClientInfo clientInfo;
        clientInfo.id = client->id();
        clientInfo.name = client->name();
        clientInfo.channel = client->channel()->id();

        clientInfoList << clientInfo;
    }

    notifyClient(client, Protocol::TCP, Opcodes::SMSG_CLIENT_LIST, clientInfoList);
}

void Server::sendServerInfo()
{
    Client* client = qobject_cast<Client*>(sender());

    if(!client)
        return;

    Shared::ServerInfo serverInfo;

    serverInfo.name = mName;
    serverInfo.defaultChannel = mDefaultChannel;

    notifyClient(client, Protocol::TCP, Opcodes::SMSG_SERVER_INFO, serverInfo);
}

void Server::sendVoiceData(const QByteArray &data)
{
    Client* client = qobject_cast<Client*>(sender());
    quint8 clientID = static_cast<quint8>(client->id());

    if(!client)
        return;

    if(client->udpAvailable())
    {
        notifyChannelWithBlackList(client, client->channel()->id(), Protocol::UDP, Opcodes::MSG_VOICE_DATA, data, clientID);
        notifyClient(client, Protocol::UDP, Opcodes::SMSG_CLIENT_SPEAKING, clientID);
    }
    else
    {
        notifyChannelWithBlackList(client, client->channel()->id(), Protocol::TCP, Opcodes::MSG_VOICE_DATA, data, clientID);
        notifyClient(client, Protocol::TCP, Opcodes::SMSG_CLIENT_SPEAKING, clientID);
    }
}

void Server::switchClientToChannel(int channelID, QString password)
{
    Client* client = qobject_cast<Client*>(sender());

    if(!client)
        return;

    switchClientToChannel(client, channelID, password);
}

void Server::switchClientToChannel(Client *client, int channelID, QString password)
{
    Channel* channel = mChannels[channelID];

    if(!channel)
        return;

    client->moveToChannel(channel);

    qDebug() << QString("Client (id : %1 ) has moved into channel (id : %2").arg(client->id()).arg(channelID);

    notifyAll(Protocol::TCP, Opcodes::SMSG_CLIENT_CHANNEL_SWITCHED, static_cast<qint8>(client->id()), static_cast<qint8>(channelID));
}

void Server::disconnectClient()
{
    Client* client = qobject_cast<Client*>(sender());

    if(!client)
        return;

    disconnectClient(client);
}

void Server::disconnectClient(Client *client)
{

    int clientID = client->id();

    removeClient(clientID);


    qDebug() << "Client disconnected";

    notifyAll(Protocol::TCP, Opcodes::SMSG_CLIENT_DISCONNECTED, static_cast<qint8>(clientID));

}

Client* Server::findClient(QHostAddress address, int port)
{
    if(address == QHostAddress::Null)
        return nullptr;

        foreach (Client* client, mClients)
        {
            if(!client)
                int size = mClients.size();

            if(!port)
            {
                if(client->IPAddress() == address)
                    return client;
            }
            else
            {
                if((client->IPAddress() == address) && (client->udpPort() == port))
                    return client;
            }
        }

    return nullptr;

}

}
