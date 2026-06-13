/**
 * @file Networkmanager.h
 * @brief Заголовочный файл менеджера сетевых подключений
 */

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

 /**
  * @class NetworkManager
  * @brief Синглтон для управления TCP-соединением с сервером
  *
  * Обеспечивает единственное TCP-соединение на весь клиент.
  * Все окна отправляют запросы через NetworkManager::getInstance()->sendRequest(...)
  * и подписываются на сигнал responseReceived(QString).
  */
class NetworkManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Получить экземпляр синглтона
     * @return Указатель на единственный экземпляр NetworkManager
     */
    static NetworkManager* getInstance();

    /**
     * @brief Подключиться к серверу
     * @param host Адрес сервера (по умолчанию "127.0.0.1")
     * @param port Порт сервера (по умолчанию 33333)
     */
    void connectToServer(const QString& host = "127.0.0.1", quint16 port = 33333);

    /**
     * @brief Отправить запрос на сервер
     * @param request Строка запроса
     */
    void sendRequest(const QString& request);

    /**
     * @brief Получить роль текущего пользователя
     * @return Роль пользователя ("admin" / "user" / "" если не вошли)
     */
    QString role() const;

    /**
     * @brief Установить роль текущего пользователя
     * @param role Роль пользователя
     */
    void setRole(const QString& role);

    /**
     * @brief Получить логин текущего пользователя
     * @return Логин пользователя
     */
    QString login() const;

    /**
     * @brief Установить логин текущего пользователя
     * @param login Логин пользователя
     */
    void setLogin(const QString& login);

    /**
     * @brief Проверить состояние подключения
     * @return true - подключение установлено, false - отключен
     */
    bool isConnected() const;

signals:
    /**
     * @brief Сигнал о получении ответа от сервера
     * @param response Ответ сервера в виде строки
     */
    void responseReceived(const QString& response);

    /// Сигнал об успешном установлении соединения
    void connected();

    /// Сигнал о разрыве соединения
    void disconnected();

private slots:
    /// Обработчик готовности данных для чтения
    void onReadyRead();

    /// Обработчик успешного подключения
    void onConnected();

    /// Обработчик отключения
    void onDisconnected();

    /**
     * @brief Обработчик ошибки сокета
     * @param error Тип ошибки
     */
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    /**
     * @brief Приватный конструктор (синглтон)
     * @param parent Родительский объект (по умолчанию nullptr)
     */
    explicit NetworkManager(QObject* parent = nullptr);

    /// Деструктор по умолчанию
    ~NetworkManager() override = default;

    // Запрет копирования (синглтон)
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    static NetworkManager* p_instance; ///< Указатель на единственный экземпляр

    QTcpSocket* m_socket;   ///< TCP-сокет для соединения с сервером
    QString     m_role;     ///< Роль текущего пользователя
    QString     m_login;    ///< Логин текущего пользователя
    QString     m_buffer;   ///< Буфер для склейки частичных пакетов
};

#endif // NETWORKMANAGER_H