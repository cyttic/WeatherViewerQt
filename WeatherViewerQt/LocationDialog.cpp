void LocationDialog::populateCities()
{
    QFile file(":/files/countries+cities.json");
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            cities.clear();
            for (const QJsonValue& val : doc.array()) {
                if (!val.isObject()) continue;
