#include "KeyInputButton.h"
#include <qdebug.h>

KeyInputButton::KeyInputButton(QWidget *parent) :
    QPushButton(parent)
{
    mKeysBuffer = QVector<int>(4, 0);
}

void KeyInputButton::setInputListened(bool listened)
{

    mInputListened = listened;
}

bool KeyInputButton::inputListened() const
{
    return mInputListened;
}

void KeyInputButton::keyPressEvent(QKeyEvent *e)
{
   if(!mInputListened)
       e->ignore();

   //setText(e->text());
   qDebug() << "KeyCode " << e->key();
   qDebug() << "Modifier " << e->modifiers();
   qDebug() << "Raw key/Modifier/VS" << e->nativeScanCode() << " / " << e->nativeModifiers() << " / " << e->nativeVirtualKey();
   qDebug() << "Ctrl " << Qt::CTRL;

   //if e->modifiers();
   // if e->key()  >= 0x01000020 && e->key() <= 0x

   for(int i = 0; i < 4; i++)
   {
       int key = mKeysBuffer[i];
       /*if(key && key == e->key())
       {
          e->ignore();
          return;
       }
       else
       {

       }
       if(mKeysBuffer[i] == e->key())
        {
           e->ignore();
            return;
        }*/

       if(key)
       {
           if(key == e->key())
           {
              e->ignore();
              break;
           }

       }
       else
       {
           mKeysBuffer[i] = e->key();
           break;
       }
   }
   //setText(QKeySequence(mKeysBuffer[0], mKeysBuffer[1], mKeysBuffer[2], mKeysBuffer[3]).toString());
   //setText(QKeySequence(mKeysBuffer[0), mKeysBuffer[1], mKeysBuffer[2], mKeysBuffer[3]).toString());
   setText(QKeySequence(0x4000000 + mKeysBuffer[0] + mKeysBuffer[2] + mKeysBuffer[3]).toString());

}
