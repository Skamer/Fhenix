#include "Client.h"
#include "Channel.h"

namespace Fhenix
{


Client::Client(QTcpSocket* socket, Channel* channel, QObject *parent) :
    mTcpSocket(socket),
    QObject(parent)
{
    mChannel = nullptr;
    mPacketSize = 0;
    mUdpSocket = new QUdpSocket;
    mUdpPort = -1;
    mUdpPacketReceived = 0;
    mTcpPacketReceived = 0;
    mUdpAvailable = false;

    if(socket)
    {
        connect(socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
        connect(this, SIGNAL(packetReceived(QByteArray)), SLOT(handlePacket(QByteArray)));
        connect(socket, &QTcpSocket::disconnected, [&](){ qDebug() << "Emit disconnected client"; emit(disconnected()); });

    }

    if(channel)
        moveToChannel(channel);
}

Channel* Client::channel()
{
    return mChannel;
}

int Client::id()
{
    return mID;
}

void Client::setID(const int id)
{
    mID = id;
}


QString Client::name()
{
    return mName;
}

void Client::setName(const QString &name)
{
    mName = name;
}

QHostAddress Client::IPAddress()
{
    return mTcpSocket->peerAddress();
}

int Client::port()
{
    return mTcpSocket->peerPort();
}

void Client::setUdpPort(int port)
{
    mUdpPort = port;
}

int Client::udpPort()
{
    return mUdpPort;
}



unsigned int Client::tcpPacketReceived()
{
    return mTcpPacketReceived;

}

unsigned int Client::udpPacketReceived()
{
    return mUdpPacketReceived;
}


void Client::incrementTcpPacketReceived()
{
    mTcpPacketReceived++;
}

void Client::incrementUdpPacketReceived()
{
    mUdpPacketReceived++;
}

bool Client::udpAvailable()
{
    return mUdpAvailable;
}

void Client::setUdpAvailable(const bool availaible)
{
    mUdpAvailable = availaible;
}

void Client::moveToChannel(Fhenix::Channel* channel)
{
    if(!channel)
        return;

    // if the client is already in a channel, remove from it
    if(mChannel)
        mChannel->removeClient(this);

    // Set the new channel as current channel
    mChannel = channel;

    // Add the client into new channel (now current)
    mChannel->addClient(this);

}




void Client::sendPacket(const QByteArray &packet)
{
   mTcpSocket->write(packet);
}

void Client::dataReceived()
{
    if (!mTcpSocket)
        return;

    Shared::DataStream in(mTcpSocket);

    while(mTcpSocket->bytesAvailable())
    {
        if(!mPacketSize)
        {
            if(mTcpSocket->bytesAvailable() < (int)sizeof(quint16))
                return;

            in >> mPacketSize;
            //qWarning() << "Packet Size received :" << mPacketSize;
            qWarning() << QString("Packet (size : %1) Received (sender id : %2)")
                          .arg(mPacketSize)
                          .arg(id());
        }

        if(mTcpSocket->bytesAvailable() < mPacketSize)
            return;

        char* buffer = new char[mPacketSize];

        mTcpSocket->read(buffer, mPacketSize);


        QByteArray packet(buffer, mPacketSize);

        incrementTcpPacketReceived();

        emit(packetReceived(packet));

        mPacketSize = 0;
    }

}



void Client::handlePacket(QByteArray packet)
{
    Shared::DataStream in(packet);

    quint8 opcode;
    in >> opcode;

    switch(opcode)
    {
        case Opcodes::CMSG_GET_CHANNEL_LIST:
            qWarning() << "Channel List Requested";
            emit(ChannelListRequested());
        break;
        case Opcodes::CMSG_GET_CLIENT_LIST:
            qWarning() << "Client List Requested";
            emit(ClientListRequested());
        break;
        case Opcodes::CMSG_GET_SERVER_INFO:
            qWarning() << "Server Info Requested";
            emit(serverInfoRequested());
        break;
        case Opcodes::MSG_SEND_MESSAGE:
        {
            QString msg;
            qint8 targetType;
            qint16 targetID;

            in >> msg >> targetType >> targetID;
            emit(messageReceived(msg, static_cast<Shared::TargetType>(targetType), targetID));
        }
        break;
        case Opcodes::CMSG_SWITCH_CHANNEL:
        {
            qint8 channelID;
            QString password;

            in >> channelID >> password;

            emit(switchChannelRequested(channelID, password));

        }
        break;
        case Opcodes::MSG_UDP_TEST:
        {
            QString test;
            in >> test;
            qWarning() << test;
        }
        break;
        case Opcodes::MSG_VOICE_DATA:
        {
            QByteArray voiceData;
            in >> voiceData;

            qWarning() << QString("[VOICE DATA] Bytes Voice (%1 )").arg(voiceData.size());
            qWarning() << "[VOICE SENDER INFORMATION]" << "Sender Ip : " << mTcpSocket->peerAddress() << " / Sender Port : " << mUdpPort;
            emit(voiceDataReceived(voiceData));

        }
        break;


    }




}
}
