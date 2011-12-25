#include "test.h"

#include <QDebug>
#include <QRgb>
#include <QtCore/qmath.h>

Test::Test(QObject *parent)
{
    m_countFile = 0;
}

void Test::setData(const vector<Vector2d> &data) {
    m_data = data;
}

void Test::addPoint(const Vector2d &point) {
    m_data.push_back(point);
}

void Test::removePoint(int index) {
    vector<Vector2d>::iterator itr = m_data.begin();
    itr += index;
    m_data.erase(itr);
}

void Test::clearPoints() {
    m_data.clear();
}

const vector<Vector2d> & Test::getData() {
    return m_data;
}

const Vector2d & Test::getPoint(int index) {
    return m_data[index];
}

void Test::drawPoints(bool isOptimized) {
    // Start drawing points on a pixmap
    QPixmap image(500,500);
    QPainter painter(&image);
    painter.fillRect(image.rect(), Qt::white);

    painter.setPen(Qt::black);
    int size = m_data.size();
    for (int i = 0; i < size; i++) {
        painter.drawPoint(m_data[i][0], m_data[i][1]);
    }
    painter.end();
    // Drawing finished.
    if (isOptimized) {
        image.save("optimized_" + QString::number(m_countFile) + m_fileName);
        m_countFile++;
    }
}

void Test::startTesting() {
    readImage(m_fileName);

    /* instead of offsets
     * receive this vector of float
     * from rotationaloffset class
     */
    vector<float> offsets;
    offsets.push_back(0);
    offsets.push_back(3.14/4);
    offsets.push_back(3.14/2);
    offsets.push_back(3*3.14/4);

    m_rationalSuperShape2d = new RationalSuperShape2D();
    m_data = m_rationalSuperShape2d->Run(m_data, offsets, true, 1);
    drawPoints(true);

}

void Test::readImage(QString fileName) {
    qWarning() << "Reading image...";
    m_data.clear();
    QImage image(fileName);
    unsigned int width = image.width();
    unsigned int height = image.height();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            QRgb color = image.pixel(i,j);
            int red = qRed(color);
            int green = qGreen(color);
            int blue = qBlue(color);

            if (red > 200 && green > 200 && blue > 200) {
                m_data.push_back(Vector2d(double(i), double(j)));
            }
        }
    }
}
