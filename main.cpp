//version 1.1.0-linux
#include <QApplication>
#include "Window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}
