#include "ServerConnectionDialog.h"

namespace Fhenix
{
    ServerConnectionDialog::ServerConnectionDialog(QWidget *parent) :
        QDialog(parent)
    {

        // Dynamic Allocation
        mButtonLayout = new QHBoxLayout;
        mConnectButton = new QPushButton("Connect");
        mCancelButton = new QPushButton("Cancel");
        mFormLayout = new QFormLayout;
        mLayout = new QVBoxLayout;
        mServerAddress = new QLineEdit;
        mServerPassword = new QLineEdit;
        mServerPort = new QLineEdit;
        mUsername = new QLineEdit;


        // hide the caracters for the password
        mServerPassword->setEchoMode(QLineEdit::Password);

        // Add our LineEdit into the form layout
        mFormLayout->addRow("Server Address", mServerAddress);
        mFormLayout->addRow("Server Port", mServerPort);
        mFormLayout->addRow("Username", mUsername);
        mFormLayout->addRow("Server Password", mServerPassword);

        // Add our Buttons into the button layout
        mButtonLayout->addWidget(mConnectButton);
        mButtonLayout->addWidget(mCancelButton);

        // Add our layouts into widget layout
        mLayout->addLayout(mFormLayout);
        mLayout->addLayout(mButtonLayout);

        setLayout(mLayout);

        // Window configuration
        setWindowTitle("Connect");
        hide();

        // Connect the signal and slot
        connect(mCancelButton, SIGNAL(clicked()), this, SLOT(hide()));
        connect(mConnectButton, &QPushButton::clicked, [&](){
            if(mServerAddress->text() != "")
            {
                emit(ConnectionInfoChanged(QHostAddress(mServerAddress->text()), mServerPort->text().toInt(), mUsername->text(), mServerPassword->text()));
                qWarning() << "clicked";
                hide();
            }

        });
    }

}
