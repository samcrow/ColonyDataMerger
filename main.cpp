#include <QApplication>
#include "colonydatamerger.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColonyDataMerger w;
    w.show();
    
    return a.exec();
}
