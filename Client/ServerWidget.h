#ifndef FHENIX_SERVER_WIDGET_H
#define FHENIX_SERVER_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include "ServerViewerWidget.h"
#include <QMap>
#include "TextChatWidget.h"
#include "../Shared/CommonEnums.h"
#include <QSplitter>
#include <QLineEdit>

namespace Fhenix
{
    class ServerConnection;
    class ServerWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit ServerWidget(QWidget *parent = 0, ServerConnection* serverConnection = 0);

        /* create a new tab for whisper
         * @arg1(name) client name whispered
         * @arg2(id) client id whispered
         */
        void openClientTextChat(const QString& name, const int id);

        /* find the client with from TextChatWidget
         * @arg2(id) ClientTextChat pointer to search
         */
        int findClientID(TextChatWidget* clientTextChat);

    public slots:
        void newClient(const Shared::ClientInfo& client);
        void removeClient(const int id);
        void updateChannelList(const QList<Shared::ChannelInfo>& channelList);
        void updateClientChannel(const int id, const int channelID);
        void updateClientList(const QList<Shared::ClientInfo>& clientList);
        void updateLocalClientID(const int id);
        void updateMessage(const QString& msg, const Shared::TargetType targetType, const int targetID, const int senderID);
        void updateServerInfo(const Shared::ServerInfo& serverInfo);
        void updateClientSpeaking(const int id);

    signals:
        void clientMessageReady(QString, Shared::TargetType, int);


    private:
        // Text Chat
        QMap<int, TextChatWidget*> mClientsTextChat;
        TextChatWidget* mChannelTextChat;
        TextChatWidget* mServerTextChat;

        // Tab Widget
        QTabWidget* mTabTextChat;

        // Server Viewer
        ServerViewerWidget* mServerViewer;

        // QSplitter
        QSplitter* mSplitter;

        // Layout
        QVBoxLayout* mLayout;

        // QLineEdit
        QLineEdit* mInputTextChat;

        ServerConnection* mServerConnection;


        int mDefaultChannel;
    };

}

#endif // SERVERWIDGET_H
