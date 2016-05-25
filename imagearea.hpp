#ifndef IMAGEAREA_HPP
#define IMAGEAREA_HPP

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QTimer>
#include <random>

class ImageArea : public QWidget
{
    Q_OBJECT
    QTimer timer;
    QPoint coords;
    QPoint direction;
    std::normal_distribution<double> dist;
    std::uniform_int_distribution<int> dist2;
    std::uniform_real_distribution<double> dist3;
    std::default_random_engine gen;
    QImage img;
    QPixmap pix;
public:
    explicit ImageArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);
    double energy(double x, double y);
signals:

public slots:
    void on_timer();
};

#endif // IMAGEAREA_HPP
