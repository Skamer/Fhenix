#ifndef FHENIX_CLIENT_H
#define FHENIX_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QUdpSocket>
#include "../Shared/Opcodes.h"
#include "../Shared/CommonEnums.h"
#include "../Shared/DataStream.h"

namespace Fhenix
{

class Channel;
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QTcpSocket* socket, Channel* channel, QObject *parent = 0);

    void sendPacket(const QByteArray& packet);
    int id();
    void setID(const int id);

    void moveToChannel(Channel* channel);
    void setName(const QString& name);
    QString name();
    Channel* channel();

    QHostAddress IPAddress();
    int port();

    int udpPort();
    void setUdpPort(int port);

    unsigned int udpPacketReceived();
    unsigned int tcpPacketReceived();

    void incrementTcpPacketReceived();
    void incrementUdpPacketReceived();

    bool udpAvailable();
    void setUdpAvailable(const bool availaible);

signals:
    void packetReceived(QByteArray packet);
    void ChannelListRequested();
    void ClientListRequested();
    void messageReceived(QString, Shared::TargetType, int);
    void serverInfoRequested();
    void switchChannelRequested(int, QString);
    void disconnected();
    void voiceDataReceived(const QByteArray&);

public slots:
    void dataReceived();
    void handlePacket(QByteArray packet);

private:
    QTcpSocket* mTcpSocket;
    QUdpSocket* mUdpSocket;
    int mID;
    quint16 mPacketSize;
    Channel* mChannel;
    QString mName;
    int mUdpPort;
    int mLocalUdpPort;

    //
    unsigned int mUdpPacketReceived;
    unsigned int mTcpPacketReceived;

    bool mUdpAvailable;

};

}

#endif // CLIENT_H
