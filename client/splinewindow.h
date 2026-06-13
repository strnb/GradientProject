/**
 * @file splinewindow.h
 * @brief Заголовочный файл окна сплайн-интерполяции
 */

#ifndef SPLINEWINDOW_H
#define SPLINEWINDOW_H

#include <QMainWindow>
#include "splinegraphwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class SplineWindow; ///< UI-класс окна сплайн-интерполяции
}
QT_END_NAMESPACE

/**
 * @class SplineWindow
 * @brief Главное окно для работы со сплайн-интерполяцией
 *
 * Окно предоставляет интерфейс для построения и отображения
 * сплайн-кривых на основе заданных опорных точек.
 */
    class SplineWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор окна сплайн-интерполяции
     * @param parent Родительский виджет (по умолчанию nullptr)
     */
    explicit SplineWindow(QWidget* parent = nullptr);

    /// Деструктор
    ~SplineWindow() override;

private:
    Ui::SplineWindow* ui; ///< Указатель на UI-компоненты
};

#endif // SPLINEWINDOW_H