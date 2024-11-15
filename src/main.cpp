#include <QApplication>
#include <memory>

#include "MainWindow.h"

#ifdef _WIN32
#include "PingerWindows.h"
#else
#include "PingerLinux.h"
#endif

int main(int argc, char* argv[])
{
    const QApplication a(argc, argv);
#ifdef _WIN32
    MainWindow w(std::make_unique<PingerWindows>());
#else
    MainWindow w(std::make_unique<PingerLinux>());
#endif
    w.show();
    return QApplication::exec();
}
