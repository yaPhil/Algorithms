#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Ray-Tracing/ray_tracer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    (void) event;
    QPainter painter(this);

    QImage img(700, 700, QImage::Format_RGB32);
    RayTraicer tracer(Vector(0, 0, 0), Vector(2, -1, 1), Vector(2, 1, 1), Vector(2, -1, -1), 699, 699);
    //tracer.addObject(new Sphere(Color(0.5, 0, 0.5, 0), Vector(5, 0, 0), 1));
    tracer.addObject(new Sphere(Color(0, 0, 1, 0), Vector(5, 0, 0), 1));
    tracer.addObject(new Triangle(Color(1, 0, 0, 0), Vector(4.0, -1, 1), Vector(4.0, 1, 1), Vector(4.7, 0.2, -2)));
    //tracer.traceRays(-1, img);
    tracer.run(img);

//    for (int i = 0; i < img.width(); ++i)
//        for (int j = 0; j < img.height(); ++j)
//            img.setPixelColor(i, j, QColor(255, i * 255 / img.width() , j * 255 / img.height()));

    painter.drawImage(30, 30, img);
}

MainWindow::~MainWindow()
{
    delete ui;
}
