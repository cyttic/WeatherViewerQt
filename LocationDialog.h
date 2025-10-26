#ifndef LOCATIONDIALOG_H
#define LOCATIONDIALOG_H
#endif

#include <QDialog>
#include <QString>
#include <QPair>

class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;

struct CityEntry {
    QString name;
    // coordinates removed
};

class LocationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LocationDialog(QWidget* parent = nullptr);

    signals:
        void locationSelected(const QString& cityName, double latitude, double longitude);

public:
    QString getSelectedCity() const;
    QPair<double, double> getSelectedCoordinates() const;

private:
    void setupUI();
    void populateCities();
    void updateCityList(const QString& filter = "");
    void updateCoordinatesLabel(int index);

private slots:
    void onApplyClicked();
    void onSearchTextChanged(const QString& text);
    void onCityChanged(int index);

private:
    QLineEdit* searchEdit;
    QComboBox* cityComboBox;
    QPushButton* applyButton;
    QPushButton* cancelButton;
    QLabel* coordinatesLabel; // NEW: displays coordinates of the selected city

    QVector<CityEntry> cities;
};