#include "scoreboardmain.h"
#include <QApplication>
#ifdef Q_OS_WIN
    #include <direct.h>
#endif
#ifdef Q_OS_MACOS
    # include <unistd.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScoreboardMain w;
    w.show();

    return a.exec();
}
