#include "vigenerewindow.h"
#include "ui_vigenerewindow.h"
#include "userwindow.h"

#include "../shared/vigenere.h"

#include <QMessageBox>

VigenereWindow::VigenereWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::VigenereWindow)
{
    ui->setupUi(this);

    connect(
        ui->encryptButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString text =
                ui->plainTextEdit->text();

            QString key =
                ui->keyEdit->text();

            if(text.isEmpty() || key.isEmpty())
            {
                QMessageBox::warning(
                    this,
                    "Error",
                    "Fill all fields");

                return;
            }

            std::string result =
                vigenereEncrypt(
                    text.toStdString(),
                    key.toStdString()
                );

            ui->encryptedEdit->setText(
                QString::fromStdString(result)
            );
        });

    connect(
        ui->decryptButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString text =
                ui->cipherTextEdit->text();

            QString key =
                ui->decryptKeyEdit->text();

            if(text.isEmpty() || key.isEmpty())
            {
                QMessageBox::warning(
                    this,
                    "Error",
                    "Fill all fields");

                return;
            }

            std::string result =
                vigenereDecrypt(
                    text.toStdString(),
                    key.toStdString()
                );

            ui->decryptedEdit->setText(
                QString::fromStdString(result)
            );
        });

    connect(
        ui->backButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            auto* window = new UserWindow();

            window->show();

            this->close();
        }
    );
}

VigenereWindow::~VigenereWindow()
{
    delete ui;
}