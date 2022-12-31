#ifndef FHENIX_TEXT_CHAT_WIDGET_H
#define FHENIX_TEXT_CHAT_WIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QTime>

namespace Fhenix
{

    class TextChatWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit TextChatWidget(QWidget *parent = 0);
        void addMessage(const QString& msg, const bool showTimestamp = true);
        static QString autoFormat(QString msg);

    signals:

    public slots:


    private:
        QTextBrowser* mTextBrowser;
        QVBoxLayout* mLayout;

    };

}
#endif // CHATWIDGET_H
