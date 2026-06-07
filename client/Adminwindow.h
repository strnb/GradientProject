#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget* parent = nullptr);
    ~AdminWindow() override = default;

private slots:
    void onServerResponse(const QString& response);
    void onRefreshUsers();
    void onRefreshHistory();
    void onRefreshLogs();
    void onBlockUser();
    void onDeleteUser();

private:
    // Вкладки
    QTabWidget*   m_tabs;

    // Вкладка "Пользователи"
    QTableWidget* m_usersTable;
    QPushButton*  m_blockButton;
    QPushButton*  m_deleteButton;
    QPushButton*  m_refreshUsersButton;

    // Вкладка "История запросов"
    QTableWidget* m_historyTable;
    QPushButton*  m_refreshHistoryButton;

    // Вкладка "Логи"
    QTableWidget* m_logsTable;
    QPushButton*  m_refreshLogsButton;

    // Текущая ожидаемая команда (чтобы правильно обработать ответ)
    QString m_pendingCommand;

    void setupUi();
    void fillTable(QTableWidget* table, const QString& csvData);
};

#endif // ADMINWINDOW_H