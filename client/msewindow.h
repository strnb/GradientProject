#ifndef MSEWINDOW_H
#define MSEWINDOW_H

#include <QMainWindow>
#include "../shared/msegraphwidget.h"
QT_BEGIN_NAMESPACE

namespace Ui
{
    class MSEWindow;
}

QT_END_NAMESPACE

class MSEWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MSEWindow(QWidget *parent = nullptr);
    ~MSEWindow() override;

private:
    Ui::MSEWindow *ui;
};

#endif