/**
 * @file gradientwindow.h
 * @brief Заголовочный файл окна градиентного метода
 */

#ifndef GRADIENTWINDOW_H
#define GRADIENTWINDOW_H

#include <QMainWindow>
#include "gradientgraphwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class GradientWindow; ///< UI-класс окна градиентного метода
}
QT_END_NAMESPACE

/**
 * @class GradientWindow
 * @brief Главное окно для работы с градиентным методом
 *
 * Окно предоставляет интерфейс для настройки параметров и отображения результатов
 * работы градиентного метода оптимизации.
 */
    class GradientWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор окна градиентного метода
     * @param parent Родительский виджет (по умолчанию nullptr)
     */
    explicit GradientWindow(QWidget* parent = nullptr);

    /// Деструктор
    ~GradientWindow() override;

private:
    Ui::GradientWindow* ui; ///< Указатель на UI-компоненты
};

#endif