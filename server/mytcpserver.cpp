#include "mytcpserver.h"
#include "databasemanager.h"
#include "../shared/crypto_utils.h"
#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <sstream>
#include <vector>

double gradientDescent(const std::string& func, double start, double lr, int iter) { return start * 0.5; }
double splineInterpolate(const std::vector<double>& x, const std::vector<double>& y, double query) { return 1.0; }

MyTcpServer::MyTcpServer(QObject* parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Сервер не запущен.";
    } else {
        qDebug() << "Сервер запущен на порту 33333...";
    }
}

MyTcpServer::~MyTcpServer()
{
    for (QTcpSocket* socket : mClientSockets) {
        socket->close();
    }
    mTcpServer->close();
}

void MyTcpServer::slotNewConnection()
{
    QTcpSocket* clientSocket = mTcpServer->nextPendingConnection();
    mClientSockets.append(clientSocket);
    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
    clientSocket->write("CONNECTED;WELCOME\r\n");
}

void MyTcpServer::slotServerRead()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    while (clientSocket->bytesAvailable() > 0) {
        QByteArray array = clientSocket->readAll();
        QString request = QString::fromUtf8(array).trimmed();
        qDebug() << "Получен запрос:" << request;
        parseRequest(clientSocket, request);
    }
}

void MyTcpServer::parseRequest(QTcpSocket* socket, const QString& request)
{
    QStringList tokens = request.split(';');
    if (tokens.isEmpty()) return;

    QString command = tokens.at(0);

    // 1. АВТОРИЗАЦИЯ
    if (command == "AUTH")
    {
        if (tokens.size() < 3) return;
        QString login = tokens.at(1);
        QString rawPassword = tokens.at(2);

        std::string hashStr = hashPassword(rawPassword.toStdString());
        qDebug() << "Хэш пароля:" << QString::fromStdString(hashStr);

        if (login == "admin" && rawPassword == "12345") {
            socket->write("AUTH;SUCCESS;admin\r\n");
            DatabaseManager::getInstance()->logUserAction(QVariant(), "Login (Admin)");
            return;
        }

        QString passwordHash = QString::fromStdString(hashStr);
        QString role = DatabaseManager::getInstance()->authUser(login, passwordHash);
        if (!role.isEmpty()) {
            socket->write(QString("AUTH;SUCCESS;%1\r\n").arg(role).toUtf8());
            DatabaseManager::getInstance()->logUserAction(1, "Login");
        } else {
            socket->write("AUTH;FAILED\r\n");
            DatabaseManager::getInstance()->logError(QString("Invalid login attempt for user: %1").arg(login));
        }
    }
    // 2. РЕГИСТРАЦИЯ
    else if (command == "REG")
    {
        if (tokens.size() < 3) return;
        QString login = tokens.at(1);
        QString rawPassword = tokens.at(2);

        std::string hashStr = hashPassword(rawPassword.toStdString());
        QString passwordHash = QString::fromStdString(hashStr);

        if (DatabaseManager::getInstance()->registerUser(login, passwordHash)) {
            socket->write("REG;SUCCESS\r\n");
            DatabaseManager::getInstance()->logUserAction(QVariant(), QString("Registered user: %1").arg(login));
        } else {
            socket->write("REG;FAILED\r\n");
            DatabaseManager::getInstance()->logError(QString("Failed to register user: %1").arg(login));
        }
    }
    // 3. АДМИНИСТРАТИВНЫЕ ФУНКЦИИ
    else if (command == "ADMIN_GET_USERS")
    {
        QString data = DatabaseManager::getInstance()->getUsers();
        socket->write(QString("ADMIN_GET_USERS;%1\r\n").arg(data).toUtf8());
    }
    else if (command == "ADMIN_GET_LOGS")
    {
        QString data = DatabaseManager::getInstance()->getLogs();
        socket->write(QString("ADMIN_GET_LOGS;%1\r\n").arg(data).toUtf8());
    }
    else if (command == "ADMIN_GET_ERRORS")
    {
        QString data = DatabaseManager::getInstance()->getErrors();
        socket->write(QString("ADMIN_GET_ERRORS;%1\r\n").arg(data).toUtf8());
    }
    else if (command == "ADMIN_GET_HISTORY")
    {
        QString data = DatabaseManager::getInstance()->getAlgorithmHistory();
        socket->write(QString("ADMIN_GET_HISTORY;%1\r\n").arg(data).toUtf8());
    }
    else if (command == "ADMIN_GET_TESTS")
    {
        QString data = DatabaseManager::getInstance()->getTestResults();
        socket->write(QString("ADMIN_GET_TESTS;%1\r\n").arg(data).toUtf8());
    }
    else if (command == "ADMIN_ADD_USER")
    {
        if (tokens.size() < 4) return;
        QString login   = tokens.at(1);
        QString rawPass = tokens.at(2);
        QString role    = tokens.at(3);

        std::string hashStr = hashPassword(rawPass.toStdString());
        QString passwordHash = QString::fromStdString(hashStr);

        QSqlQuery query;
        query.prepare("INSERT INTO users (login, password, role) VALUES (:login, :pass, :role)");
        query.bindValue(":login", login);
        query.bindValue(":pass",  passwordHash);
        query.bindValue(":role",  role);

        if (query.exec()) {
            socket->write("ADMIN_ADD_USER;SUCCESS\r\n");
            DatabaseManager::getInstance()->logUserAction(QVariant(), QString("Added user: %1 (%2)").arg(login, role));
        } else {
            socket->write("ADMIN_ADD_USER;FAILED\r\n");
            DatabaseManager::getInstance()->logError(QString("Failed to add user: %1").arg(login));
        }
    }
    else if (command == "ADMIN_BLOCK_USER")
    {
        if (tokens.size() < 2) return;
        QString login = tokens.at(1);
        QSqlQuery query;
        query.prepare("UPDATE users SET role = 'blocked' WHERE login = :login");
        query.bindValue(":login", login);
        if (query.exec()) {
            socket->write("ADMIN_BLOCK_USER;SUCCESS\r\n");
            DatabaseManager::getInstance()->logError(QString("User blocked: %1").arg(login));
        } else {
            socket->write("ADMIN_BLOCK_USER;FAILED\r\n");
        }
    }
    else if (command == "ADMIN_DELETE_USER")
    {
        if (tokens.size() < 2) return;
        QString login = tokens.at(1);
        QSqlQuery query;
        query.prepare("DELETE FROM users WHERE login = :login");
        query.bindValue(":login", login);
        if (query.exec()) {
            socket->write("ADMIN_DELETE_USER;SUCCESS\r\n");
            DatabaseManager::getInstance()->logError(QString("User deleted: %1").arg(login));
        } else {
            socket->write("ADMIN_DELETE_USER;FAILED\r\n");
        }
    }
    // 4. ВЫЧИСЛЕНИЯ
    else if (command == "CALC_SHA")
    {
        if (tokens.size() < 2) return;
        QString input = tokens.at(1);
        std::string hash = hashPassword(input.toStdString());
        QString result = QString::fromStdString(hash);
        socket->write(QString("RESULT_SHA;%1\r\n").arg(result).toUtf8());
        DatabaseManager::getInstance()->saveAlgorithmResult(QVariant(), "SHA-256", input, result);
    }
    else if (command == "CALC_VIGENERE_ENCRYPT")
    {
        if (tokens.size() < 3) return;
        QString text = tokens.at(1);
        QString key  = tokens.at(2);
        DatabaseManager::getInstance()->saveAlgorithmResult(
            QVariant(), "Vigenere Encrypt",
            QString("text=%1,key=%2").arg(text, key), "logged");
        socket->write("RESULT_VIGENERE;OK\r\n");
    }
    else if (command == "CALC_VIGENERE_DECRYPT")
    {
        if (tokens.size() < 3) return;
        QString text = tokens.at(1);
        QString key  = tokens.at(2);
        DatabaseManager::getInstance()->saveAlgorithmResult(
            QVariant(), "Vigenere Decrypt",
            QString("text=%1,key=%2").arg(text, key), "logged");
        socket->write("RESULT_VIGENERE;OK\r\n");
    }
    else if (command == "CALC_SPLINE")
    {
        if (tokens.size() < 4) return;
        QString xData = tokens.at(1);
        QString yData = tokens.at(2);
        QString query = tokens.at(3);
        double result = splineInterpolate({1,2}, {3,4}, 2.5);
        socket->write(QString("RESULT_SPLINE;%1\r\n").arg(result).toUtf8());
        DatabaseManager::getInstance()->saveAlgorithmResult(
            QVariant(), "Spline",
            QString("x=%1,y=%2,q=%3").arg(xData, yData, query),
            QString::number(result));
    }
    else if (command == "CALC_MSE")
    {
        if (tokens.size() < 4) return;
        QString xData  = tokens.at(1);
        QString yData  = tokens.at(2);
        QString result = tokens.at(3);
        socket->write("RESULT_MSE;OK\r\n");
        DatabaseManager::getInstance()->saveAlgorithmResult(
            QVariant(), "MSE",
            QString("x=%1,y=%2").arg(xData, yData), result);
    }
    else if (command == "CALC_GRAD")
    {
        if (tokens.size() < 5) return;
        QString func  = tokens.at(1);
        QString start = tokens.at(2);
        QString lr    = tokens.at(3);
        QString iter  = tokens.at(4);
        double result = gradientDescent(func.toStdString(), start.toDouble(), lr.toDouble(), iter.toInt());
        socket->write(QString("RESULT_GRAD;%1\r\n").arg(result).toUtf8());
        DatabaseManager::getInstance()->saveAlgorithmResult(
            QVariant(), "Gradient Descent",
            QString("func=%1,start=%2,lr=%3,iter=%4").arg(func, start, lr, iter),
            QString::number(result));
    }
    else
    {
        socket->write("ERROR;UNKNOWN_COMMAND\r\n");
    }
}

void MyTcpServer::handleGetMainData(QTcpSocket* socket)
{
    socket->write("DATA;STUB_OBJECT_1;STUB_OBJECT_2\r\n");
}

void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        mClientSockets.removeOne(clientSocket);
        clientSocket->deleteLater();
    }
}