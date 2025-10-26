#include "LocationDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCoreApplication>
#include <QByteArray>
#include <QDir>
#include <QResource>

LocationDialog::LocationDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Choose Location");
    setModal(true);
    setupUI();
    populateCities();
}

void LocationDialog::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);

    // Search box
    auto* searchLabel = new QLabel("Search:", this);
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Type to filter cities...");

    mainLayout->addWidget(searchLabel);
    mainLayout->addWidget(searchEdit);
    mainLayout->addSpacing(10);

    // City selection
    auto* cityLabel = new QLabel("Select City:", this);
    cityComboBox = new QComboBox(this);
    // NEW: coordinates display
    coordinatesLabel = new QLabel("Coordinates: ", this);

    mainLayout->addWidget(cityLabel);
    mainLayout->addWidget(cityComboBox);
    mainLayout->addWidget(coordinatesLabel);
    mainLayout->addSpacing(20);

    // Buttons
    auto* buttonLayout = new QHBoxLayout();
    applyButton = new QPushButton("Apply", this);
    cancelButton = new QPushButton("Cancel", this);

    buttonLayout->addStretch();
    buttonLayout->addWidget(applyButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(applyButton, &QPushButton::clicked, this, &LocationDialog::onApplyClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(searchEdit, &QLineEdit::textChanged, this, &LocationDialog::onSearchTextChanged);
    connect(cityComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LocationDialog::onCityChanged);

    setMinimumWidth(400);
}

void LocationDialog::populateCities()
{
    // Load all city names from <exeDir>/file/countries.json; fallback to resource if needed.
    cities.clear();

    const QString exeDir = QCoreApplication::applicationDirPath();
    const QString fsPath = exeDir + "/files/countries.json";

    auto tryLoad = [&](const QString& path) -> bool {
        QFile file(path);
        if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return false;
        }

        const QByteArray data = file.readAll();
        file.close();

        QJsonParseError parseErr{};
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseErr);
        if (parseErr.error != QJsonParseError::NoError) {
            return false;
        }

        auto addCityName = [&](const QJsonValue& v) {
            if (v.isString()) {
                const QString s = v.toString().trimmed();
                if (!s.isEmpty()) {
                    cities.append(CityEntry{ s });
                }
            } else if (v.isObject()) {
                const QJsonObject o = v.toObject();
                QString name;
                if (o.contains("name") && o["name"].isString()) name = o["name"].toString();
                else if (o.contains("city") && o["city"].isString()) name = o["city"].toString();
                else if (o.contains("title") && o["title"].isString()) name = o["title"].toString();
                if (!name.isEmpty()) {
                    cities.append(CityEntry{ name });
                }
            }
        };

        auto extractFromCountryObject = [&](const QJsonObject& countryObj) {
            // If the country object has a "cities" array that can be strings or objects.
            if (countryObj.contains("cities") && countryObj["cities"].isArray()) {
                const QJsonArray arr = countryObj["cities"].toArray();
                for (const QJsonValue& cityVal : arr) {
                    addCityName(cityVal);
                }
            }
        };

        auto extractFromRootObject = [&](const QJsonObject& rootObj) {
            // Top-level cities array (strings or objects)
            if (rootObj.contains("cities") && rootObj["cities"].isArray()) {
                const QJsonArray citiesArr = rootObj["cities"].toArray();
                for (const QJsonValue& cityVal : citiesArr) {
                    addCityName(cityVal);
                }
            }
            // Countries array (each may contain a "cities" array)
            if (rootObj.contains("countries") && rootObj["countries"].isArray()) {
                const QJsonArray countriesArr = rootObj["countries"].toArray();
                for (const QJsonValue& countryVal : countriesArr) {
                    if (countryVal.isObject()) {
                        extractFromCountryObject(countryVal.toObject());
                    } else if (countryVal.isArray()) {
                        // Some structures might have an array of cities directly
                        for (const QJsonValue& cityVal : countryVal.toArray()) {
                            addCityName(cityVal);
                        }
                    }
                }
            }
        };

        if (doc.isArray()) {
            // Could be an array of city strings/objects or country objects with "cities"
            const QJsonArray rootArr = doc.array();
            for (const QJsonValue& item : rootArr) {
                if (item.isObject()) {
                    const QJsonObject obj = item.toObject();
                    if (obj.contains("cities")) {
                        extractFromCountryObject(obj);
                    } else {
                        addCityName(item);
                    }
                } else {
                    addCityName(item);
                }
            }
        } else if (doc.isObject()) {
            extractFromRootObject(doc.object());
        } else {
            return false;
        }

        return !cities.isEmpty();
    };

    bool loaded = tryLoad(fsPath);
    if (!loaded) {
        loaded = tryLoad(":/files/countries.json");
    }

    updateCityList();
    if (cityComboBox->count() > 0) {
        cityComboBox->setCurrentIndex(0);
        updateCoordinatesLabel(0);
    }
}


void LocationDialog::onApplyClicked()
{
    QString selectedCityName = cityComboBox->currentText();

    if (selectedCityName == "No cities found") {
        return;
    }

    // Find the city in the original list
    for (const auto& city : cities) {
        if (city.name == selectedCityName) {
            emit locationSelected(city.name, 0.0, 0.0);
            accept();
            return;
        }
    }
}

QString LocationDialog::getSelectedCity() const
{
    return cityComboBox->currentText();
}

QPair<double, double> LocationDialog::getSelectedCoordinates() const
{
    // Coordinates are no longer stored; return defaults
    return qMakePair(0.0, 0.0);
}

void LocationDialog::updateCityList(const QString& filter)
{
    cityComboBox->clear();

    for (const auto& city : cities) {
        if (filter.isEmpty() || city.name.contains(filter, Qt::CaseInsensitive)) {
            cityComboBox->addItem(city.name);
        }
    }

    if (cityComboBox->count() == 0) {
        cityComboBox->addItem("No cities found");
    }
}

void LocationDialog::onSearchTextChanged(const QString& text)
{
    updateCityList(text);
}

// NEW: show coordinates when city changes
void LocationDialog::updateCoordinatesLabel(int index)
{
    Q_UNUSED(index);
    coordinatesLabel->setText("Coordinates: ");
}

void LocationDialog::onCityChanged(int index)
{
    updateCoordinatesLabel(index);
}
