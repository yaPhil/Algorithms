#include "painter.h"
#include "../Ray-Tracing/ray_tracer.h"
// —-----------------

PaintDevice::PaintDevice(QWidget *parent): QWidget(parent)
{
    //setStyleSheet("background: white");
    showrect = false;
    draw(true);
}

void PaintDevice::draw(bool x)
{


    // —------------------

    RayTraicer tracer("/home/philipp/GitHub/Algorithms/Ray-Tracer/scene01.rt");

    QImage img(tracer.getResX() + 1, tracer.getResY() + 1, QImage::Format_RGB32);
    pict = new QPicture;
    painter = new QPainter(pict);

    tracer.run(img, 4);
    //tracer.traceRays(-1, img, 1);
    // —------------------

//    for (uint i = 0; i < 100; ++i)
//    {
//        for (uint j = 0; j < 200; ++j)
//        {
//            //painter->setPen(QColor::fromRgbF(bitmap[i][j].r, bitmap[i][j].g, bitmap[i][j].b));
//            painter->setPen(Qt::blue);//QColor::fromRgbF(i /600.0, j/600.0, (i + j)/.700));
//            painter->drawPoint(i, j);
//        }
//    }

    // —----------------------------
painter->drawImage(0,0,img);
    painter->end();
    showrect = true;
    repaint();
    //delete [] bitmap;
}

void PaintDevice::paintEvent(QPaintEvent *ev)
{
    if (showrect)
    {
        QPainter p;
        p.begin(this);
        p.drawPicture(0, 0, *pict);
        p.end();
    }
    QWidget::paintEvent(ev);
}

void PaintDevice::mouseMoveEvent(QMouseEvent *ev)
{
    lbl->setText(QString::number(ev->x()) + " x " + QString::number(ev->y()));
}
