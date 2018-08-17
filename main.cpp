#include <QApplication>
#include "filetitlewidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileTitleWidget w;
    w.show();
    return a.exec();
}
