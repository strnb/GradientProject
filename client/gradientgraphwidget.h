#pragma once

#include <QWidget>
#include <vector>

class GradientGraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GradientGraphWidget(QWidget* parent = nullptr);

    void setPoints(
        const std::vector<double>& points
    );

protected:
    void paintEvent(QPaintEvent*) override;

private:
    std::vector<double> m_points;
};