/**
 * @file vigenerewindow.h
 * @brief Заголовочный файл окна шифра Виженера
 */

#ifndef VIGENEREWINDOW_H
#define VIGENEREWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class VigenereWindow; ///< UI-класс окна шифра Виженера
}
QT_END_NAMESPACE

/**
 * @class VigenereWindow
 * @brief Главное окно для работы с шифром Виженера
 *
 * Окно предоставляет интерфейс для шифрования и дешифрования
 * текста с использованием алгоритма Виженера.
 */
    class VigenereWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор окна шифра Виженера
     * @param parent Родительский виджет (по умолчанию nullptr)
     */
    explicit VigenereWindow(QWidget* parent = nullptr);

    /// Деструктор
    ~VigenereWindow();

private:
    Ui::VigenereWindow* ui; ///< Указатель на UI-компоненты
};

#endif