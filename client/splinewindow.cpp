#include "splinewindow.h"
#include "ui_splinewindow.h"
#include "userwindow.h"
#include "networkmanager.h"

#include "../shared/spline.h"

#include <QStringList>
#include <QMessageBox>

SplineWindow::SplineWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::SplineWindow)
{
    ui->setupUi(this);

    connect(ui->calculateButton, &QPushButton::clicked, this, [this]() {
        QString xText    = ui->xEdit->text();
        QString yText    = ui->yEdit->text();
        QString queryText = ui->queryEdit->text();

        QStringList xList = xText.split(" ", Qt::SkipEmptyParts);
        QStringList yList = yText.split(" ", Qt::SkipEmptyParts);

        std::vector<double> x, y;
        for (const QString& v : xList) x.push_back(v.toDouble());
        for (const QString& v : yList) y.push_back(v.toDouble());

        if (x.size() != y.size()) {
            QMessageBox::warning(this, "Error", "X and Y size mismatch");
            return;
        }

        double query  = queryText.toDouble();
        double result = splineInterpolate(x, y, query);

        auto graph = findChild<SplineGraphWidget*>("graphWidget");
        if (graph) graph->setData(x, y);

        ui->resultLabel->setText(QString::number(result));

        // Логируем на сервер
        NetworkManager::getInstance()->sendRequest(
            QString("CALC_SPLINE;%1;%2;%3")
                .arg(xText, yText, queryText)
        );
    });

    connect(ui->backButton, &QPushButton::clicked, this, [this]() {
        auto* menu = new UserWindow();
        menu->show();
        this->close();
    });
}

SplineWindow::~SplineWindow()
{
    delete ui;
}