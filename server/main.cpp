#include <QCoreApplication>
#include "mytcpserver.h"
#include "databasemanager.h"
#include <windows.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SetConsoleOutputCP(CP_UTF8);

    // Подключаем БД через синглтон
    if (!DatabaseManager::getInstance()->connectToDatabase()) {
        qCritical() << "Критическая ошибка: Приложение запущено без БД.";
        return -1;
    }

    // Запускаем сервер
    MyTcpServer server;

    return a.exec();
}
