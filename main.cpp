#include <QApplication>
#include "MapWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MapWindow window;
    window.setWindowTitle("WeatherViewerQt");
    window.resize(800, 600);
    window.show();

    return app.exec();
}
