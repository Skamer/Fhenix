#include "ServerViewerWidget.h"
#include "ServerConnection.h"
#include <qdebug.h>

namespace Fhenix
{

    ServerViewerWidget::ServerViewerWidget(QWidget* parent, ServerConnection* serverConnection) :
        QTreeWidget(parent),
        mServerConnection(serverConnection)
    {
        mServerItem = new QTreeWidgetItem(this, ServerType);

        setColumnCount(1);
        setContextMenuPolicy(Qt::CustomContextMenu);
        setRootIsDecorated(false);
        setExpandsOnDoubleClick(false);
        setHeaderHidden(true);

        mServerItem->setExpanded(true);

        // Connect
        connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequested(QPoint)));
        connect(this, &QTreeWidget::itemDoubleClicked, [&](QTreeWidgetItem* item, int column)
        {
            int channelID = findChannelID(item);
            emit(switchChannelRequested(channelID, QString("")));
        });

    }

    void ServerViewerWidget::addChannel(const int id, const QString& name, const int parent, const QPixmap& icon)
    {
        if(parent < -1)
            return;

        QTreeWidgetItem* channelItem = nullptr;

        if (parent == -1)
        {
            channelItem = new QTreeWidgetItem(ChannelType);
            channelItem->setText(0, name);
            mServerItem->addChild(channelItem);

        }
        else
        {
            QTreeWidgetItem* channelParentItem = mChannelItems[parent];

            if(!channelParentItem)
            {
                qDebug() << "Channel parent not found !";
                return;
            }

            channelItem = new QTreeWidgetItem(ChannelType);
            channelItem->setText(0, name);
            channelItem->setIcon(0, icon);
            channelParentItem->addChild(channelItem);
            channelParentItem->setExpanded(true);


        }


        mChannelItems[id] = channelItem;
     }


    void ServerViewerWidget::addChannel(const Shared::ChannelInfo& channel)
    {
        QPixmap icon;
        icon.loadFromData(channel.icon);

        addChannel(channel.id, channel.name, channel.parent, icon);
    }

    void ServerViewerWidget::addClient(const int id, const QString& name, const int channelID)
    {
        QTreeWidgetItem* channel = mChannelItems[channelID];

        if(!channel)
            return;

        if(mClientItems.contains(id))
            return;

        QTreeWidgetItem* clientItem = new QTreeWidgetItem(ClientType);
        clientItem->setText(0, name);
        clientItem->setIcon(0, QIcon("images/mic_idle.png"));
        channel->addChild(clientItem);

        mClientItems[id] = clientItem;

    }

    void ServerViewerWidget::addClient(const Shared::ClientInfo &client)
    {
        addClient(client.id, client.name, client.channel);
    }

    QString ServerViewerWidget::channelName(const int id) const
    {
        QTreeWidgetItem* channelItem = mChannelItems[id];

        if(channelItem)
            return channelItem->text(0);
    }


    int ServerViewerWidget::clientID() const
    {
        return mClientID;
    }

    QString ServerViewerWidget::clientName(const int id) const
    {
        if(!mClientItems.contains(id))
            return QString("");

        QTreeWidgetItem* clientItem = mClientItems.value(id);

        if(clientItem)
            return clientItem->text(0);


        return QString("");
    }

    int ServerViewerWidget::findChannelID(QTreeWidgetItem *item)
    {
        if(!item)
            return -1;

        for(auto  it = mChannelItems.cbegin(); it != mChannelItems.cend(); it++)
        {
            if(it.value() == item)
                return it.key();
        }

    }

    void ServerViewerWidget::moveClient(const int id, const int channelID)
    {
        QTreeWidgetItem* client = mClientItems[id];
        QTreeWidgetItem* channel = mChannelItems[channelID];

        if(!client || !channel)
            return;

        QTreeWidgetItem* clientParent = client->parent();

        if(!clientParent)
            return;

        clientParent->removeChild(client);

        channel->addChild(client);
        channel->setExpanded(true);
    }

    void ServerViewerWidget::removeClient(const int id)
    {
        QTreeWidgetItem* client = mClientItems[id];

        if(!client)
            return;

        QTreeWidgetItem* clientParent = client->parent();

        if(!clientParent)
            return;

        clientParent->removeChild(client);

        mClientItems.remove(id);

        delete client;


    }

    void ServerViewerWidget::setServerName(const QString& name)
    {
        mServerItem->setText(0, name);
    }

    void ServerViewerWidget::setClientID(const int id)
    {
        mClientID = id;
    }

    void ServerViewerWidget::clientSpeaking(const int id)
    {
        QTimer* speakingTimer;

        if(mClientSpeakingHooks.contains(id))
        {
            speakingTimer = mClientSpeakingHooks[id];
        }
        else
        {
            speakingTimer = new QTimer;
            mClientSpeakingHooks[id] = speakingTimer;
            speakingTimer->setProperty("client id", id);

            connect(speakingTimer, SIGNAL(timeout()), this, SLOT(clientSpeakingTimeOut()));
            mClientItems[id]->setIcon(0, QIcon("images/mic_speaking.png"));

        }

        speakingTimer->start(100);
    }


    // ********************************************************************
    // *                          SLOT                                    *
    // ********************************************************************

    void ServerViewerWidget::contextMenuRequested(QPoint pos)
    {
        QTreeWidgetItem* item = itemAt(pos);

        if(item)
        {
            showContextMenu(item, viewport()->mapToGlobal(pos));
        }
    }

    void ServerViewerWidget::showContextMenu(QTreeWidgetItem *item, const QPoint &globalPos)
    {
        QMenu menu;

        switch(item->type())
        {
        case ServerType:
            menu.addAction("This is a server type");
            break;
        case ChannelType:
        {
            if(mClientItems[mClientID]->parent() != item)
            {
                QAction* switchToChannelAction = menu.addAction("Switch to Channel");
                menu.addSeparator();             

                connect(switchToChannelAction, &QAction::triggered, [&](){
                   int channelID = findChannelID(item);
                   emit(switchChannelRequested(channelID, QString("")));
                });

            }

            //menu.addAction("This is a channel type");
            QAction* createChannelAction = menu.addAction("Create Channel");
            QAction* createSubChannelAction = menu.addAction("Create Sub-Channel");

            menu.addSeparator();

            QAction* editChannelAction = menu.addAction("Edit Channel");
            QAction* deleteChannelAction = menu.addAction("Delete Channel");
            break;
       }
        case ClientType:
            if(mClientItems[mClientID] != item)
                menu.addAction("Open text chat");

            menu.addAction("This is a client type");

        break;


        }

        menu.exec(globalPos);
    }
    void ServerViewerWidget::clientSpeakingTimeOut()
    {
        QObject* timerSender = sender();
        const int clientID = timerSender->property("client id").toInt();

        mClientSpeakingHooks.remove(clientID);
        mClientItems[clientID]->setIcon(0, QIcon("images/mic_idle.png"));

        delete timerSender;

        timerSender = nullptr;


    }

}
