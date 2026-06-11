#include "shawindow.h"
#include "ui_shawindow.h"

#include "userwindow.h"
#include "networkmanager.h"
#include "../shared/crypto_utils.h"

#include <QPushButton>
#include <QMessageBox>

ShaWindow::ShaWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ShaWindow)
{
    ui->setupUi(this);

    // GENERATE SHA-256
    connect(
        ui->generateButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString text = ui->inputEdit->text();
            if (text.isEmpty()) return;

            // Считаем локально
            std::string hash = hashPassword(text.toStdString());
            QString result = QString::fromStdString(hash);
            ui->resultEdit->setText(result);

            // Логируем на сервер
            NetworkManager::getInstance()->sendRequest(
                QString("CALC_SHA;%1").arg(text)
            );
        }
    );

    // VERIFY HASH
    connect(
        ui->verifyButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString text = ui->verifyTextEdit->text();
            QString hash = ui->verifyHashEdit->text();

            std::string generatedHash = hashPassword(text.toStdString());

            if (generatedHash == hash.toStdString())
            {
                QMessageBox::information(this, "Успех", "Хэш совпадает");
            }
            else
            {
                QMessageBox::warning(this, "Ошибка", "Хэш не совпадает");
            }
        }
    );

    // BACK BUTTON
    connect(
        ui->backButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            UserWindow* window = new UserWindow();
            window->show();
            this->close();
        }
    );
}

ShaWindow::~ShaWindow()
{
    delete ui;
}