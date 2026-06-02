#pragma once

#include <QWidget>
#include <vector>

class MSEGraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MSEGraphWidget(QWidget* parent = nullptr);

    void setData(
        const std::vector<double>& x,
        const std::vector<double>& y,
        const std::vector<double>& yPred);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<double> m_x;
    std::vector<double> m_y;
    std::vector<double> m_yPred;
};
