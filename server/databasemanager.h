#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

class DatabaseManager;

class DatabaseDestroyer {
private:
    DatabaseManager *p_instance;
public:
    ~DatabaseDestroyer();
    void initialize(DatabaseManager *p);
};

class DatabaseManager {
private:
    static DatabaseManager *p_instance;
    static DatabaseDestroyer destroyer;
    QSqlDatabase db;

    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    friend class DatabaseDestroyer;

public:
    static DatabaseManager* getInstance();
    
    bool connectToDatabase();
    bool registerUser(const QString &login, const QString &passwordHash);
    QString authUser(const QString &login, const QString &passwordHash); // Возвращает роль или пустую строку
};

#endif // DATABASEMANAGER_H
