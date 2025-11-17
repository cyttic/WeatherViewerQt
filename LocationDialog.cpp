#include "LocationDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCompleter>
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
#include <QStringListModel>
#include <QSortFilterProxyModel>

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
    cityComboBox->setEditable(true);
    cityComboBox->setInsertPolicy(QComboBox::NoInsert);

    // Set up completer for filtering
    auto* completer = cityComboBox->completer();
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);

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


    QFile file(exeDir + "/files/worldcities.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file!";
        return;
    }

    QTextStream in(&file);
    bool firstLine = true;

    QStringListModel * model = new QStringListModel;

    while (!in.atEnd()) {
        QString line = in.readLine();

        // Skip header line if needed
        if (firstLine) {
            firstLine = false;
            continue;
        }

        // Split by comma
        QStringList fields = line.split(',');
        //Agenda:
        //"city","city_ascii","lat","lng","country","iso2","iso3","admin_name","capital","population","id"

        if (fields.size() < 5) continue; // skip incomplete lines

        QString name = QString(fields[1].begin()+1, fields[1].length()-2);
        double latitude = parseDouble(fields[2]);

        double longitude = parseDouble(fields[3]);
        QString country = QString(fields[4].begin()+1, fields[4].length()-2);
        QString id = fields[10];

        cities.push_back(Country{name, country, latitude, longitude, id.toInt()});
        model->setData(model->index(model->rowCount() - 1, 0), name, Qt::DisplayRole);

        qDebug() << id << name << country << latitude << longitude;
    }

    QSortFilterProxyModel * proxy = new QSortFilterProxyModel;
    proxy->setSourceModel(model);
    cityComboBox->setModel(proxy);
    cityComboBox->setCompleter(0);

    file.close();

}

double LocationDialog::parseDouble(const QString &text) {
    QString t = text.trimmed();
    t.remove('\"');           // remove quotes
    return t.toDouble();
}


void LocationDialog::onApplyClicked()
{
    QString selectedCityName = cityComboBox->currentText();

    if (selectedCityName == "No cities found") {
        return;
    }

    // Find the city in the original list
    for ( auto& city : cities) {
        if (city.getName() == selectedCityName) {
            emit locationSelected(city.getName(), city.getLat(), city.getLong());
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

    for ( auto& city : cities) {
        if (filter.isEmpty() || city.getName().contains(filter, Qt::CaseInsensitive)) {
            cityComboBox->addItem(city.getName());
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
