#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>

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
    void onAddUser();
    void onAddAdmin();
    void onChangePassword();  // новое
    void onUpgradeToAdmin();  // новое

private:
    QTabWidget*   m_tabs;

    // Вкладка "Пользователи"
    QTableWidget* m_usersTable;
    QPushButton*  m_blockButton;
    QPushButton*  m_deleteButton;
    QPushButton*  m_refreshUsersButton;
    QPushButton*  m_addUserButton;
    QPushButton*  m_addAdminButton;
    QPushButton*  m_changePasswordButton;  // новое
    QPushButton*  m_upgradeToAdminButton;  // новое

    // Вкладка "История запросов"
    QTableWidget* m_historyTable;
    QPushButton*  m_refreshHistoryButton;

    // Вкладка "Логи"
    QTableWidget* m_logsTable;
    QPushButton*  m_refreshLogsButton;

    QString m_pendingCommand;

    void setupUi();
    void fillTable(QTableWidget* table, const QString& csvData);
    void showAddUserDialog(const QString& role);
};

#endif // ADMINWINDOW_H