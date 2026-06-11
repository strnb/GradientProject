#include "loginwindow.h"
#include "ui_loginwindow.h"

#include "networkmanager.h"
#include "userwindow.h"
#include "adminwindow.h"

#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

LoginWindow::LoginWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
    , m_loginAsAdmin(false)
{
    ui->setupUi(this);

    // Показываем статус подключения
    connect(NetworkManager::getInstance(), &NetworkManager::connected, this, [this]() {
        ui->statusLabel->setText("Сервер подключён ✓");
        ui->statusLabel->setStyleSheet("color: green; font-size: 11px;");
    });

    connect(NetworkManager::getInstance(), &NetworkManager::disconnected, this, [this]() {
        ui->statusLabel->setText("Сервер недоступен ✗");
        ui->statusLabel->setStyleSheet("color: red; font-size: 11px;");
    });

    // Подписываемся на ответы сервера
    connect(NetworkManager::getInstance(), &NetworkManager::responseReceived,
            this, &LoginWindow::onServerResponse);

    // Кнопка входа как пользователь
    connect(ui->loginButton, &QPushButton::clicked, this, [this]() {
        m_loginAsAdmin = false;
        onLoginButtonClicked();
    });

    // Кнопка входа как администратор
    connect(ui->adminLoginButton, &QPushButton::clicked, this, [this]() {
        m_loginAsAdmin = true;
        onLoginButtonClicked();
    });
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::onLoginButtonClicked()
{
    QString login    = ui->loginEdit->text().trimmed();
    QString password = ui->passwordEdit->text();

    if (login.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль");
        return;
    }

    setUiEnabled(false);
    ui->statusLabel->setText("Авторизация...");
    ui->statusLabel->setStyleSheet("color: gray; font-size: 11px;");

    NetworkManager::getInstance()->sendRequest(
        QString("AUTH;%1;%2").arg(login, password)
    );
}

void LoginWindow::onServerResponse(const QString& response)
{
    if (!response.startsWith("AUTH;"))
        return;

    setUiEnabled(true);

    QStringList parts = response.split(';');

    if (parts.size() >= 3 && parts.at(1) == "SUCCESS")
    {
        QString role  = parts.at(2);
        QString login = ui->loginEdit->text().trimmed();

        // Если нажали "Войти как admin" но роль не admin — отказываем
        if (m_loginAsAdmin && role != "admin")
        {
            ui->statusLabel->setText("Нет прав администратора");
            ui->statusLabel->setStyleSheet("color: red; font-size: 11px;");
            QMessageBox::warning(this, "Отказано", "У этого пользователя нет прав администратора");
            return;
        }

        NetworkManager::getInstance()->setRole(role);
        NetworkManager::getInstance()->setLogin(login);

        qDebug() << "Авторизация успешна. Роль:" << role;

        disconnect(NetworkManager::getInstance(), &NetworkManager::responseReceived,
                   this, &LoginWindow::onServerResponse);

        if (m_loginAsAdmin)
        {
            AdminWindow* window = new AdminWindow();
            window->show();
        }
        else
        {
            UserWindow* window = new UserWindow();
            window->show();
        }

        this->hide(); // было this->close()
    }
    else
    {
        ui->statusLabel->setText("Неверный логин или пароль");
        ui->statusLabel->setStyleSheet("color: red; font-size: 11px;");
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}

void LoginWindow::setUiEnabled(bool enabled)
{
    ui->loginEdit->setEnabled(enabled);
    ui->passwordEdit->setEnabled(enabled);
    ui->loginButton->setEnabled(enabled);
    ui->adminLoginButton->setEnabled(enabled);
}