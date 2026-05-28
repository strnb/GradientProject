#ifndef SHAWINDOW_H
#define SHAWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class ShaWindow;
}

QT_END_NAMESPACE

class ShaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShaWindow(QWidget *parent = nullptr);
    ~ShaWindow() override;

private:
    Ui::ShaWindow *ui;
};

#endif // SHAWINDOW_H