/**
 * @file gradientgraphwidget.h
 * @brief Заголовочный файл виджета градиентного графика
 */

#pragma once

#include <QWidget>
#include <vector>

 /**
  * @class GradientGraphWidget
  * @brief Виджет для отображения графика с градиентной заливкой
  *
  * Позволяет отображать набор точек и соединять их линией с градиентной заливкой под областью графика.
  */
class GradientGraphWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор виджета градиентного графика
     * @param parent Родительский виджет (по умолчанию nullptr)
     */
    explicit GradientGraphWidget(QWidget* parent = nullptr);

    /**
     * @brief Установить точки для отображения на графике
     * @param points Вектор значений точек (Y-координаты)
     */
    void setPoints(const std::vector<double>& points);

protected:
    /**
     * @brief Обработчик события перерисовки виджета
     * @param event Указатель на событие перерисовки
     */
    void paintEvent(QPaintEvent*) override;

private:
    std::vector<double> m_points; ///< Вектор точек для отображения
};