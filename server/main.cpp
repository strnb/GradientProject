#include <QCoreApplication>
#include "mytcpserver.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Подключаем БД через синглтон
    if (!DatabaseManager::getInstance()->connectToDatabase()) {
        qCritical() << "Критическая ошибка: Приложение запущено без БД.";
        return -1;
    }

    // Запускаем сервер
    MyTcpServer server;

    return a.exec();
}
