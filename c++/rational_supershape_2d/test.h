#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QPixmap>
#include <QPainter>

#include "irationalsupershape2d.h"
#include "rationalsupershape2d.h"
//#include "Eigen/StdVector"

class Test: public QObject
{
    Q_OBJECT
public:
    Test(QObject *parent=0);
    Test(QString fileName, QObject *parent=0);

    void setData(const vector<Vector2d> &data);
    void addPoint(const Vector2d &point);
    void removePoint(int index);
    void clearPoints();

    void readImage(QString fileName);

    const vector<Vector2d> & getData();
    const Vector2d & getPoint(int index);

    void drawPoints(bool isOptimized);
    void startTesting();


    inline void setFileName(QString fileName) {
        m_fileName = fileName;
    }

signals:
    void drawImage(const QPixmap &image, bool isOptimized);

private:
    vector<Vector2d> m_data;
    double m_err;
    IRationalSuperShape2D *m_rationalSuperShape2d;

    QString m_fileName;
    int m_countFile;
};

#endif // TEST_H
