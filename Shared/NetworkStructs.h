#ifndef FHENIX_NETWORK_STRUCTS_H
#define FHENIX_NETWORK_STRUCTS_H

#include <QtGlobal>
#include <QDataStream>
#include <QByteArray>
//#include "DataStream.h"

namespace Fhenix
{
    namespace Shared
    {
        struct ClientInfo
        {
            ClientInfo(QString name, quint8 id, QString uid = "", quint8 channel = 0) :
                name(name), id(id), uid(uid), channel(channel)
            {

            }

            ClientInfo()
            {
            }

            QString name;
            QString uid;
            quint8 id;
            quint8 channel;

        };

        inline QDataStream& operator << (QDataStream &out, const ClientInfo &clientInfo)
        {
            out << clientInfo.name << clientInfo.uid << clientInfo.id << clientInfo.channel;
            return out;
        }

        inline QDataStream& operator >> (QDataStream &in, ClientInfo &clientInfo)
        {
            in >> clientInfo.name >> clientInfo.uid >> clientInfo.id >> clientInfo.channel;
            return in;
        }


        struct ChannelInfo
        {
            ChannelInfo()
            {
            }

            ChannelInfo(QString name, quint8 id, qint8 parent = -1) :
                name(name), id(id), parent(parent)
            {

            }

            QString name;
            quint8 id;
            qint8 parent;
            QByteArray icon;
        };

        inline QDataStream& operator << (QDataStream &out, const ChannelInfo& channelInfo)
        {
            out << channelInfo.name
                << channelInfo.id
                << channelInfo.parent
                << channelInfo.icon;
            return out;
        }

        inline QDataStream& operator >> (QDataStream& in, ChannelInfo& channelInfo)
        {
            in >> channelInfo.name
               >> channelInfo.id
               >> channelInfo.parent
               >> channelInfo.icon;
            return in;
        }

        struct ServerInfo
        {
            ServerInfo()
            {
            }

            QString name;
            quint8 defaultChannel;

        };

        inline QDataStream& operator << (QDataStream &out, const ServerInfo& serverInfo)
        {
            out << serverInfo.name
                << serverInfo.defaultChannel;
            return out;
        }

        inline QDataStream& operator >> (QDataStream &in, ServerInfo& serverInfo)
        {
            in >> serverInfo.name
               >> serverInfo.defaultChannel;
            return in;
        }

    }

}


#endif
