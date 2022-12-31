#ifndef NATIVEWINDOWSINPUT_H
#define NATIVEWINDOWSINPUT_H




#include <QObject>
#include <QMap>

#ifdef Q_OS_WIN32
#include <windows.h>

namespace Fhenix
{

class NativeWindowsInput : public QObject
{
    Q_OBJECT
public:
    static NativeWindowsInput* get();

    static void UpdateKeyState(BYTE *keystate, int keycode);

    static LRESULT CALLBACK handleWindowsRawInput(int nCode, WPARAM wParam, LPARAM lParam);

    void emitKeyPressedSignal(int keyCode);
    void emitKeyReleasedSignal(int keyCode);

    bool isKeyPressed(int key);

    void setKeyPressed(int key, bool pressed);

signals:
    void KeyPressed(int, int);
    void KeyReleased(int, int);


private:
    explicit NativeWindowsInput();
    static NativeWindowsInput* mNativeWindowsInput;
    static HHOOK mHook;
    int mPreviousKeyPressed;

    QMap<int, bool> mKeysPressed;


};


}

#endif



#endif // NATIVEWINDOWSINPUT_H
