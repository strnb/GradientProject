#include "userwindow.h"
#include "ui_userwindow.h"

#include "networkmanager.h"
#include "shawindow.h"
#include "splinewindow.h"
#include "gradientwindow.h"
#include "vigenerewindow.h"
#include "msewindow.h"

#include <QPushButton>

UserWindow::UserWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
{
    ui->setupUi(this);

    QString login = NetworkManager::getInstance()->login();
    setWindowTitle(QString("Главное меню — %1").arg(login));

    // SHA-256
    connect(ui->shaButton, &QPushButton::clicked, this, [this]() {
        auto* window = new ShaWindow();
        window->show();
        this->close();
    });

    // Сплайны
    connect(ui->splineButton, &QPushButton::clicked, this, [this]() {
        auto* window = new SplineWindow();
        window->show();
        this->close();
    });

    // Градиентный спуск
    connect(ui->gradientButton, &QPushButton::clicked, this, [this]() {
        auto* window = new GradientWindow();
        window->show();
        this->close();
    });

    // Шифр Виженера
    connect(ui->vigenereButton, &QPushButton::clicked, this, [this]() {
        auto* window = new VigenereWindow();
        window->show();
        this->close();
    });

    // MSE
    connect(ui->mseButton, &QPushButton::clicked, this, [this]() {
        auto* window = new MSEWindow();
        window->show();
        this->close();
    });


}

UserWindow::~UserWindow()
{
    delete ui;
}