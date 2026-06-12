#include "gradientwindow.h"
#include "ui_gradientwindow.h"
#include "userwindow.h"
#include "networkmanager.h"

#include "../shared/gradient.h"

#include <QMessageBox>

GradientWindow::GradientWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::GradientWindow)
{
    ui->setupUi(this);

    connect(ui->calculateButton, &QPushButton::clicked, this, [this]() {
        QString func       = ui->functionEdit->text();
        double start       = ui->startEdit->text().toDouble();
        double learningRate = ui->rateEdit->text().toDouble();
        int iterations     = ui->iterationsEdit->text().toInt();

        if (func.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Введите функцию");
            return;
        }

        double result = gradientDescent(func.toStdString(), start, learningRate, iterations);
        auto points   = gradientPath(func.toStdString(), start, learningRate, iterations);

        auto graph = findChild<GradientGraphWidget*>("graphWidget");
        if (graph) graph->setPoints(points);

        ui->resultLabel->setText(QString::number(result, 'f', 10));

        // Логируем на сервер
        NetworkManager::getInstance()->sendRequest(
            QString("CALC_GRAD;%1;%2;%3;%4")
                .arg(func)
                .arg(start)
                .arg(learningRate)
                .arg(iterations)
        );
    });

    connect(ui->backButton, &QPushButton::clicked, this, [this]() {
        auto* menu = new UserWindow();
        menu->show();
        this->close();
    });
}

GradientWindow::~GradientWindow()
{
    delete ui;
}