#ifndef SPLINEWINDOW_H
#define SPLINEWINDOW_H

#include <QMainWindow>
#include "splinegraphwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class SplineWindow;
}
QT_END_NAMESPACE

class SplineWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SplineWindow(QWidget* parent = nullptr);
    ~SplineWindow() override;

private:
    Ui::SplineWindow* ui;
};

#endif // SPLINEWINDOW_H