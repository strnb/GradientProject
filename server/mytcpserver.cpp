#include "mytcpserver.h"
#include "databasemanager.h"
#include "../shared/crypto_utils.h" 
#include <QDebug>
#include <QStringList>
#include <sstream>
#include <vector>

// Предполагаем, что функции математики объявлены или подключены
double gradientDescent(const std::string& func, double start, double lr, int iter) { return start * 0.5; } // заглушка
double splineInterpolate(const std::vector<double>& x, const std::vector<double>& y, double query) { return 1.0; } // заглушка

MyTcpServer::MyTcpServer(QObject* parent) : QObject(parent) {
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Сервер не запущен.";
    }
    else {
        qDebug() << "Сервер запущен на порту 33333...";
    }
}

MyTcpServer::~MyTcpServer() {
    for (QTcpSocket* socket : mClientSockets) {
        socket->close();
    }
    mTcpServer->close();
}

void MyTcpServer::slotNewConnection() {
    QTcpSocket* clientSocket = mTcpServer->nextPendingConnection();
    mClientSockets.append(clientSocket);

    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

    clientSocket->write("CONNECTED;WELCOME\r\n");
}

void MyTcpServer::slotServerRead() {
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    while (clientSocket->bytesAvailable() > 0) {
        QByteArray array = clientSocket->readAll();
        QString request = QString::fromUtf8(array).trimmed();
        qDebug() << "Получен запрос:" << request;
        parseRequest(clientSocket, request);
    }
}

void MyTcpServer::parseRequest(QTcpSocket* socket, const QString& request) {
    QStringList tokens = request.split(';');
    if (tokens.isEmpty()) return;

    QString command = tokens.at(0);

    // 1. АВТОРИЗАЦИЯ
    if (command == "AUTH") {
        if (tokens.size() < 3) return;
        QString login = tokens.at(1);
        QString rawPassword = tokens.at(2);

        // Хардкод администратора по ТЗ
        if (login == "admin" && rawPassword == "12345") {
            socket->write("AUTH;SUCCESS;admin\r\n");
            DatabaseManager::getInstance()->logUserAction(QVariant(), "Login (Admin)");
            return;
        }

        std::string hashStr = hashPassword(rawPassword.toStdString());
        QString passwordHash = QString::fromStdString(hashStr);

        QString role = DatabaseManager::getInstance()->authUser(login, passwordHash);
        if (!role.isEmpty()) {
            socket->write(QString("AUTH;SUCCESS;%1\r\n").arg(role).toUtf8());
            // Для простоты пишем логи по имени, в реальной структуре можно хранить ID сессии
            DatabaseManager::getInstance()->logUserAction(1, "Login");
        }
        else {
            socket->write("AUTH;FAILED\r\n");
            DatabaseManager::getInstance()->logError(QString("Invalid login attempt for user: %1").arg(login));
        }
    }
    // 2. РЕГИСТРАЦИЯ
    else if (command == "REG") {
        if (tokens.size() < 3) return;
        QString login = tokens.at(1);
        QString rawPassword = tokens.at(2);

        std::string hashStr = hashPassword(rawPassword.toStdString());
        QString passwordHash = QString::fromStdString(hashStr);

        if (DatabaseManager::getInstance()->registerUser(login, passwordHash)) {
            socket->write("REG;SUCCESS\r\n");
            DatabaseManager::getInstance()->logUserAction(QVariant(), QString("Registered user: %1").arg(login));
        }
        else {
            socket->write("REG;FAILED\r\n");
            DatabaseManager::getInstance()->logError(QString("Failed to register user: %1").arg(login));
        }
    }
    // 3. ФУНКЦИИ АДМИНИСТРАТИВНОЙ ПАНЕЛИ
    else if (command == "ADMIN_GET_USERS") {
        socket->write(DatabaseManager::getInstance()->getUsers().toUtf8() + "\r\n");
    }
    else if (command == "ADMIN_GET_LOGS") {
        socket->write(DatabaseManager::getInstance()->getLogs().toUtf8() + "\r\n");
    }
    else if (command == "ADMIN_GET_ERRORS") {
        socket->write(DatabaseManager::getInstance()->getErrors().toUtf8() + "\r\n");
    }
    else if (command == "ADMIN_GET_HISTORY") {
        socket->write(DatabaseManager::getInstance()->getAlgorithmHistory().toUtf8() + "\r\n");
    }
    else if (command == "ADMIN_GET_TESTS") {
        socket->write(DatabaseManager::getInstance()->getTestResults().toUtf8() + "\r\n");
    }
    // 4. ВЫЧИСЛЕНИЯ (Пример интеграции сохранения результатов алгоритмов)
    else if (command == "CALC_GRAD") {
        if (tokens.size() < 5) return;
        QString func = tokens.at(1);
        QString start = tokens.at(2);
        QString lr = tokens.at(3);
        QString iter = tokens.at(4);

        // Имитируем вычисление
        double result = gradientDescent(func.toStdString(), start.toDouble(), lr.toDouble(), iter.toInt());
        socket->write(QString("RESULT;%1\r\n").arg(result).toUtf8());

        // Логируем действие пользователя и сохраняем в историю алгоритмов по ТЗ!
        DatabaseManager::getInstance()->logUserAction(1, "Gradient Descent");
        DatabaseManager::getInstance()->saveAlgorithmResult(1, "Gradient Descent", QString("Func:%1, Start:%2").arg(func, start), QString::number(result));
    }
    else if (command == "CALC_SPLINE") {
        double result = splineInterpolate({ 1,2 }, { 3,4 }, 2.5);
        socket->write(QString("RESULT;%1\r\n").arg(result).toUtf8());

        DatabaseManager::getInstance()->logUserAction(1, "Spline Calculation");
        DatabaseManager::getInstance()->saveAlgorithmResult(1, "Splines", "X:[1,2] Y:[3,4]", QString::number(result));
    }
    else {
        socket->write("ERROR;UNKNOWN_COMMAND\r\n");
    }
}

void MyTcpServer::handleGetMainData(QTcpSocket* socket) {
    socket->write("DATA;STUB_OBJECT_1;STUB_OBJECT_2\r\n");
}

void MyTcpServer::slotClientDisconnected() {
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        mClientSockets.removeOne(clientSocket);
        clientSocket->deleteLater();
    }
}