#include "userwindow.h"
#include "ui_userwindow.h"

#include "shawindow.h"
#include "splinewindow.h"
#include "gradientwindow.h"
#include "vigenerewindow.h"
#include "msewindow.h"

#include <QPushButton>
#include <QMessageBox>

UserWindow::UserWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
{
    ui->setupUi(this);


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
        [this]()
        {
            auto* window = new SplineWindow();
            window->show();

            this->close();
        }
    );

    // GRADIENT

    connect(
        ui->gradientButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            auto* window = new GradientWindow();
            window->show();

            this->close();
        }
    );

    // VIGENERE

    connect(
        ui->vigenereButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            auto* window = new VigenereWindow();

            window->show();

            this->close();
        }
    );
    connect(
        ui->mseButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            auto* window = new MSEWindow();

            window->show();

            this->close();
        }
    );
}

UserWindow::~UserWindow()
{
    delete ui;
}