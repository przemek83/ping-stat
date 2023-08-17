#include <QApplication>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
    const QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
