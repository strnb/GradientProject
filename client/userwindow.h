#ifndef GRADIENTPROJECT_USERWINDOW_H
#define GRADIENTPROJECT_USERWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class UserWindow; }
QT_END_NAMESPACE

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget* parent = nullptr);
    ~UserWindow() override;

private:
    Ui::UserWindow* ui;
};

#endif //GRADIENTPROJECT_USERWINDOW_H