#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPicture>
#include <QMouseEvent>
#include <QVBoxLayout>

#include "painter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *ev);
    ~MainWindow();

    PaintDevice *pd;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
