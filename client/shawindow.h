/**
 * @file shawindow.h
 * @brief Заголовочный файл окна SHA-хэширования
 */

#ifndef SHAWINDOW_H
#define SHAWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ShaWindow; ///< UI-класс окна SHA
}
QT_END_NAMESPACE

/**
 * @class ShaWindow
 * @brief Главное окно для вычисления SHA-хэшей
 *
 * Окно предоставляет интерфейс для вычисления хэш-сумм
 * с использованием алгоритмов семейства SHA.
 */
    class ShaWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор окна SHA
     * @param parent Родительский виджет (по умолчанию nullptr)
     */
    explicit ShaWindow(QWidget* parent = nullptr);

    /// Деструктор
    ~ShaWindow() override;

private:
    Ui::ShaWindow* ui; ///< Указатель на UI-компоненты
};

#endif // SHAWINDOW_H