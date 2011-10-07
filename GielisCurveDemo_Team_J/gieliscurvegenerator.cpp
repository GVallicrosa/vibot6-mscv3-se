#include "gieliscurvegenerator.h"

#include <QPainter>
#include <QtCore/qmath.h>

GielisCurveGenerator::GielisCurveGenerator()
{
}


QPixmap GielisCurveGenerator::generateGielisCurve(double a, double b, double m, double n1, double n2, double n3)
{
    //:TODO: Take width and height as an argument, calculate center accordingly
    //Generate the gielis curve painted on QPixmap
    QPixmap img(400,400);
    QPainter painter(&img);

    //Background is white
    painter.fillRect(img.rect(), Qt::white);
    //The color of the curve is black
    painter.setPen(Qt::black);

    double center = 200;

    for (double i = 0; i <= PI*2; i += 0.0001) {
        double r = qPow((qPow(qAbs(qCos(m*i/4)/a), n2) + qPow(qAbs(qSin(m*i/4)/b), n3)), -(1/n1));
        double x = center + r * qCos(i)*100;
        double y = center + r * qSin(i)*100;
        painter.drawPoint(x,y);
    }

    //Each time a painter initialized, it has to be ended when it won't be used anymore.
    painter.end();

    //Return the pixmap on which the curve is drawn.
    return img;
}
