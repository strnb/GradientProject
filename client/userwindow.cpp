#include "userwindow.h"
#include "ui_userwindow.h"

#include "shawindow.h"

#include <QPushButton>
#include <QMessageBox>

UserWindow::UserWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
{
    ui->setupUi(this);

    // VIGENERE

    connect(
        ui->vigenereButton,
        &QPushButton::clicked,
        this,
        []()
        {
            QMessageBox::information(
                nullptr,
                "Vigenere",
                "Vigenere cipher window"
            );
        }
    );

    // SHA-256 WINDOW

    connect(
        ui->shaButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            ShaWindow* window =
                new ShaWindow();

            window->show();

            this->close();
        }
    );

    // SPLINES

    connect(
        ui->splineButton,
        &QPushButton::clicked,
        this,
        []()
        {
            QMessageBox::information(
                nullptr,
                "Splines",
                "Spline interpolation window"
            );
        }
    );

    // GRADIENT

    connect(
        ui->gradientButton,
        &QPushButton::clicked,
        this,
        []()
        {
            QMessageBox::information(
                nullptr,
                "Gradient",
                "Gradient descent window"
            );
        }
    );
}

UserWindow::~UserWindow()
{
    delete ui;
}