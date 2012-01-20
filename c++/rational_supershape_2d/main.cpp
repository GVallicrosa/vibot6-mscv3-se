#include <QCoreApplication>
#include "test.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Test testObject;
    testObject.setFileName("../rational_supershape_2d/image.jpg");
    testObject.startTesting();

    return app.exec();
}
