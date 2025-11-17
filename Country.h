//
// Created by cyttic on 11/17/25.
//

#ifndef WEATHERVIEWERQT_COUNTRY_H
#define WEATHERVIEWERQT_COUNTRY_H


#include <QString>

class Country {
public:
    Country(const QString &name, const QString &countryName, double lat, double lon, int id);
    const QString getCountryCode();
    const QString getName();
    double getLat();
    double getLong();
    int getId();
private:
    QString name;
    double latitude;
    double longitude;
    QString country;
    int id;
};


#endif //WEATHERVIEWERQT_COUNTRY_H