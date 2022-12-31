#ifndef NATIVEINPUTFILTER_H
#define NATIVEINPUTFILTER_H

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QAbstractNativeEventFilter>
#include <QTime>
#include <QApplication>

class NativeInputFilter : public QAbstractNativeEventFilter
{
public:
    NativeInputFilter() { }

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result)
    {
        //qDebug() << eventType;

        #ifdef Q_OS_WIN
            if(eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
        {
            MSG* msg = static_cast<MSG*>(message);
            LRESULT* lResult = static_cast<LRESULT*>(result);
              qDebug() << msg->message;

              if(msg->message == WM_SYSKEYDOWN )
              {
                  qDebug() << "Key DOWN";
              }
              else if(msg->message == WM_SYSKEYDOWN)
              {
                  qDebug() << "Key Up";
              }


        }
        #endif

        //qDebug() << QTime::currentTime().toString() <<  " event";

        return false;
    }

};

#endif // NATIVEINPUTFILTER_H
