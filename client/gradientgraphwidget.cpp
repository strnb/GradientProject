#include "gradientgraphwidget.h"

#include <QPainter>
#include <cmath>

GradientGraphWidget::GradientGraphWidget(QWidget* parent)
    : QWidget(parent)
{
}

void GradientGraphWidget::setPoints(
    const std::vector<double>& points)
{
    m_points = points;
    update();
}

void GradientGraphWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    p.fillRect(rect(), Qt::white);

    int margin = 40;

    p.drawLine(
        margin,
        height() - margin,
        width() - margin,
        height() - margin);

    p.drawLine(
        margin,
        margin,
        margin,
        height() - margin);

    if (m_points.empty())
        return;

    p.setPen(Qt::blue);

    for (int px = 0; px < width() - 2 * margin; px++)
    {
        double x =
            -5.0 +
            10.0 * px /
            (width() - 2 * margin);

        double y = x * x;

        int drawX =
            margin + px;

        int drawY =
            height() - margin -
            y * 10;

        if (drawY >= margin &&
            drawY <= height() - margin)
        {
            p.drawPoint(
                drawX,
                drawY
            );
        }
    }

    p.setPen(Qt::red);

    for (double x : m_points)
    {
        double y = x * x;

        int drawX =
            margin +
            (x + 5.0) *
            (width() - 2 * margin) /
            10.0;

        int drawY =
            height() - margin -
            y * 10;

        p.drawEllipse(
            drawX - 4,
            drawY - 4,
            8,
            8
        );
    }
}
