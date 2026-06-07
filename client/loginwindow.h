#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow() override;

private slots:
    void onLoginButtonClicked();
    void onServerResponse(const QString& response);

private:
    Ui::LoginWindow* ui;
    bool m_loginAsAdmin;

    void setUiEnabled(bool enabled);
};

#endif // LOGINWINDOW_H