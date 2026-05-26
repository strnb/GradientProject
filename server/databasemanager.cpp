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
    
    // ВАЖНО: Указываем имя контейнера из docker-compose вместо localhost
    db.setHostName("postgres"); 
    db.setDatabaseName("gradient_db"); 
    db.setUserName("admin");
    db.setPassword("admin");
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
    // Соответствие твоей структуре init.sql: login, password, role
    query.prepare("INSERT INTO users (login, password, role) VALUES (:login, :pass, 'user')");
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
    // Соответствие твоей структуре init.sql: выбор поля role
    query.prepare("SELECT role FROM users WHERE login = :login AND password = :pass");
    query.bindValue(":login", login);
    query.bindValue(":pass", passwordHash);
    
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";
}
