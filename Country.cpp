//
// Created by cyttic on 11/17/25.
//

#include "Country.h"

Country::Country(const QString &countryCode, const QString &countryName, double lat, double lon, int id) {
    this->country  = countryName;
    this->name = countryCode;
    this->latitude = lat;
    this->longitude = lon;
    this->id = id;

}

const QString Country::getCountryCode() {
    return this->country;
}

const QString Country::getName() {
    return this->name;
}

double Country::getLat() {
    return this->latitude;
}

double Country::getLong() {
    return this->longitude;
}

int Country::getId() {
    return this->id;
}