#ifndef FHENIX_SERVER_VIEWER_WIDGET_H
#define FHENIX_SERVER_VIEWER_WIDGET_H

#include <QTreeWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QMap>
#include <QTimer>

#include "../Shared/NetworkStructs.h"


static const int ServerType = QTreeWidgetItem::UserType;
static const int ChannelType = QTreeWidgetItem::UserType + 1;
static const int UserType = QTreeWidgetItem::UserType + 2;
static const int ClientType = UserType;

namespace Fhenix
{
    class ServerConnection;
    class ServerViewerWidget : public QTreeWidget
    {
        Q_OBJECT
    public:
        ServerViewerWidget(QWidget* parent = 0, ServerConnection* serverConnection = 0);

        /* Add a channel into ServerViewer
         * @Arg1(channelID) : Channel id
         * @Arg2(name) : Chanel name
         * @Arg3(parent) : Channel's parent id
         * @Arg4(icon) : Channel icon
         */
        void addChannel(const int id, const QString& name, const int parentID = -1,const QPixmap& icon = QPixmap());

        /* Overload Method 1
         * Add a channel into Server Viewer
         * @Arg1(channelInfo) : ChannelType
         */
        void addChannel(const Shared::ChannelInfo& channel);

        /* Add a client into Server
         * @Arg1(clientID) : client id
         * @Arg2(QString) : client name
         * @Arg3(channelID) : channel id where the client is
         */
        void addClient(const int id, const QString& name, const int channelID);

        /* Overload Method 1
         * Add a client into server Viewer
         * @Arg1(client) : client to addA
         */
        void addClient(const Shared::ClientInfo& client);

        /* Get the channel name by id
         * @arg1(id) : channel id to search
         * @return : channel name
         */
        QString channelName(const int id) const;

        /* Return the local client id
         * @return client id
         */
        int clientID() const;

        /* Return the client name by id
         * @param client id to search
         * @return client name
         */
        QString clientName(const int id) const;

        /* Find channel id by item
         * @arg1(item) : channel item to search
         * @return : channel id
         */
        int findChannelID(QTreeWidgetItem* item);

        /* Move the client into another channel
         * @Arg1(clientID) : client to move
         * @Arg2(channelID) : new client channel
         */
        void moveClient(const int id, const int channelID);

        /* Remove the channel from server viewer
         * @arg1(channelID) : channel to remove
         */
        void removeChannel(const int id);

        /* Remove the client from server viewer
         * @arg1(clientID) : client to remove
         */
        void removeClient(const int id);

        /* Set the local client id
         * @arg1(clientID) : local client id to set
         */
        void setClientID(const int id);

        /* Set the server name shown in the server widget
         * @Arg1(name) : Server name3
         */
        void setServerName(const QString& name);

        void clientSpeaking(const int id);

    signals:
        void switchChannelRequested(int, QString);

    private slots:
        void contextMenuRequested(QPoint pos);
        void showContextMenu(QTreeWidgetItem* item, const QPoint& globalPos);
        void clientSpeakingTimeOut();

    private:
        int mClientID;
        QTreeWidgetItem* mServerItem;
        QMap<int, QTreeWidgetItem*> mChannelItems;
        QMap<int, QTreeWidgetItem*> mClientItems;
        ServerConnection* mServerConnection;
        QMap<int, QTimer*> mClientSpeakingHooks;

    };

}

#endif // SERVERVIEWERWIDGET_H
