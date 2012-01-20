#include <QtGui/QApplication>
#include "gui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Vibot6-MSCV3");
    QCoreApplication::setOrganizationDomain("https://sites.google.com/site/mcvibot2011sep/home");
    QCoreApplication::setApplicationName("Road Sign Detection");

    Gui w;
    w.show();

    return a.exec();
}
