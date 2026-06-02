#include "msegraphwidget.h"

#include <QPainter>

MSEGraphWidget::MSEGraphWidget(QWidget* parent)
    : QWidget(parent)
{
}

void MSEGraphWidget::setData(
    const std::vector<double>& x,
    const std::vector<double>& y,
    const std::vector<double>& yPred)
{
    m_x = x;
    m_y = y;
    m_yPred = yPred;

    update();
}

void MSEGraphWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.fillRect(rect(), Qt::white);

    if(m_x.empty())
        return;

    int margin = 40;

    double maxX = *std::max_element(m_x.begin(), m_x.end());
    double minX = *std::min_element(m_x.begin(), m_x.end());

    double maxY = *std::max_element(m_y.begin(), m_y.end());
    double minY = *std::min_element(m_y.begin(), m_y.end());

    painter.drawLine(
        margin,
        height() - margin,
        width() - margin,
        height() - margin);

    painter.drawLine(
        margin,
        margin,
        margin,
        height() - margin);

    painter.setPen(Qt::blue);

    for(size_t i = 0; i < m_x.size(); i++)
    {
        int px =
            margin +
            (m_x[i] - minX) /
            (maxX - minX) *
            (width() - 2 * margin);

        int py =
            height() - margin -
            (m_y[i] - minY) /
            (maxY - minY) *
            (height() - 2 * margin);

        painter.drawEllipse(px - 4, py - 4, 8, 8);
    }

    painter.setPen(Qt::red);

    for(size_t i = 1; i < m_x.size(); i++)
    {
        int x1 =
            margin +
            (m_x[i - 1] - minX) /
            (maxX - minX) *
            (width() - 2 * margin);

        int y1 =
            height() - margin -
            (m_yPred[i - 1] - minY) /
            (maxY - minY) *
            (height() - 2 * margin);

        int x2 =
            margin +
            (m_x[i] - minX) /
            (maxX - minX) *
            (width() - 2 * margin);

        int y2 =
            height() - margin -
            (m_yPred[i] - minY) /
            (maxY - minY) *
            (height() - 2 * margin);

        painter.drawLine(x1, y1, x2, y2);
    }
}
