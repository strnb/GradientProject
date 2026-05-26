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
    db.setDatabaseName("your_db_name"); // Имя твоей БД (например, zhes_db)
    db.setUserName("postgres");
    db.setPassword("your_password");
    db.setPort(5432);
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();
    }
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

bool DatabaseManager::registerUser(const QString &login, const QString &passwordHash) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (login, password_hash, role) VALUES (:login, :pass, 'user')");
    query.bindValue(":login", login);
    query.bindValue(":pass", passwordHash);
    
    if (!query.exec()) {
        qDebug() << "Ошибка регистрации:" << query.lastError().text();
        return false;
    }
    return true;
}

QString DatabaseManager::authUser(const QString &login, const QString &passwordHash) {
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE login = :login AND password_hash = :pass");
    query.bindValue(":login", login);
    query.bindValue(":pass", passwordHash);
    
    if (query.exec() && query.next()) {
        return query.value(0).toString(); // Возвращает роль ('user' или 'admin')
    }
    return ""; // Авторизация провалена
}
