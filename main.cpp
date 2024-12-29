#include "animenestwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnimeNestWindow w;
    w.show();
    return a.exec();
}
