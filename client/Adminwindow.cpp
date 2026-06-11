#include "adminwindow.h"
#include "networkmanager.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

AdminWindow::AdminWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Панель администратора");
    setMinimumSize(800, 500);

    setupUi();

    // Подписываемся на ответы сервера
    connect(
        NetworkManager::getInstance(),
        &NetworkManager::responseReceived,
        this,
        &AdminWindow::onServerResponse
    );

    // Загружаем пользователей сразу при открытии
    onRefreshUsers();
}

void AdminWindow::setupUi()
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Заголовок
    QLabel* title = new QLabel("Панель администратора", this);
    QFont f = title->font();
    f.setPointSize(14);
    f.setBold(true);
    title->setFont(f);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Вкладки
    m_tabs = new QTabWidget(this);
    mainLayout->addWidget(m_tabs);

    // ===== Вкладка "Пользователи" =====
    QWidget* usersTab = new QWidget();
    QVBoxLayout* usersLayout = new QVBoxLayout(usersTab);

    m_usersTable = new QTableWidget(0, 3, this);
    m_usersTable->setHorizontalHeaderLabels({"ID", "Логин", "Роль" });
    m_usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    usersLayout->addWidget(m_usersTable);

    QHBoxLayout* userButtons = new QHBoxLayout();
    m_refreshUsersButton = new QPushButton("Обновить", this);
    m_blockButton        = new QPushButton("Заблокировать", this);
    m_deleteButton       = new QPushButton("Удалить", this);
    m_blockButton->setStyleSheet("QPushButton { background-color: #e67e22; color: white; }");
    m_deleteButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; }");
    userButtons->addWidget(m_refreshUsersButton);
    userButtons->addStretch();
    userButtons->addWidget(m_blockButton);
    userButtons->addWidget(m_deleteButton);
    usersLayout->addLayout(userButtons);
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
    connect(m_refreshUsersButton,  &QPushButton::clicked, this, &AdminWindow::onRefreshUsers);
    connect(m_refreshHistoryButton,&QPushButton::clicked, this, &AdminWindow::onRefreshHistory);
    connect(m_refreshLogsButton,   &QPushButton::clicked, this, &AdminWindow::onRefreshLogs);
    connect(m_blockButton,         &QPushButton::clicked, this, &AdminWindow::onBlockUser);
    connect(m_deleteButton,        &QPushButton::clicked, this, &AdminWindow::onDeleteUser);
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
    if (row < 0)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя");
        return;
    }
    QString login = m_usersTable->item(row, 1)->text();
    m_pendingCommand = "ADMIN_BLOCK_USER";
    NetworkManager::getInstance()->sendRequest(QString("ADMIN_BLOCK_USER;%1").arg(login));
}

void AdminWindow::onDeleteUser()
{
    int row = m_usersTable->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя");
        return;
    }
    QString login = m_usersTable->item(row, 1)->text();

    auto reply = QMessageBox::question(
        this, "Подтверждение",
        QString("Удалить пользователя %1?").arg(login)
    );

    if (reply == QMessageBox::Yes)
    {
        m_pendingCommand = "ADMIN_DELETE_USER";
        NetworkManager::getInstance()->sendRequest(QString("ADMIN_DELETE_USER;%1").arg(login));
    }
}

// Разбираем ответ сервера и заполняем нужную таблицу
// Формат от сервера: ADMIN_GET_USERS;id,login,role,date|id,login,role,date|...
void AdminWindow::onServerResponse(const QString& response)
{
    if (response.startsWith("ADMIN_GET_USERS;"))
    {
        QString data = response.mid(QString("ADMIN_GET_USERS;").length());
        fillTable(m_usersTable, data);
    }
    else if (response.startsWith("ADMIN_GET_HISTORY;"))
    {
        QString data = response.mid(QString("ADMIN_GET_HISTORY;").length());
        fillTable(m_historyTable, data);
    }
    else if (response.startsWith("ADMIN_GET_LOGS;"))
    {
        QString data = response.mid(QString("ADMIN_GET_LOGS;").length());
        fillTable(m_logsTable, data);
    }
    else if (response.startsWith("ADMIN_BLOCK_USER;") || response.startsWith("ADMIN_DELETE_USER;"))
    {
        QMessageBox::information(this, "Готово", "Операция выполнена");
        onRefreshUsers(); // обновляем список
    }
}

// Заполняем QTableWidget из строки формата:
// "val1,val2,val3|val1,val2,val3|..."
void AdminWindow::fillTable(QTableWidget* table, const QString& csvData)
{
    table->setRowCount(0);

    if (csvData.isEmpty() || csvData == "EMPTY")
        return;

    QStringList rows = csvData.split('|', Qt::SkipEmptyParts);
    for (const QString& rowStr : rows)
    {
        QStringList cols = rowStr.split(',');
        int rowIdx = table->rowCount();
        table->insertRow(rowIdx);
        for (int c = 0; c < cols.size() && c < table->columnCount(); ++c)
        {
            table->setItem(rowIdx, c, new QTableWidgetItem(cols.at(c).trimmed()));
        }
    }
}