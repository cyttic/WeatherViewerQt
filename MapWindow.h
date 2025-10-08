#pragma once

#include <QMainWindow>

class QWebEngineView;

class MapWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MapWindow(QWidget* parent = nullptr);
    ~MapWindow() override;

private:
    void setupUI();
    QWebEngineView* webView;
};
