#include "NativeWindowsInput.h"
#include <QDebug>

#ifdef Q_OS_WIN
namespace Fhenix
{

NativeWindowsInput* NativeWindowsInput::mNativeWindowsInput = nullptr;
HHOOK NativeWindowsInput::mHook = NULL;

NativeWindowsInput::NativeWindowsInput()
{
    mHook = SetWindowsHookEx(WH_KEYBOARD_LL, handleWindowsRawInput, NULL, 0);
    mPreviousKeyPressed = -1;
}

NativeWindowsInput* NativeWindowsInput::get()
{
    if(!mNativeWindowsInput)
        mNativeWindowsInput = new NativeWindowsInput;

    return mNativeWindowsInput;

}

void NativeWindowsInput::UpdateKeyState(BYTE *keystate, int keycode)
{
    keystate[keycode] = GetKeyState(keycode);
}

void NativeWindowsInput::setKeyPressed(int key, bool pressed)
{
    mKeysPressed[key] = pressed;
}

LRESULT CALLBACK NativeWindowsInput::handleWindowsRawInput(int nCode, WPARAM wParam, LPARAM lParam)
{
    //qDebug() << "Key pressed";

    if (wParam == WM_KEYDOWN || wParam == WM_KEYUP){
        //Get the key information
        KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);

        wchar_t buffer[5];

        //get the keyboard state
        BYTE keyboard_state[256];
        GetKeyboardState(keyboard_state);
        UpdateKeyState(keyboard_state, VK_SHIFT);
        UpdateKeyState(keyboard_state, VK_CAPITAL);
        UpdateKeyState(keyboard_state, VK_CONTROL);
        UpdateKeyState(keyboard_state, VK_MENU);

        //Get keyboard layout
        HKL keyboard_layout = GetKeyboardLayout(0);

        //Get the name
        char lpszName[0X100] = {0};

        DWORD dwMsg = 1;
        dwMsg += cKey.scanCode << 16;
        dwMsg += cKey.flags << 24;

        int i = GetKeyNameText(dwMsg, (LPTSTR)lpszName, 255);

        //Try to convert the key information
        int result = ToUnicodeEx(cKey.vkCode, cKey.scanCode, keyboard_state, buffer, 4, 0, keyboard_layout);
        buffer[4] = L'\0';

        //Print the output
        //qDebug() << "Key: " << cKey.vkCode << " " << QString::fromUtf16((ushort*)buffer) << " " << QString::fromUtf16((ushort*)lpszName);

        if(wParam == WM_KEYDOWN )
           NativeWindowsInput::get()->emitKeyPressedSignal(cKey.vkCode);
        else if(wParam == WM_KEYUP)
           NativeWindowsInput::get()->emitKeyReleasedSignal(cKey.vkCode);


    }

    return CallNextHookEx(mHook, nCode, wParam, lParam);
}

void NativeWindowsInput::emitKeyPressedSignal(int keyCode)
{
    emit(KeyPressed(keyCode, mPreviousKeyPressed));
    mPreviousKeyPressed = keyCode;
}

void NativeWindowsInput::emitKeyReleasedSignal(int keyCode)
{
    emit(KeyReleased(keyCode, -1));
    mPreviousKeyPressed = -1;

}

bool NativeWindowsInput::isKeyPressed(int key)
{
    qDebug() << "Native Windows Input : " << key << " " << mKeysPressed.value(key, false);
    return mKeysPressed.value(key, false);
}



}

#endif
