#ifndef KEYINPUTBUTTON_H
#define KEYINPUTBUTTON_H

#include <QPushButton>
#include <QKeyEvent>
#include <QVector>

class KeyInputButton : public QPushButton
{
    Q_OBJECT
public:
    explicit KeyInputButton(QWidget *parent = 0);
    void setInputListened(bool);
    bool inputListened() const;

protected:
    void keyPressEvent(QKeyEvent*);

signals:

public slots:

private:
    bool mInputListened;
    QKeySequence mKeySequence;
    QVector<int> mKeysBuffer;

};

#endif // KEYINPUTBUTTON_H
