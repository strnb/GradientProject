#ifndef VIGENEREWINDOW_H
#define VIGENEREWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class VigenereWindow;
}
QT_END_NAMESPACE

class VigenereWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VigenereWindow(QWidget *parent = nullptr);
    ~VigenereWindow();

private:
    Ui::VigenereWindow *ui;
};

#endif