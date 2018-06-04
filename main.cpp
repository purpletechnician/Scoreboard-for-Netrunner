#include "scoreboardmain.h"
#include <QApplication>
#ifdef Q_OS_WIN
    #include <direct.h>
#elif
    # include <unistd.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScoreboardMain w;
    mkdir(".\\Output\\");
    mkdir(".\\Update\\");
    w.show();

    return a.exec();
}
