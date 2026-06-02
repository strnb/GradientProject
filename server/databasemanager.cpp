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

bool DatabaseManager::registerUser(const QString& login, const QString& passwordHash) {
    QSqlQuery query;
    // ИСПРАВЛЕНО: 'password' вместо 'password_hash' в соответствии с init.sql
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
    // ИСПРАВЛЕНО: 'password' вместо 'password_hash' в соответствии с init.sql
    query.prepare("SELECT role FROM users WHERE login = :login AND password = :pass");
    query.bindValue(":login", login);
    query.bindValue(":pass", passwordHash);

    if (query.exec() && query.next()) {
        return query.value(0).toString(); // Возвращает роль ("user", "admin" и т.д.)
    }
    return QString(); // Возвращает пустую строку, если пользователь не найден
}

bool DatabaseManager::logAction(const QString& login, const QString& actionType, const QString& description) {
    QSqlQuery query;
    QVariant userId = QVariant(QVariant::Int); // По умолчанию NULL, если это admin или неверный логин

    // Ищем ID пользователя, если это не хардкод-админ
    if (login != "admin") {
        query.prepare("SELECT id FROM users WHERE login = :login");
        query.bindValue(":login", login);
        if (query.exec() && query.next()) {
            userId = query.value(0).toInt();
        }
    }

    // Записываем лог
    query.prepare("INSERT INTO user_actions (user_id, action_type, description) "
        "VALUES (:user_id, :action_type, :description)");
    query.bindValue(":user_id", userId);
    query.bindValue(":action_type", actionType);
    query.bindValue(":description", description);

    if (!query.exec()) {
        qDebug() << "Ошибка записи лога:" << query.lastError().text();
        return false;
    }
    return true;
}

QString DatabaseManager::getLogsForAdmin() {
    QSqlQuery query;
    // Связываем таблицы, чтобы вытащить текстовый логин. Если user_id NULL и логин не найден, 
    // проверяем: если это было действие админа — пишем 'admin', иначе 'system/unknown'
    query.prepare("SELECT COALESCE(u.login, 'admin') as username, a.action_type, a.description, a.created_at "
        "FROM user_actions a "
        "LEFT JOIN users u ON a.user_id = u.id "
        "ORDER BY a.created_at DESC LIMIT 100");

    QString logReport = "LOGS_LIST";
    if (query.exec()) {
        while (query.next()) {
            QString user = query.value("username").toString();
            QString type = query.value("action_type").toString();
            QString desc = query.value("description").toString();
            QString time = query.value("created_at").toString(); // Можно форматировать под QDateTime

            logReport += QString("\n[%1] [%2] Позов.: %3 | %4").arg(time, type, user, desc);
        }
    }
    else {
        logReport += ";ERROR_FETCHING_LOGS";
    }
    return logReport;
}

// 1. Сохранение действия пользователя (userId передаем как QVariant, чтобы легко писать NULL для админа)
bool DatabaseManager::logUserAction(const QVariant& userId, const QString& action) {
    QSqlQuery query;
    query.prepare("INSERT INTO user_logs (user_id, action) VALUES (:user_id, :action)");
    query.bindValue(":user_id", userId);
    query.bindValue(":action", action);
    return query.exec();
}

// 2. Сохранение ошибки системы
bool DatabaseManager::logError(const QString& message) {
    QSqlQuery query;
    query.prepare("INSERT INTO error_logs (error_message) VALUES (:msg)");
    query.bindValue(":msg", message);
    return query.exec();
}

// 3. Сохранение истории вычислений алгоритмов
bool DatabaseManager::saveAlgorithmResult(const QVariant& userId, const QString& algorithmName, const QString& inputData, const QString& resultData) {
    QSqlQuery query;
    query.prepare("INSERT INTO algorithm_history (user_id, algorithm_name, input_data, result_data) "
        "VALUES (:user_id, :algo, :input, :result)");
    query.bindValue(":user_id", userId);
    query.bindValue(":algo", algorithmName);
    query.bindValue(":input", inputData);
    query.bindValue(":result", resultData);
    return query.exec();
}

// 4. Сохранение результатов тестирования
bool DatabaseManager::saveTestResult(const QString& testName, const QString& status, double executionTime) {
    QSqlQuery query;
    query.prepare("INSERT INTO test_results (test_name, status, execution_time) VALUES (:name, :status, :time)");
    query.bindValue(":name", testName);
    query.bindValue(":status", status);
    query.bindValue(":time", executionTime);
    return query.exec();
}

// 5. Получение списка пользователей
QString DatabaseManager::getUsers() {
    QSqlQuery query("SELECT id, login, role FROM users ORDER BY id");
    QString res = "USERS_LIST";
    while (query.next()) {
        res += QString("\nID: %1 | Login: %2 | Role: %3")
            .arg(query.value(0).toString(), query.value(1).toString(), query.value(2).toString());
    }
    return res;
}

// 6. Получение журнала действий пользователей (с INNER JOIN для вывода красивого логина)
QString DatabaseManager::getLogs() {
    QSqlQuery query("SELECT l.id, COALESCE(u.login, 'admin') as name, l.action, l.created_at "
        "FROM user_logs l LEFT JOIN users u ON l.user_id = u.id ORDER BY l.created_at DESC LIMIT 100");
    QString res = "LOGS_LIST";
    while (query.next()) {
        res += QString("\n[%1] User: %2 -> Action: %3")
            .arg(query.value(3).toString(), query.value(1).toString(), query.value(2).toString());
    }
    return res;
}

// 7. Получение списка системных ошибок
QString DatabaseManager::getErrors() {
    QSqlQuery query("SELECT id, error_message, created_at FROM error_logs ORDER BY created_at DESC LIMIT 100");
    QString res = "ERRORS_LIST";
    while (query.next()) {
        res += QString("\n[%1] ERROR: %2").arg(query.value(2).toString(), query.value(1).toString());
    }
    return res;
}

// 8. Получение истории работы алгоритмов
QString DatabaseManager::getAlgorithmHistory() {
    QSqlQuery query("SELECT h.id, COALESCE(u.login, 'guest') as name, h.algorithm_name, h.input_data, h.result_data, h.created_at "
        "FROM algorithm_history h LEFT JOIN users u ON h.user_id = u.id ORDER BY h.created_at DESC LIMIT 50");
    QString res = "ALGO_HISTORY";
    while (query.next()) {
        res += QString("\n[%1] User: %2 | Algo: %3 | Input: %4 | Result: %5")
            .arg(query.value(5).toString(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString(), query.value(4).toString());
    }
    return res;
}

// 9. Получение результатов тестирования
QString DatabaseManager::getTestResults() {
    QSqlQuery query("SELECT id, test_name, status, execution_time, created_at FROM test_results ORDER BY created_at DESC LIMIT 50");
    QString res = "TEST_RESULTS";
    while (query.next()) {
        res += QString("\n[%1] Test: %2 | Status: %3 | Time: %4 ms")
            .arg(query.value(4).toString(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString());
    }
    return res;
}