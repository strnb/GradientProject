/**
 * @file Adminwindow.h
 * @brief Заголовочный файл окна администратора
 */

#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

 /**
  * @class AdminWindow
  * @brief Главное окно панели администратора
  *
  * Окно предоставляет администратору возможности управления пользователями,
  * просмотра истории запросов и логов системы.
  */
class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор окна администратора
     * @param parent Родительский виджет (по умолчанию nullptr)
     */
    explicit AdminWindow(QWidget* parent = nullptr);

    /// Деструктор по умолчанию
    ~AdminWindow() override = default;

private slots:
    /**
     * @brief Обработчик ответа от сервера
     * @param response Ответ сервера в виде строки
     */
    void onServerResponse(const QString& response);

    /// Обновить список пользователей
    void onRefreshUsers();

    /// Обновить историю запросов
    void onRefreshHistory();

    /// Обновить логи системы
    void onRefreshLogs();

    /// Заблокировать выбранного пользователя
    void onBlockUser();

    /// Удалить выбранного пользователя
    void onDeleteUser();

private:
    // === Виджеты интерфейса ===

    /// Виджет с вкладками
    QTabWidget* m_tabs;

    // Вкладка "Пользователи"
    QTableWidget* m_usersTable;      ///< Таблица пользователей
    QPushButton* m_blockButton;     ///< Кнопка блокировки пользователя
    QPushButton* m_deleteButton;    ///< Кнопка удаления пользователя
    QPushButton* m_refreshUsersButton; ///< Кнопка обновления списка пользователей

    // Вкладка "История запросов"
    QTableWidget* m_historyTable;    ///< Таблица истории запросов
    QPushButton* m_refreshHistoryButton; ///< Кнопка обновления истории

    // Вкладка "Логи"
    QTableWidget* m_logsTable;       ///< Таблица логов
    QPushButton* m_refreshLogsButton; ///< Кнопка обновления логов

    /// Текущая ожидаемая команда для правильной обработки ответа
    QString m_pendingCommand;

    /**
     * @brief Настройка пользовательского интерфейса
     */
    void setupUi();

    /**
     * @brief Заполнить таблицу данными в формате CSV
     * @param table Указатель на таблицу для заполнения
     * @param csvData Данные в формате CSV
     */
    void fillTable(QTableWidget* table, const QString& csvData);
};

#endif // ADMINWINDOW_H