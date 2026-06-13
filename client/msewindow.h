/**
 * @file msewindow.h
 * @brief Заголовочный файл окна среднеквадратичной ошибки (MSE)
 */

#ifndef MSEWINDOW_H
#define MSEWINDOW_H

#include <QMainWindow>
#include "../shared/msegraphwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MSEWindow; ///< UI-класс окна MSE
}
QT_END_NAMESPACE

/**
 * @class MSEWindow
 * @brief Главное окно для отображения среднеквадратичной ошибки
 *
 * Окно предоставляет интерфейс для вычисления и визуализации
 * среднеквадратичной ошибки (Mean Squared Error).
 */
    class MSEWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор окна MSE
     * @param parent Родительский виджет (по умолчанию nullptr)
     */
    explicit MSEWindow(QWidget* parent = nullptr);

    /// Деструктор
    ~MSEWindow() override;

private:
    Ui::MSEWindow* ui; ///< Указатель на UI-компоненты
};

#endif