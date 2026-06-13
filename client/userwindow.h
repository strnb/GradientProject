/**
 * @file userwindow.h
 * @brief Заголовочный файл окна пользователя
 */

#ifndef GRADIENTPROJECT_USERWINDOW_H
#define GRADIENTPROJECT_USERWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class UserWindow; } ///< UI-класс окна пользователя
QT_END_NAMESPACE

/**
 * @class UserWindow
 * @brief Главное окно пользователя
 *
 * Окно предоставляет стандартному пользователю доступ
 * к функциональности приложения в соответствии с его правами.
 */
    class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор окна пользователя
     * @param parent Родительский виджет (по умолчанию nullptr)
     */
    explicit UserWindow(QWidget* parent = nullptr);

    /// Деструктор
    ~UserWindow() override;

private:
    Ui::UserWindow* ui; ///< Указатель на UI-компоненты
};

#endif // GRADIENTPROJECT_USERWINDOW_H