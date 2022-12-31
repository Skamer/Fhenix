#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
//#include "NativeInputFilter.h"

/*#ifdef Q_OS_WIN
#include <windows.h>

HHOOK hHook = NULL;

LRESULT CALLBACK windowsInputHook(int nCode, WPARAM wParam, LPARAM lParam)
{

    qDebug() << "Press windows key";

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

#endif*/

#include "NativeWindowsInput.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Fhenix::MainWindow w;
    w.show();
    //Fhenix::NativeWindowsInput::get();

    //a.installNativeEventFilter(&w);
    //a.installNativeEventFilter(filter);
    qDebug() << QApplication::libraryPaths();

    //hHook = SetWindowsHookEx(WH_KEYBOARD_LL, windowsInputHook, NULL, 0);


    return a.exec();
}
