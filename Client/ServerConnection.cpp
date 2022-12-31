#include "ServerConnection.h"
#include "../Shared/NetworkStructs.h"
#include <QtDebug>
#include <QDataStream>
#include <QTime>

namespace Fhenix
{
    ServerConnection::ServerConnection(QObject *parent) :
        QObject(parent),
        mClientID(-1),
        mSocketConnected(false),
        mConnected(false),
        mUdpAvailaible(false),
        mPacketSize(0)
    {
        mTcpSocket = new QTcpSocket;
        mUdpSocket = new QUdpSocket;
        mPingTimer = new QTimer(this);
        mUdpConnectionTimer = new QTimer(this);

        connect(mUdpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagram()));
    }

    ServerConnection::~ServerConnection()
    {
        mTcpSocket->disconnectFromHost();
        mTcpSocket->deleteLater();

        mUdpSocket->disconnectFromHost();
        mUdpSocket->deleteLater();
    }

    void ServerConnection::dataReceived()
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
                //qDebug() << "Client Size Packet" << mPacketSize;
            }

            if(mTcpSocket->bytesAvailable() < mPacketSize)
                return;

            // Transfert the packet data into a buffer
            char* buffer = new char[mPacketSize];
            mTcpSocket->read(buffer, mPacketSize);

            // Create the packet with the buffer provided
            QByteArray packet(buffer, mPacketSize);

            // emit when the full packet is ready and send it into a function that handle it
            emit(packetReceived(packet));

            mPacketSize = 0;
        }
    }

    void ServerConnection::readPendingDatagram()
    {
        while(mUdpSocket->hasPendingDatagrams())
        {
            // Create the datagram
            QByteArray datagram;
            datagram.resize(mUdpSocket->pendingDatagramSize());

            quint16 size = mUdpSocket->pendingDatagramSize();

            // read the socket then transfert the data into our datagram
            mUdpSocket->readDatagram(datagram.data(), datagram.size());

            // calculate the lenght size content packet
            size = (quint16)(sizeof(datagram.size()) - sizeof(quint16));

            // remove the lenght content size into the datagram
            datagram = datagram.remove(0, size);

            emit(packetReceived(datagram));
        }
    }

    void ServerConnection::connectToHost(const QHostAddress& host, const int port, const QString& username, const QString& password)
    {
        connect(mTcpSocket, SIGNAL(connected()), this, SLOT(sendConnectRequest()));
        mTcpSocket->connectToHost(host, port);

        mServerPassword = password;
        mUsername = username;

    }

    void ServerConnection::sendConnectRequest()
    {
        mSocketConnected = true;
        connect(mTcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
        connect(this, SIGNAL(packetReceived(QByteArray)), this, SLOT(handlePacket(QByteArray)));

        // Send the connection request
        QByteArray packet = Shared::makePacket(Opcodes::CMSG_CONNECT, QString(""), mUsername, mServerPassword);
        mTcpSocket->write(packet);

    }

    void ServerConnection::sendMessage(const QString& text, const Shared::TargetType targetType, const int targetID)
    {
        notify(Protocol::TCP, Opcodes::MSG_SEND_MESSAGE, text, static_cast<qint8>(targetType), static_cast<qint16>(targetID));
    }

    void ServerConnection::sendSwitchChannelRequest(const int id, const QString& password)
    {
        notify(Protocol::TCP, Opcodes::CMSG_SWITCH_CHANNEL, static_cast<qint8>(id), password);
    }

    void ServerConnection::sendVoiceData(const QByteArray& data)
    {

        if(mUdpAvailaible)
            notify(Protocol::UDP, Opcodes::MSG_VOICE_DATA, data);
        else
            notify(Protocol::TCP, Opcodes::MSG_VOICE_DATA, data);


    }

    void ServerConnection::sendPingRequest()
    {

    }

    void ServerConnection::sendCreateUdpConnectionRequest()
    {
        notify(Protocol::UDP, Opcodes::CMSG_CREATE_UDP_CONNECTION, static_cast<quint16>(mClientID));
    }

    void ServerConnection::handlePacket(const QByteArray& packet)
    {
        QDataStream in(packet);

        quint8 opcode;

        in >> opcode;

        switch(opcode)
        {

            case Opcodes::SMSG_CHANNEL_LIST:
            {
                QList<Shared::ChannelInfo> channelInfoList;
                in >> channelInfoList;

                emit(channelListReceived(channelInfoList));

                notify(Protocol::TCP, Opcodes::CMSG_GET_CLIENT_LIST);


            }
            break;
            case Opcodes::SMSG_SERVER_INFO:
            {
                Shared::ServerInfo serverInfo;
                in >> serverInfo;

                emit(serverInfoReceived(serverInfo));

                notify(Protocol::TCP, Opcodes::CMSG_GET_CHANNEL_LIST);

            }
            break;

            case Opcodes::SMSG_CONNECT_RESULT:
            {
                mConnected = true;
                qint16 clientID;
                qint16 port;
                in >> clientID >> port;

                mPort = port;

                qWarning() << QString("Client ID : %1 / UDP Port %2").arg(clientID).arg(port);
                mClientID = clientID;

                emit(clientIDUpdated(clientID));

                // Send the requests to start the udp connection
                mUdpConnectionTimer->start(40);
                connect(mUdpConnectionTimer, SIGNAL(timeout()), this, SLOT(sendCreateUdpConnectionRequest()));


                notify(Protocol::TCP, Opcodes::CMSG_GET_SERVER_INFO);

            }
            break;

            case Opcodes::SMSG_CLIENT_LIST:
            {
                QList<Shared::ClientInfo> clientInfoList;
                in >> clientInfoList;

                emit(clientListReceived(clientInfoList));
            }
            break;

            case Opcodes::MSG_SEND_MESSAGE:
            {
                QString msg;
                qint8 targetType;
                qint16 targetID;
                qint16 senderID;

                in >> msg >> targetType >> targetID >> senderID;

                emit(messageReceived(msg, static_cast<Shared::TargetType>(targetType), targetID, senderID));
            }
            break;
        case Opcodes::SMSG_CLIENT_CHANNEL_SWITCHED:
        {
            qint8 clientID;
            qint8 channelID;

            in >> clientID >> channelID;

            emit(clientChannelSwitched(clientID, channelID));

        }
        break;
        case Opcodes::SMSG_NEW_CLIENT_CONNECTED:
        {
            Shared::ClientInfo clientInfo;
            in >> clientInfo;

            emit(newClientConnected(clientInfo));
        }
        break;
        case Opcodes::SMSG_CLIENT_DISCONNECTED:
        {
            qint8 clientID;
            in >> clientID;

            emit(clientDisconnected(clientID));
        }
        break;
        case Opcodes::MSG_UDP_TEST:
        {
            QString udpTest;
            in >> udpTest;

            qDebug() << "UDP Test : ->"  << udpTest;
            notify(Protocol::UDP, Opcodes::MSG_UDP_TEST, QString("Est ce que chez toi ? ça marche"));
        }
        break;
        case Opcodes::MSG_VOICE_DATA:
        {
            QByteArray data;
            quint8 clientID;
            in >> data >> clientID;



            if(data.size() != 0)
            {
                emit(voiceDataReceived(data));
                qDebug() << "Voice Data Received (Size : " << data.size() << ") by client id : " << clientID;
                emit(clientSpeaking(clientID));
            }
        }
        break;
        case Opcodes::SMSG_UDP_CONNECION_OK:
        {
            mUdpAvailaible = true;
            mPort = mUdpSocket->localPort();

            mUdpConnectionTimer->stop();

            qDebug() << "[UDP] protocol ok";
        }
        break;
        case Opcodes::SMSG_CLIENT_SPEAKING:
        {
            quint8 clientID;
            in >> clientID;

            emit(clientSpeaking(clientID));
        }

        }
    }

}
