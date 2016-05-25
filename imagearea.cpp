#include "imagearea.hpp"
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QRadialGradient>

ImageArea::ImageArea(QWidget *parent) : QWidget(parent)
  , dist(0, 1)
  , direction(1,1)
  , dist2(-1,1)
  , dist3(0,5)
  , img(this->width(), this->height(), QImage::Format_ARGB32)
  , pix(this->width(), this->height())
{
    coords.rx() = 200;
    coords.ry() = 200;
    connect(&timer, SIGNAL(timeout()), this, SLOT(on_timer()));
    QPainter p(&pix);
    for(int i = 0 ; i < this->width(); ++i){
        for(int j = 0; j < this->height(); ++j){
            auto e = energy(i, j);
            //img.setPixel(i,j,qRgba(255/e,0,0,255));
            p.setPen(qRgba(255/e,0,0,255));
            p.drawPoint(i, j);
        }
    }
    timer.start(1000);
}

void ImageArea::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.drawPixmap(0,0,pix);
    painter.setBrush(QBrush(Qt::green));
    painter.drawEllipse(coords, 25, 25);
    e->accept();
}

void ImageArea::resizeEvent(QResizeEvent *)
{
    //img = img.scaled(this->size());
    pix = pix.scaled(this->size());
    QPainter p(&pix);
    for(int i = 0 ; i < this->width(); ++i){
        for(int j = 0; j < this->height(); ++j){
            auto e = energy(i, j);
            p.setPen(qRgba(e*(255/16),0,255-e*(255/16),255));
            p.drawPoint(i,j);
            //img.setPixel(i,j,qRgba(e*(255/16),0,255-e*(255/16),255));
        }
    }
}

void ImageArea::mousePressEvent(QMouseEvent *e)
{
    coords = e->pos();
}

double ImageArea::energy(double x, double y)
{
    x = (x - this->width()/2.0)/(this->width()/6.0);
    y = (y - this->height()/2.0)/(this->height()/6.0);
    return 3*(1-x)*(1-x)*exp(-(x*x)-(y+1)*(y+1))-10*(0.2*x-x*x*x-y*y*y*y*y)*exp(-(x*x)-y*y)-1./3*exp(-(x+1)*(x+1)-y*y)+7;
}

void ImageArea::on_timer()
{
    int x = 0;
    int y = 0;
    if(abs(dist(gen)) > 1){
        direction.rx() = dist2(gen);
        direction.ry() = dist2(gen);
    }
    do{
        x = coords.x() + dist3(gen)*direction.x();
        y = coords.y() + dist3(gen)*direction.y();
    }while(x < 0 || x >= this->width() || y < 0 || y >= this->height());

    auto r1 = pix.toImage().pixelColor(coords.x(), coords.y()).red();
    auto r2 = pix.toImage().pixelColor(x, y).red();

    //if(energy(coords.x(), coords.y()) <= energy(x,y) || abs(dist(gen)) > 1){
    if(r1 <= r2){
        coords = {x, y};
    }

    QPainter p(&pix);
    QPen pen(QColor(0,0,0,0));
    pen.setWidth(0);
    p.setPen(pen);
    QRadialGradient grad(coords, 25);
    grad.setColorAt(0, QColor(0, 0, 0, 10));
    grad.setColorAt(1, QColor(0, 0, 0, 0));
    p.setBrush(QBrush(grad));
    p.drawEllipse(coords,25,25);

    repaint();
    timer.start(10);
}
