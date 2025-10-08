#include <QApplication>
#include <QMainWindow>
#include <QLabel>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("WeatherViewerQt");

    auto* label = new QLabel("Hello from Qt!");
    label->setAlignment(Qt::AlignCenter);
    window.setCentralWidget(label);

    window.resize(480, 320);
    window.show();

    return app.exec();
}