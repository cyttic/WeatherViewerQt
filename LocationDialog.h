#pragma once

#include <QDialog>
#include <QString>
#include <QPair>

class QComboBox;
class QPushButton;
class QLineEdit;

class LocationDialog : public QDialog {
    Q_OBJECT

public:
    explicit LocationDialog(QWidget* parent = nullptr);
    ~LocationDialog() override = default;

    QString getSelectedCity() const;
    QPair<double, double> getSelectedCoordinates() const;

signals:
    void locationSelected(const QString& city, double latitude, double longitude);

private slots:
    void onApplyClicked();
    void onSearchTextChanged(const QString& text);

private:
    void setupUI();
    void populateCities();
    void updateCityList(const QString& filter = QString());

    QLineEdit* searchEdit;
    QComboBox* cityComboBox;
    QPushButton* applyButton;
    QPushButton* cancelButton;

    struct CityData {
        QString name;
        double latitude;
        double longitude;
    };

    QVector<CityData> cities;
};
