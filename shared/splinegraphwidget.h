#pragma once

#include <QWidget>
#include <vector>

class SplineGraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SplineGraphWidget(QWidget* parent = nullptr);

    void setData(
        const std::vector<double>& x,
        const std::vector<double>& y
    );

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<double> m_x;
    std::vector<double> m_y;
};