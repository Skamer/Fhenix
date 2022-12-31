#ifndef FHENIX_SERVER_CONNECTION_DIALOG_H
#define FHENIX_SERVER_CONNECTION_DIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHostAddress>

namespace Fhenix
{

    class ServerConnectionDialog : public QDialog
    {
        Q_OBJECT
    public:
        explicit ServerConnectionDialog(QWidget *parent = 0);

    signals:
        void ConnectionInfoChanged(QHostAddress, int, QString, QString);

    public slots:

    private:

        // LineEdit
        QLineEdit* mServerAddress;
        QLineEdit* mServerPort;
        QLineEdit* mUsername;
        QLineEdit* mServerPassword;

        // Button
        QPushButton* mConnectButton;
        QPushButton* mCancelButton;

        // Layout
        QVBoxLayout* mLayout;
        QHBoxLayout* mButtonLayout;
        QFormLayout* mFormLayout;

    };

}

#endif // CONNECTIONWIDGET_H
