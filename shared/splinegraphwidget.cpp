#include "splinegraphwidget.h"

#include <QPainter>
#include <algorithm>

SplineGraphWidget::SplineGraphWidget(QWidget* parent)
    : QWidget(parent)
{
}

void SplineGraphWidget::setData(
    const std::vector<double>& x,
    const std::vector<double>& y)
{
    m_x = x;
    m_y = y;

    update();
}

void SplineGraphWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.fillRect(rect(), Qt::white);

    if (m_x.empty())
        return;

    int margin = 40;

    int w = width() - margin * 2;
    int h = height() - margin * 2;

    double minX =
        *std::min_element(m_x.begin(), m_x.end());

    double maxX =
        *std::max_element(m_x.begin(), m_x.end());

    double minY =
        *std::min_element(m_y.begin(), m_y.end());

    double maxY =
        *std::max_element(m_y.begin(), m_y.end());

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

    for (size_t i = 0; i < m_x.size(); i++)
    {
        int px =
            margin +
            (m_x[i] - minX) *
            w /
            (maxX - minX);

        int py =
            height() - margin -
            (m_y[i] - minY) *
            h /
            (maxY - minY);

        painter.drawEllipse(px - 3, py - 3, 6, 6);
    }

    painter.setPen(Qt::red);

    for (size_t i = 0; i + 1 < m_x.size(); i++)
    {
        int x1 =
            margin +
            (m_x[i] - minX) *
            w /
            (maxX - minX);

        int y1 =
            height() - margin -
            (m_y[i] - minY) *
            h /
            (maxY - minY);

        int x2 =
            margin +
            (m_x[i + 1] - minX) *
            w /
            (maxX - minX);

        int y2 =
            height() - margin -
            (m_y[i + 1] - minY) *
            h /
            (maxY - minY);

        painter.drawLine(x1, y1, x2, y2);
    }
}