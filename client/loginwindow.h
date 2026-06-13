/**
 * @file loginwindow.h
 * @brief Заголовочный файл окна авторизации
 */

#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; } ///< UI-класс окна авторизации
QT_END_NAMESPACE

/**
 * @class LoginWindow
 * @brief Окно авторизации пользователя
 *
 * Окно предоставляет форму входа для пользователей и администраторов.
 * Обрабатывает запросы к серверу для проверки учетных данных.
 */
    class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор окна авторизации
     * @param parent Родительский виджет (по умолчанию nullptr)
     */
    explicit LoginWindow(QWidget* parent = nullptr);

    /// Деструктор
    ~LoginWindow() override;

private slots:
    /// Обработчик нажатия кнопки входа
    void onLoginButtonClicked();

    /**
     * @brief Обработчик ответа от сервера
     * @param response Ответ сервера в виде строки
     */
    void onServerResponse(const QString& response);

private:
    Ui::LoginWindow* ui;      ///< Указатель на UI-компоненты
    bool m_loginAsAdmin;      ///< Флаг входа в режиме администратора

    /**
     * @brief Установить состояние доступности UI-компонентов
     * @param enabled true - компоненты включены, false - отключены
     */
    void setUiEnabled(bool enabled);
};

#endif // LOGINWINDOW_H