#include "msewindow.h"
#include "ui_msewindow.h"
#include "msegraphwidget.h"

#include <sstream>
#include <vector>
#include <cmath>

#include "userwindow.h"

#include <QMessageBox>

MSEWindow::MSEWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MSEWindow)
{
    ui->setupUi(this);

    connect(
        ui->calculateButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            std::vector<double> x;
            std::vector<double> y;

            std::stringstream sx(
                ui->xEdit->text().toStdString());

            std::stringstream sy(
                ui->yEdit->text().toStdString());

            double value;

            while (sx >> value)
                x.push_back(value);

            while (sy >> value)
                y.push_back(value);

            if (x.size() != y.size() || x.empty())
            {
                QMessageBox::warning(
                    this,
                    "Error",
                    "Invalid data");

                return;
            }

            int n = static_cast<int>(x.size());

            double sumX = 0;
            double sumY = 0;
            double sumXY = 0;
            double sumXX = 0;

            for (int i = 0; i < n; i++)
            {
                sumX += x[i];
                sumY += y[i];
                sumXY += x[i] * y[i];
                sumXX += x[i] * x[i];
            }

            double a =
                (n * sumXY - sumX * sumY) /
                (n * sumXX - sumX * sumX);

            double b =
                (sumY - a * sumX) / n;

            std::vector<double> yPred;

            double mse = 0;

            for (int i = 0; i < n; i++)
            {
                double pred = a * x[i] + b;

                yPred.push_back(pred);

                mse +=
                    (y[i] - pred) *
                    (y[i] - pred);
            }

            mse /= n;

            ui->resultLabel->setText(
                QString("MSE = %1")
                    .arg(mse));

            auto graph =
                findChild<MSEGraphWidget*>("graphWidget");

            if (graph)
            {
                graph->setData(
                    x,
                    y,
                    yPred);
            }
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

MSEWindow::~MSEWindow()
{
    delete ui;
}