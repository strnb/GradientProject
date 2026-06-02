#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>

class DatabaseManager;

class DatabaseDestroyer {
private:
    DatabaseManager* p_instance;
public:
    ~DatabaseDestroyer();
    void initialize(DatabaseManager* p);
};

class DatabaseManager {
private:
    static DatabaseManager* p_instance;
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
    bool registerUser(const QString& login, const QString& passwordHash);
    QString authUser(const QString& login, const QString& passwordHash);

    // ВСЕ МЕТОДЫ ИЗ ТЗ (СОХРАНЕНИЕ ДАННЫХ)
    bool logUserAction(const QVariant& userId, const QString& action);
    bool logError(const QString& message);
    bool saveAlgorithmResult(const QVariant& userId, const QString& algorithmName, const QString& inputData, const QString& resultData);
    bool saveTestResult(const QString& testName, const QString& status, double executionTime);

    // ВСЕ МЕТОДЫ ИЗ ТЗ (ПОЛУЧЕНИЕ ДАННЫХ ДЛЯ АДМИНКИ)
    QString getUsers();
    QString getLogs();
    QString getErrors();
    QString getAlgorithmHistory();
    QString getTestResults();
};

#endif // DATABASEMANAGER_H