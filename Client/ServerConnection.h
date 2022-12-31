#ifndef FHENIX_SERVER_CONNECTION_H
#define FHENIX_SERVER_CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QTimer>
#include "../Shared/DataStream.h"
#include "../Shared/Opcodes.h"
#include "../Shared/NetworkUtils.h"
#include "../Shared/NetworkStructs.h"
#include "../Shared/CommonEnums.h"
#include <QTextEdit>


namespace Fhenix
{
    class ServerConnection : public QObject
    {
        Q_OBJECT
    public:
        explicit ServerConnection(QObject *parent = 0);
        ~ServerConnection();

    public slots:
        void dataReceived();
        void readPendingDatagram();
        void connectToHost(const QHostAddress& host, const int port, const QString& usernanme, const QString& password);
        void handlePacket(const QByteArray& packet);
        void sendConnectRequest();
        void sendMessage(const QString& text, const Shared::TargetType targetType, const int targetID);
        void sendSwitchChannelRequest(const int id, const QString& password);
        void sendVoiceData(const QByteArray& data);
        void sendPingRequest();
        void sendCreateUdpConnectionRequest();

    signals:
        void packetReceived(const QByteArray&);
        void channelListReceived(const QList<Shared::ChannelInfo>&);
        void clientListReceived(const QList<Shared::ClientInfo>&);
        void clientIDUpdated(const int);
        void messageReceived(const QString&, const Shared::TargetType, const int, const int);
        void serverInfoReceived(const Shared::ServerInfo&);
        void clientChannelSwitched(const int, const int);
        void newClientConnected(const Shared::ClientInfo&);
        void clientDisconnected(const int);
        void voiceDataReceived(const QByteArray&);
        void clientSpeaking(const int);

    private:
        QTcpSocket* mTcpSocket;
        QUdpSocket* mUdpSocket;
        bool mConnected;
        bool mSocketConnected;
        quint16 mPacketSize;
        QString mUsername;
        QString mServerPassword;
        int mClientID;
        QTimer* mPingTimer;
        QTimer* mUdpConnectionTimer;
        int  mPort;
        bool mUdpAvailaible;

    public:
        template <typename ...Params>
        void notify(Protocol protocol, Opcodes opcode, Params&& ...params)
        {
            if(mConnected && mSocketConnected)
            {
               QByteArray packet = Shared::makePacket(opcode, params...);

               if(protocol == Protocol::TCP)
                    mTcpSocket->write(packet);
               else if(protocol == Protocol::UDP)
                    mUdpSocket->writeDatagram(packet, mTcpSocket->peerAddress(), 20021);
            }
        }



    };

}

#endif // SERVERCONNECTION_H
