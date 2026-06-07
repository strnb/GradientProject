#include "networkmanager.h"

#include <QDebug>

// Инициализация статического поля
NetworkManager* NetworkManager::p_instance = nullptr;

NetworkManager* NetworkManager::getInstance()
{
    if (!p_instance)
    {
        p_instance = new NetworkManager();
    }
    return p_instance;
}

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::readyRead,
            this,     &NetworkManager::onReadyRead);

    connect(m_socket, &QTcpSocket::connected,
            this,     &NetworkManager::onConnected);

    connect(m_socket, &QTcpSocket::disconnected,
            this,     &NetworkManager::onDisconnected);

    connect(m_socket, &QAbstractSocket::errorOccurred,
            this,     &NetworkManager::onErrorOccurred);
}

void NetworkManager::connectToServer(const QString& host, quint16 port)
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
        return;

    qDebug() << "NetworkManager: подключаемся к" << host << ":" << port;
    m_socket->connectToHost(host, port);
}

void NetworkManager::sendRequest(const QString& request)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "NetworkManager: нет соединения, запрос не отправлен:" << request;
        return;
    }

    // Сервер читает строки, заканчивающиеся на \r\n
    QString message = request + "\r\n";
    m_socket->write(message.toUtf8());
    m_socket->flush();

    qDebug() << "NetworkManager: отправлено ->" << request;
}

// Сервер может прислать несколько строк в одном пакете
// или одну строку в нескольких пакетах — буферизуем и режем по \n
void NetworkManager::onReadyRead()
{
    m_buffer += QString::fromUtf8(m_socket->readAll());

    while (m_buffer.contains('\n'))
    {
        int idx = m_buffer.indexOf('\n');
        QString line = m_buffer.left(idx).trimmed(); // убираем \r\n
        m_buffer = m_buffer.mid(idx + 1);

        if (!line.isEmpty())
        {
            qDebug() << "NetworkManager: получено <-" << line;
            emit responseReceived(line);
        }
    }
}

void NetworkManager::onConnected()
{
    qDebug() << "NetworkManager: соединение установлено";
    emit connected();
}

void NetworkManager::onDisconnected()
{
    qDebug() << "NetworkManager: соединение разорвано";
    m_role.clear();
    m_login.clear();
    emit disconnected();
}

void NetworkManager::onErrorOccurred(QAbstractSocket::SocketError error)
{
    qWarning() << "NetworkManager: ошибка сокета:" << m_socket->errorString() << "(" << error << ")";
}

QString NetworkManager::role() const
{
    return m_role;
}

void NetworkManager::setRole(const QString& role)
{
    m_role = role;
}

QString NetworkManager::login() const
{
    return m_login;
}

void NetworkManager::setLogin(const QString& login)
{
    m_login = login;
}

bool NetworkManager::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}
