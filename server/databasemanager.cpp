#include "databasemanager.h"
#include <QDebug>

DatabaseManager* DatabaseManager::p_instance = nullptr;
DatabaseDestroyer DatabaseManager::destroyer;

DatabaseDestroyer::~DatabaseDestroyer() {
    delete p_instance;
}
void DatabaseDestroyer::initialize(DatabaseManager *p) {
    p_instance = p;
}

DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("gradient_db");
    db.setUserName("admin");
    db.setPassword("admin");
    db.setPort(5432);
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) db.close();
}

DatabaseManager* DatabaseManager::getInstance() {
    if (!p_instance) {
        p_instance = new DatabaseManager();
        destroyer.initialize(p_instance);
    }
    return p_instance;
}

bool DatabaseManager::connectToDatabase() {
    if (!db.open()) {
        qDebug() << "Ошибка подключения к БД:" << db.lastError().text();
        return false;
    }
    qDebug() << "Успешное подключение к PostgreSQL!";
    return true;
}

bool DatabaseManager::registerUser(const QString& login, const QString& passwordHash) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (login, password, role) VALUES (:login, :pass, 'user')");
    query.bindValue(":login", login);
    query.bindValue(":pass", passwordHash);
    if (!query.exec()) {
        qDebug() << "Ошибка регистрации:" << query.lastError().text();
        return false;
    }
    return true;
}

QString DatabaseManager::authUser(const QString& login, const QString& passwordHash) {
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE login = :login AND password = :pass");
    query.bindValue(":login", login);
    query.bindValue(":pass", passwordHash);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return QString();
}

bool DatabaseManager::logUserAction(const QVariant& userId, const QString& action) {
    QSqlQuery query;
    query.prepare("INSERT INTO user_logs (user_id, action) VALUES (:user_id, :action)");
    query.bindValue(":user_id", userId);
    query.bindValue(":action", action);
    return query.exec();
}

bool DatabaseManager::logError(const QString& message) {
    QSqlQuery query;
    query.prepare("INSERT INTO error_logs (error_message) VALUES (:msg)");
    query.bindValue(":msg", message);
    return query.exec();
}

bool DatabaseManager::saveAlgorithmResult(const QVariant& userId, const QString& algorithmName,
                                           const QString& inputData, const QString& resultData) {
    QSqlQuery query;
    query.prepare("INSERT INTO algorithm_history (user_id, algorithm_name, input_data, result_data) "
                  "VALUES (:user_id, :algo, :input, :result)");
    query.bindValue(":user_id", userId);
    query.bindValue(":algo", algorithmName);
    query.bindValue(":input", inputData);
    query.bindValue(":result", resultData);
    return query.exec();
}

bool DatabaseManager::saveTestResult(const QString& testName, const QString& status, double executionTime) {
    QSqlQuery query;
    query.prepare("INSERT INTO test_results (test_name, status, execution_time) VALUES (:name, :status, :time)");
    query.bindValue(":name", testName);
    query.bindValue(":status", status);
    query.bindValue(":time", executionTime);
    return query.exec();
}

bool DatabaseManager::logAction(const QString& login, const QString& actionType, const QString& description) {
    QSqlQuery query;
    QVariant userId = QVariant(QMetaType(QMetaType::Int));
    if (login != "admin") {
        query.prepare("SELECT id FROM users WHERE login = :login");
        query.bindValue(":login", login);
        if (query.exec() && query.next()) {
            userId = query.value(0).toInt();
        }
    }
    query.prepare("INSERT INTO user_logs (user_id, action) VALUES (:user_id, :action)");
    query.bindValue(":user_id", userId);
    query.bindValue(":action", QString("%1: %2").arg(actionType, description));
    if (!query.exec()) {
        qDebug() << "Ошибка записи лога:" << query.lastError().text();
        return false;
    }
    return true;
}

QString DatabaseManager::getLogsForAdmin() {
    return getLogs();
}

// Формат: "id,login,role,created_at|id,login,role,created_at|..."
QString DatabaseManager::getUsers() {
    QSqlQuery query("SELECT id, login, role, created_at FROM users ORDER BY id");
    QStringList rows;
    while (query.next()) {
        rows << QString("%1,%2,%3,%4")
                .arg(query.value(0).toString(),
                     query.value(1).toString(),
                     query.value(2).toString(),
                     query.value(3).toString());
    }
    return rows.isEmpty() ? "EMPTY" : rows.join("|");
}

// Формат: "id,login,action,created_at|..."
QString DatabaseManager::getLogs() {
    QSqlQuery query(
        "SELECT l.id, COALESCE(u.login, 'admin') as name, l.action, l.created_at "
        "FROM user_logs l LEFT JOIN users u ON l.user_id = u.id "
        "ORDER BY l.created_at DESC LIMIT 100"
    );
    QStringList rows;
    while (query.next()) {
        rows << QString("%1,%2,%3,%4")
                .arg(query.value(0).toString(),
                     query.value(1).toString(),
                     query.value(2).toString(),
                     query.value(3).toString());
    }
    return rows.isEmpty() ? "EMPTY" : rows.join("|");
}

// Формат: "id,message,created_at|..."
QString DatabaseManager::getErrors() {
    QSqlQuery query("SELECT id, error_message, created_at FROM error_logs ORDER BY created_at DESC LIMIT 100");
    QStringList rows;
    while (query.next()) {
        rows << QString("%1,%2,%3")
                .arg(query.value(0).toString(),
                     query.value(1).toString(),
                     query.value(2).toString());
    }
    return rows.isEmpty() ? "EMPTY" : rows.join("|");
}

// Формат: "id,login,algorithm,input,result|..."
QString DatabaseManager::getAlgorithmHistory() {
    QSqlQuery query(
        "SELECT h.id, COALESCE(u.login, 'guest') as name, h.algorithm_name, h.input_data, h.result_data "
        "FROM algorithm_history h LEFT JOIN users u ON h.user_id = u.id "
        "ORDER BY h.created_at DESC LIMIT 50"
    );
    QStringList rows;
    while (query.next()) {
        rows << QString("%1,%2,%3,%4,%5")
                .arg(query.value(0).toString(),
                     query.value(1).toString(),
                     query.value(2).toString(),
                     query.value(3).toString(),
                     query.value(4).toString());
    }
    return rows.isEmpty() ? "EMPTY" : rows.join("|");
}

// Формат: "id,test_name,status,execution_time|..."
QString DatabaseManager::getTestResults() {
    QSqlQuery query("SELECT id, test_name, status, execution_time FROM test_results ORDER BY created_at DESC LIMIT 50");
    QStringList rows;
    while (query.next()) {
        rows << QString("%1,%2,%3,%4")
                .arg(query.value(0).toString(),
                     query.value(1).toString(),
                     query.value(2).toString(),
                     query.value(3).toString());
    }
    return rows.isEmpty() ? "EMPTY" : rows.join("|");
}