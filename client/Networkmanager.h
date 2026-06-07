#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

// Синглтон — единственное TCP-соединение на весь клиент.
// Все окна отправляют запросы через NetworkManager::getInstance()->sendRequest(...)
// и подписываются на сигнал responseReceived(QString).

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    static NetworkManager* getInstance();

    // Подключиться к серверу (вызывается один раз в main.cpp)
    void connectToServer(const QString& host = "127.0.0.1", quint16 port = 33333);

    // Отправить запрос на сервер
    void sendRequest(const QString& request);

    // Роль текущего пользователя ("admin" / "user" / "" если не вошли)
    QString role() const;
    void setRole(const QString& role);

    // Логин текущего пользователя
    QString login() const;
    void setLogin(const QString& login);

    bool isConnected() const;

    signals:
        // Приходит каждый раз когда сервер прислал ответ
        void responseReceived(const QString& response);

    // Соединение установлено / разорвано
    void connected();
    void disconnected();

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    explicit NetworkManager(QObject* parent = nullptr);
    ~NetworkManager() override = default;

    // Запрет копирования (синглтон)
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    static NetworkManager* p_instance;

    QTcpSocket* m_socket;
    QString     m_role;
    QString     m_login;
    QString     m_buffer; // буфер для склейки частичных пакетов
};

#endif // NETWORKMANAGER_H
