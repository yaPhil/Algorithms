#include "painter.h"
#include "../Ray-Tracing/ray_tracer.h"
#include "ctime"
#include <iostream>
// —-----------------

PaintDevice::PaintDevice(QWidget *parent): QWidget(parent)
{
    //setStyleSheet("background: white");
    showrect = false;
    draw(true);
}

std::vector<SolidObject*> piramid(Vector loc, int cnt, long double edge) {
    std::vector<SolidObject*> lst;
    if (cnt > 5) {
        edge = edge / 2.;
        std::vector<SolidObject*> vect(piramid(loc, cnt / 5, edge));
        for (size_t i = 0; i < vect.size(); ++i)
            lst.push_back(vect[i]);
        vect = std::move(piramid(loc + Vector{ edge, 0., 0. }, cnt / 5, edge));
        for (size_t i = 0; i < vect.size(); ++i)
            lst.push_back(vect[i]);
        vect = std::move(piramid(loc + Vector{ 0., edge, 0. }, cnt / 5, edge));
        for (size_t i = 0; i < vect.size(); ++i)
            lst.push_back(vect[i]);
        vect = std::move(piramid(loc + Vector{ edge, edge, 0. }, cnt / 5, edge));
        for (size_t i = 0; i < vect.size(); ++i)
            lst.push_back(vect[i]);
        vect = std::move(piramid(loc + Vector{ edge / 2., edge / 2., edge }, cnt / 5, edge));
        for (size_t i = 0; i < vect.size(); ++i)
            lst.push_back(vect[i]);
    }
    else {
        lst.push_back(new Triangle(Color(1, 1, 1, 1, 0, 1), loc, loc + Vector( edge,0.,0. ), loc + Vector( edge / 2,edge / 2,edge )));
        lst.push_back(new Triangle(Color(1, 1, 1, 1, 0, 1), loc, loc + Vector( 0.,edge,0. ), loc + Vector( edge / 2,edge / 2,edge )));
        lst.push_back(new Triangle(Color(1, 1, 1, 1, 0, 1), loc + Vector( edge,edge,0 ), loc + Vector( edge,edge,0 ) + Vector( -edge,0.,0. ), loc + Vector( edge,edge,0 ) + Vector( -edge / 2,-edge / 2,edge )));
        lst.push_back(new Triangle(Color(1, 1, 1, 1, 0, 1), loc + Vector( edge, edge, 0 ), loc + Vector( edge, edge, 0 ) + Vector( 0.,-edge,0. ), loc + Vector( edge, edge, 0 ) + Vector( -edge / 2,-edge / 2,edge )));
    }
    return lst;
}

void PaintDevice::draw(bool x)
{


    // —------------------

    //RayTraicer tracer("/home/philipp/GitHub/Algorithms/Ray-Tracer/scene01.rt");
    RayTraicer tracer(Vector(0, 0, 0), Vector(2, -4, 4), Vector(2, 4, 4), Vector(2, -4, -4), 699, 699);
    QImage img(tracer.getResX() + 1, tracer.getResY() + 1, QImage::Format_RGB32);
    pict = new QPicture;
    painter = new QPainter(pict);
    auto pol = piramid(Vector(2.2, -2, -1.5), 100000, 8);

    for(int i = 0; i < pol.size(); ++i) {
        tracer.addObject(pol[i]);
    }
    tracer.addLight(LightSource(Vector(2.5, 0, 5), 20));
    tracer.addLight(LightSource(Vector(2, 1, 2), 10));
    tracer.buildTree();
    long double t = std::time(0);
    tracer.run(img, 3);
    //tracer.traceRays(-1, img, 1);
    t = std::time(0) - t;
    std::cout << t << "\n";
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
