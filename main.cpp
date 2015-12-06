#include "shop.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
    QApplication a(argc, argv);

    Shop shop;
    shop.Login();

    return a.exec();
}
