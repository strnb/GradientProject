#include "shawindow.h"
#include "ui_shawindow.h"

#include "userwindow.h"

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
            QString text =
                ui->inputEdit->text();

            std::string hash =
                hashPassword(
                    text.toStdString()
                );

            ui->resultEdit->setText(
                QString::fromStdString(hash)
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
            QString text =
                ui->verifyTextEdit->text();

            QString hash =
                ui->verifyHashEdit->text();

            std::string generatedHash =
                hashPassword(
                    text.toStdString()
                );

            if (generatedHash ==
                hash.toStdString())
            {
                QMessageBox::information(
                    this,
                    "Success",
                    "Hash matches"
                );
            }
            else
            {
                QMessageBox::warning(
                    this,
                    "Error",
                    "Hash does not match"
                );
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
            UserWindow* window =
                new UserWindow();

            window->show();

            this->close();
        }
    );
}

ShaWindow::~ShaWindow()
{
    delete ui;
}