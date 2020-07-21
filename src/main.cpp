#include "notepad.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Notepad w;

    w.setWindowTitle("Notepad");
    w.showMaximized();

    return a.exec();
}
