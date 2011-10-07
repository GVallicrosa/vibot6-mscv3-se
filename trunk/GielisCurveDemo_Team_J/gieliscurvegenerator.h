#ifndef GIELISCURVEGENERATOR_H
#define GIELISCURVEGENERATOR_H

#include <QPixmap>

#define PI 3.14

class GielisCurveGenerator
{
public:
    GielisCurveGenerator();

    static QPixmap generateGielisCurve(double a, double b, double m, double n1, double n2, double n3);
};

#endif // GIELISCURVEGENERATOR_H
