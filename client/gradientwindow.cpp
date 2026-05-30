#include "gradientwindow.h"
#include "ui_gradientwindow.h"
#include "userwindow.h"

#include "../shared/gradient.h"

#include <QMessageBox>

GradientWindow::GradientWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::GradientWindow)
{
    ui->setupUi(this);

    connect(
        ui->calculateButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString func =
                ui->functionEdit->text();

            double start =
                ui->startEdit->text().toDouble();

            double learningRate =
                ui->rateEdit->text().toDouble();

            int iterations =
                ui->iterationsEdit->text().toInt();

            double result =
                gradientDescent(
                    func.toStdString(),
                    start,
                    learningRate,
                    iterations
                );

            ui->resultLabel->setText(
                QString::number(result, 'f', 10)
            );
        }
    );

    connect(
        ui->backButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            auto* menu = new UserWindow();
            menu->show();
            this->close();
        }
    );
}

GradientWindow::~GradientWindow()
{
    delete ui;
}