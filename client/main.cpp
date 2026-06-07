#include <QApplication>
#include "loginwindow.h"
#include "networkmanager.h"
#include <windows.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SetConsoleOutputCP(CP_UTF8);

    // Инициализируем синглтон и подключаемся к серверу ОДИН РАЗ
    // Все окна будут использовать это же соединение
    NetworkManager::getInstance()->connectToServer("127.0.0.1", 33333);

    LoginWindow window;
    window.show();

    return app.exec();
}