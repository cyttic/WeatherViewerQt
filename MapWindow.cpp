#include "MapWindow.h"
#include "LocationDialog.h"
#include <QWebEngineView>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MapWindow::MapWindow(QWidget* parent)
    : QMainWindow(parent), webView(nullptr) {
    setupMenuBar();
    setupUI();
}

MapWindow::~MapWindow() = default;

void MapWindow::setupMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);

    // File menu
    QMenu* fileMenu = menuBar->addMenu(tr("&File"));
    QAction* exitAction = fileMenu->addAction(tr("E&xit"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Settings menu
    QMenu* settingsMenu = menuBar->addMenu(tr("&Settings"));
    QAction* locationAction = settingsMenu->addAction(tr("&Location..."));
    connect(locationAction, &QAction::triggered, this, &MapWindow::openLocationDialog);

    QAction* preferencesAction = settingsMenu->addAction(tr("&Preferences..."));
    // TODO: Connect to preferences dialog

    // Help menu
    QMenu* helpMenu = menuBar->addMenu(tr("&Help"));
    QAction* aboutAction = helpMenu->addAction(tr("&About..."));
    // TODO: Connect to about dialog

    setMenuBar(menuBar);
}

void MapWindow::openLocationDialog()
{
    LocationDialog* dialog = new LocationDialog(this);
    connect(dialog, &LocationDialog::locationSelected, this,
            [this](const QString& city, double latitude, double longitude) {
                navigateToLocation(latitude, longitude);
            });
    dialog->exec();
    dialog->deleteLater();
}

void MapWindow::navigateToLocation(double latitude, double longitude)
{
    QString script = QString(
        "map.setView([%1, %2], 12);"
    ).arg(latitude).arg(longitude);

    webView->page()->runJavaScript(script);
}

void MapWindow::setupUI() {
    webView = new QWebEngineView(this);

    const char* html = R"HTML(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8"/>
  <title>Map</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link
    rel="stylesheet"
    href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css"
    integrity="sha256-p4NxAoJBhIIN+hmNHrzRCf9tD/miZyoHS5obTRR9BMY="
    crossorigin=""/>
  <script
    src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"
    integrity="sha256-20nQCchB9co0qIjJZRGuk2/Z9VM+kNiyxNV1lvTlZBo="
    crossorigin=""></script>
  <style>
    body { margin: 0; padding: 0; }
    #map { height: 100vh; width: 100vw; }
  </style>
</head>
<body>
  <div id="map"></div>
  <script>
    var map = L.map('map').setView([51.505, -0.09], 13);
    L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
      maxZoom: 19,
      attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
    }).addTo(map);
  </script>
</body>
</html>
)HTML";

    webView->setHtml(html);
    setCentralWidget(webView);
}
