#include "loginwindow.h"
#include "ui_loginwindow.h"

#include "userwindow.h"

#include <QPushButton>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    connect(
        ui->loginButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString login =
                ui->loginEdit->text();

            QString password =
                ui->passwordEdit->text();

            if (login == "admin" &&
                password == "12345")
            {
                UserWindow* window =
                    new UserWindow();

                window->show();

                this->close();
            }
            else
            {
                QMessageBox::warning(
                    this,
                    "Error",
                    "Invalid login or password"
                );
            }
        }
    );
}

LoginWindow::~LoginWindow()
{
    delete ui;
}