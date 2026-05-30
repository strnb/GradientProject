#ifndef GRADIENTWINDOW_H
#define GRADIENTWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class GradientWindow;
}
QT_END_NAMESPACE

class GradientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GradientWindow(QWidget* parent = nullptr);
    ~GradientWindow() override;

private:
    Ui::GradientWindow* ui;
};

#endif