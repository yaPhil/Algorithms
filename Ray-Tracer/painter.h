#ifndef PAINTER_H
#define PAINTER_H
#include<QWidget>
#include<QLabel>
#include<QMouseEvent>

class PaintDevice : public QWidget
{
Q_OBJECT
public:
    PaintDevice(QWidget* parent = 0);
    virtual void paintEvent(QPaintEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);

    bool showrect;

    QPicture *pict;
    QPainter *painter;

    QLabel * lbl;
    public slots:
    void draw(bool x);
};

#endif // PAINTER_H
