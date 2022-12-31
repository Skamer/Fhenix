#include "TextChatWidget.h"
#include <QRegExp>

namespace Fhenix
{

    TextChatWidget::TextChatWidget(QWidget *parent) :
        QWidget(parent)
    {
        mTextBrowser = new QTextBrowser;
        mLayout = new QVBoxLayout;

        mLayout->addWidget(mTextBrowser);
        setLayout(mLayout);
        mTextBrowser->setReadOnly(true);
        mTextBrowser->setOpenExternalLinks(true);

        //mTextEdit->setTextInteractionFlags(Qt::TextBrowserInteraction);
    }

    void TextChatWidget::addMessage(const QString& msg, const bool showTimestamp)
    {

        if(showTimestamp)
        {
            QString timeString = QTime::currentTime().toString();

            mTextBrowser->insertHtml(QString("&#60;%1&#62; %2 <br />").arg(timeString).arg(autoFormat(msg)));
        }
        else
        {
            mTextBrowser->insertHtml(QString("%1 <br />").arg(autoFormat(msg)));
        }
    }

    QString TextChatWidget::autoFormat(QString msg)
    {
        QRegExp r;
        r.setPatternSyntax(QRegExp::RegExp2);
        r.setPattern("[a-z-]+://[^ <]*");
        r.setMinimal(false);

        int idx = 0;
        do {
            idx = r.indexIn(msg, idx);
            if(idx >= 0)
            {
                QString url = r.capturedTexts().at(0);
                QUrl u(url);
                if(u.isValid())
                {
                    int len = r.matchedLength();
                    QString replacement = QString("<a href=\"%1\">%1</a>").arg(url);
                    msg.replace(idx, len, replacement);
                    idx += replacement.length();
                }
                else
                {
                    idx++;
                }
            }

        } while(idx >= 0);

        return msg;
    }

}
