#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QString>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

public slots:
    void slotNewConnection();
    void slotClientDisconnected();
    void slotServerRead();

private:
    QTcpServer *mTcpServer;
    QList<QTcpSocket*> mClientSockets; // Поддержка нескольких клиентов [Пункт плана выполнен]
    
    void parseRequest(QTcpSocket* socket, const QString &request);
    
    // Заглушки под остальной функционал [Пункт плана выполнен]
    void handleAdminFunction(QTcpSocket* socket);
    void handleGetMainData(QTcpSocket* socket);
};

#endif // MYTCPSERVER_H
