#include "splinewindow.h"
#include "ui_splinewindow.h"

#include "../shared/spline.h"

#include <QStringList>
#include <QMessageBox>

SplineWindow::SplineWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::SplineWindow)
{
    ui->setupUi(this);

    connect(
        ui->calculateButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString xText =
                ui->xEdit->text();

            QString yText =
                ui->yEdit->text();

            QString queryText =
                ui->queryEdit->text();

            QStringList xList =
                xText.split(" ");

            QStringList yList =
                yText.split(" ");

            std::vector<double> x;
            std::vector<double> y;

            for (const QString& value : xList)
            {
                x.push_back(value.toDouble());
            }

            for (const QString& value : yList)
            {
                y.push_back(value.toDouble());
            }

            if (x.size() != y.size())
            {
                QMessageBox::warning(
                    this,
                    "Error",
                    "X and Y size mismatch"
                );

                return;
            }

            double query =
                queryText.toDouble();

            double result =
                splineInterpolate(
                    x,
                    y,
                    query
                );

            ui->resultLabel->setText(
                QString::number(result)
            );
        }
    );

    connect(
        ui->backButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            this->close();
        }
    );
}

SplineWindow::~SplineWindow()
{
    delete ui;
}