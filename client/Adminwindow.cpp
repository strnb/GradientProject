#include "adminwindow.h"
#include "networkmanager.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDebug>

AdminWindow::AdminWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Панель администратора");
    setMinimumSize(900, 550);

    setupUi();

    connect(
        NetworkManager::getInstance(),
        &NetworkManager::responseReceived,
        this,
        &AdminWindow::onServerResponse
    );

    onRefreshUsers();
}

void AdminWindow::setupUi()
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    QLabel* title = new QLabel("Панель администратора", this);
    QFont f = title->font();
    f.setPointSize(14);
    f.setBold(true);
    title->setFont(f);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    m_tabs = new QTabWidget(this);
    mainLayout->addWidget(m_tabs);

    // ===== Вкладка "Пользователи" =====
    QWidget* usersTab = new QWidget();
    QVBoxLayout* usersLayout = new QVBoxLayout(usersTab);

    m_usersTable = new QTableWidget(0, 3, this);
    m_usersTable->setHorizontalHeaderLabels({"ID", "Логин", "Роль"});
    m_usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    usersLayout->addWidget(m_usersTable);

    // Первая строка кнопок — добавление
    QHBoxLayout* addButtons = new QHBoxLayout();
    m_refreshUsersButton = new QPushButton("Обновить", this);
    m_addUserButton      = new QPushButton("Добавить пользователя", this);
    m_addAdminButton     = new QPushButton("Добавить администратора", this);

    m_addUserButton->setStyleSheet(
        "QPushButton { background-color: #27ae60; color: white; }"
        "QPushButton:hover { background-color: #2ecc71; }");
    m_addAdminButton->setStyleSheet(
        "QPushButton { background-color: #2980b9; color: white; }"
        "QPushButton:hover { background-color: #3498db; }");

    addButtons->addWidget(m_refreshUsersButton);
    addButtons->addWidget(m_addUserButton);
    addButtons->addWidget(m_addAdminButton);
    usersLayout->addLayout(addButtons);

    // Вторая строка кнопок — управление выбранным пользователем
    QHBoxLayout* manageButtons = new QHBoxLayout();
    m_changePasswordButton = new QPushButton("Сменить пароль", this);
    m_upgradeToAdminButton = new QPushButton("Сделать администратором", this);
    m_blockButton          = new QPushButton("Заблокировать", this);
    m_deleteButton         = new QPushButton("Удалить", this);

    m_changePasswordButton->setStyleSheet(
        "QPushButton { background-color: #8e44ad; color: white; }"
        "QPushButton:hover { background-color: #9b59b6; }");
    m_upgradeToAdminButton->setStyleSheet(
        "QPushButton { background-color: #16a085; color: white; }"
        "QPushButton:hover { background-color: #1abc9c; }");
    m_blockButton->setStyleSheet(
        "QPushButton { background-color: #e67e22; color: white; }");
    m_deleteButton->setStyleSheet(
        "QPushButton { background-color: #e74c3c; color: white; }");

    manageButtons->addWidget(m_changePasswordButton);
    manageButtons->addWidget(m_upgradeToAdminButton);
    manageButtons->addStretch();
    manageButtons->addWidget(m_blockButton);
    manageButtons->addWidget(m_deleteButton);
    usersLayout->addLayout(manageButtons);

    m_tabs->addTab(usersTab, "Пользователи");

    // ===== Вкладка "История запросов" =====
    QWidget* historyTab = new QWidget();
    QVBoxLayout* historyLayout = new QVBoxLayout(historyTab);

    m_historyTable = new QTableWidget(0, 5, this);
    m_historyTable->setHorizontalHeaderLabels({"ID", "Пользователь", "Алгоритм", "Входные данные", "Результат"});
    m_historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyLayout->addWidget(m_historyTable);

    m_refreshHistoryButton = new QPushButton("Обновить", this);
    historyLayout->addWidget(m_refreshHistoryButton);
    m_tabs->addTab(historyTab, "История запросов");

    // ===== Вкладка "Логи" =====
    QWidget* logsTab = new QWidget();
    QVBoxLayout* logsLayout = new QVBoxLayout(logsTab);

    m_logsTable = new QTableWidget(0, 3, this);
    m_logsTable->setHorizontalHeaderLabels({"ID", "Действие", "Время"});
    m_logsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_logsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    logsLayout->addWidget(m_logsTable);

    m_refreshLogsButton = new QPushButton("Обновить", this);
    logsLayout->addWidget(m_refreshLogsButton);
    m_tabs->addTab(logsTab, "Логи");

    // Подключаем кнопки
    connect(m_refreshUsersButton,   &QPushButton::clicked, this, &AdminWindow::onRefreshUsers);
    connect(m_refreshHistoryButton, &QPushButton::clicked, this, &AdminWindow::onRefreshHistory);
    connect(m_refreshLogsButton,    &QPushButton::clicked, this, &AdminWindow::onRefreshLogs);
    connect(m_blockButton,          &QPushButton::clicked, this, &AdminWindow::onBlockUser);
    connect(m_deleteButton,         &QPushButton::clicked, this, &AdminWindow::onDeleteUser);
    connect(m_addUserButton,        &QPushButton::clicked, this, &AdminWindow::onAddUser);
    connect(m_addAdminButton,       &QPushButton::clicked, this, &AdminWindow::onAddAdmin);
    connect(m_changePasswordButton, &QPushButton::clicked, this, &AdminWindow::onChangePassword);
    connect(m_upgradeToAdminButton, &QPushButton::clicked, this, &AdminWindow::onUpgradeToAdmin);
}

void AdminWindow::onRefreshUsers()
{
    m_pendingCommand = "ADMIN_GET_USERS";
    NetworkManager::getInstance()->sendRequest("ADMIN_GET_USERS");
}

void AdminWindow::onRefreshHistory()
{
    m_pendingCommand = "ADMIN_GET_HISTORY";
    NetworkManager::getInstance()->sendRequest("ADMIN_GET_HISTORY");
}

void AdminWindow::onRefreshLogs()
{
    m_pendingCommand = "ADMIN_GET_LOGS";
    NetworkManager::getInstance()->sendRequest("ADMIN_GET_LOGS");
}

void AdminWindow::onBlockUser()
{
    int row = m_usersTable->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Ошибка", "Выберите пользователя"); return; }
    QString login = m_usersTable->item(row, 1)->text();
    m_pendingCommand = "ADMIN_BLOCK_USER";
    NetworkManager::getInstance()->sendRequest(QString("ADMIN_BLOCK_USER;%1").arg(login));
}

void AdminWindow::onDeleteUser()
{
    int row = m_usersTable->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Ошибка", "Выберите пользователя"); return; }
    QString login = m_usersTable->item(row, 1)->text();

    auto reply = QMessageBox::question(this, "Подтверждение",
        QString("Удалить пользователя %1?").arg(login));

    if (reply == QMessageBox::Yes) {
        m_pendingCommand = "ADMIN_DELETE_USER";
        NetworkManager::getInstance()->sendRequest(QString("ADMIN_DELETE_USER;%1").arg(login));
    }
}

void AdminWindow::onChangePassword()
{
    int row = m_usersTable->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Ошибка", "Выберите пользователя"); return; }
    QString login = m_usersTable->item(row, 1)->text();

    QDialog dialog(this);
    dialog.setWindowTitle(QString("Смена пароля — %1").arg(login));
    dialog.setMinimumWidth(300);

    QFormLayout* form = new QFormLayout(&dialog);
    QLineEdit* newPassEdit    = new QLineEdit(&dialog);
    QLineEdit* confirmEdit    = new QLineEdit(&dialog);
    newPassEdit->setEchoMode(QLineEdit::Password);
    confirmEdit->setEchoMode(QLineEdit::Password);

    form->addRow("Новый пароль:", newPassEdit);
    form->addRow("Подтверждение:", confirmEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    QString newPass = newPassEdit->text();
    QString confirm = confirmEdit->text();

    if (newPass.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите новый пароль");
        return;
    }
    if (newPass != confirm) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают");
        return;
    }

    // Отправляем: ADMIN_CHANGE_PASSWORD;login;newPassword
    NetworkManager::getInstance()->sendRequest(
        QString("ADMIN_CHANGE_PASSWORD;%1;%2").arg(login, newPass)
    );
}

void AdminWindow::onUpgradeToAdmin()
{
    int row = m_usersTable->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Ошибка", "Выберите пользователя"); return; }

    QString login = m_usersTable->item(row, 1)->text();
    QString role  = m_usersTable->item(row, 2)->text();

    if (role == "admin") {
        QMessageBox::information(this, "Инфо", "Пользователь уже является администратором");
        return;
    }

    auto reply = QMessageBox::question(this, "Подтверждение",
        QString("Сделать %1 администратором?").arg(login));

    if (reply == QMessageBox::Yes) {
        // Отправляем: ADMIN_UPGRADE_USER;login
        NetworkManager::getInstance()->sendRequest(
            QString("ADMIN_UPGRADE_USER;%1").arg(login)
        );
    }
}

void AdminWindow::onAddUser()  { showAddUserDialog("user"); }
void AdminWindow::onAddAdmin() { showAddUserDialog("admin"); }

void AdminWindow::showAddUserDialog(const QString& role)
{
    QDialog dialog(this);
    dialog.setWindowTitle(role == "admin" ? "Добавить администратора" : "Добавить пользователя");
    dialog.setMinimumWidth(300);

    QFormLayout* form = new QFormLayout(&dialog);
    QLineEdit* loginEdit    = new QLineEdit(&dialog);
    QLineEdit* passwordEdit = new QLineEdit(&dialog);
    passwordEdit->setEchoMode(QLineEdit::Password);

    form->addRow("Логин:", loginEdit);
    form->addRow("Пароль:", passwordEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    QString login    = loginEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля");
        return;
    }

    NetworkManager::getInstance()->sendRequest(
        QString("ADMIN_ADD_USER;%1;%2;%3").arg(login, password, role)
    );
}

void AdminWindow::onServerResponse(const QString& response)
{
    if (response.startsWith("ADMIN_GET_USERS;")) {
        fillTable(m_usersTable, response.mid(16));
    }
    else if (response.startsWith("ADMIN_GET_HISTORY;")) {
        fillTable(m_historyTable, response.mid(18));
    }
    else if (response.startsWith("ADMIN_GET_LOGS;")) {
        fillTable(m_logsTable, response.mid(15));
    }
    else if (response.startsWith("ADMIN_BLOCK_USER;") || response.startsWith("ADMIN_DELETE_USER;")) {
        QMessageBox::information(this, "Готово", "Операция выполнена");
        onRefreshUsers();
    }
    else if (response.startsWith("ADMIN_ADD_USER;")) {
        QString status = response.mid(15);
        if (status == "SUCCESS") {
            QMessageBox::information(this, "Готово", "Пользователь добавлен");
            onRefreshUsers();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось добавить. Логин уже занят.");
        }
    }
    else if (response.startsWith("ADMIN_CHANGE_PASSWORD;")) {
        QString status = response.mid(22);
        if (status == "SUCCESS") {
            QMessageBox::information(this, "Готово", "Пароль успешно изменён");
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось изменить пароль");
        }
    }
    else if (response.startsWith("ADMIN_UPGRADE_USER;")) {
        QString status = response.mid(19);
        if (status == "SUCCESS") {
            QMessageBox::information(this, "Готово", "Пользователь повышен до администратора");
            onRefreshUsers();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось изменить роль");
        }
    }
}

void AdminWindow::fillTable(QTableWidget* table, const QString& csvData)
{
    table->setRowCount(0);
    if (csvData.isEmpty() || csvData == "EMPTY") return;

    QStringList rows = csvData.split('|', Qt::SkipEmptyParts);
    for (const QString& rowStr : rows) {
        QStringList cols = rowStr.split(',');
        int rowIdx = table->rowCount();
        table->insertRow(rowIdx);
        for (int c = 0; c < cols.size() && c < table->columnCount(); ++c) {
            table->setItem(rowIdx, c, new QTableWidgetItem(cols.at(c).trimmed()));
        }
    }
}