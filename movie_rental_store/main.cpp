#include <QApplication>
#include "GUI/GUI.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    FileRepo repo { "file.txt" };
    MovieValidator val;
    Service serv{ repo, val };
    MovieStoreGUI gui { serv };

    gui.show();
    return QApplication::exec();
}
