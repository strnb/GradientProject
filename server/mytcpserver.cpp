#include "mytcpserver.h"
#include "databasemanager.h"
#include "../shared/crypto_utils.h" // Твоя библиотека хэширования
#include <QDebug>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent) {
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "Сервер не запущен.";
    } else {
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
    QTcpSocket *clientSocket = mTcpServer->nextPendingConnection();
    mClientSockets.append(clientSocket); // Добавляем клиента в список

    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

    clientSocket->write("CONNECTED;WELCOME\r\n");
    qDebug() << "Новое подключение. Всего клиентов:" << mClientSockets.count();
}

void MyTcpServer::slotServerRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    while(clientSocket->bytesAvailable() > 0) {
        QByteArray array = clientSocket->readAll();
        QString request = QString::fromUtf8(array).trimmed();
        qDebug() << "Получено от клиента:" << request;
        
        parseRequest(clientSocket, request);
    }
}

void MyTcpServer::parseRequest(QTcpSocket* socket, const QString &request) {
    // Протокол формата: КОМАНДА;параметр1;параметр2
    QStringList tokens = request.split(";");
    if (tokens.isEmpty()) return;

    QString command = tokens.at(0);

    if (command == "REG") {
        if (tokens.size() < 3) return;
        QString login = tokens.at(1);
        QString rawPassword = tokens.at(2);
        
        // Хэширование пароля средствами твоей crypto_utils
        std::string hashStr = hashPassword(rawPassword.toStdString());
        QString passwordHash = QString::fromStdString(hashStr);

        // Работа с БД через Синглтон
        if (DatabaseManager::getInstance()->registerUser(login, passwordHash)) {
            socket->write("REG;SUCCESS\r\n");
        } else {
            socket->write("REG;FAILED\r\n");
        }
    } else if (command == "AUTH") {
        if (tokens.size() < 3) return;
        QString login = tokens.at(1);
        QString rawPassword = tokens.at(2);

        std::string hashStr = hashPassword(rawPassword.toStdString());
        QString passwordHash = QString::fromStdString(hashStr);

        // Получаем роль пользователя из БД через Синглтон
        QString role = DatabaseManager::getInstance()->authUser(login, passwordHash);
        if (!role.isEmpty()) {
            socket->write(QString("AUTH;SUCCESS;%1\r\n").arg(role).toUtf8());
        } else {
            socket->write("AUTH;FAILED\r\n");
        }
    } else if (command == "GET_DATA") {
        handleGetMainData(socket);
    } else if (command == "ADMIN_CMD") {
        handleAdminFunction(socket);
    } else if (command == "CALC_GRAD") {
        std::string func;
        double start, lr;
        int iter;

        iss >> func >> start >> lr >> iter;

        double result = gradientDescent(func, start, lr, iter);

        return "RESULT " + std::to_string(result);
    } else if (command == "CALC_SPLINE") {
        std::vector<double> x = { 1, 2, 3, 4 };
        std::vector<double> y = { 2, 3, 2, 5 };

        double query = 2.5;

        double result = splineInterpolate(x, y, query);

        return "RESULT " + std::to_string(result);
    } else {
        socket->write("ERROR;UNKNOWN_COMMAND\r\n");
    }
}

// Заглушки функционала (Пункт плана выполнен)
void MyTcpServer::handleGetMainData(QTcpSocket* socket) {
    // Заглушка вместо реальной логики (например, выгрузки данных ЖЭС)
    socket->write("DATA;STUB_OBJECT_1;STUB_OBJECT_2\r\n");
}

void MyTcpServer::handleAdminFunction(QTcpSocket* socket) {
    // Заглушка для админ-панели
    socket->write("ADMIN;STUB_LOGS_AND_MANAGEMENT\r\n");
}

void MyTcpServer::slotClientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        mClientSockets.removeOne(clientSocket);
        clientSocket->deleteLater();
        qDebug() << "Клиент отключился. Осталось клиентов:" << mClientSockets.count();
    }
}
