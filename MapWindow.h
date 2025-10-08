#pragma once

#include <QMainWindow>

class QWebEngineView;

class MapWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MapWindow(QWidget* parent = nullptr);
    ~MapWindow() override;

public slots:
    void navigateToLocation(double latitude, double longitude);

private slots:
    void openLocationDialog();

private:
    void setupUI();
    void setupMenuBar();
    QWebEngineView* webView;
};
