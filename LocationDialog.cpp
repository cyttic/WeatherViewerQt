#include "LocationDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

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

    mainLayout->addWidget(cityLabel);
    mainLayout->addWidget(cityComboBox);
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

    setMinimumWidth(400);
}

void LocationDialog::populateCities()
{
    // Comprehensive list of world cities with their coordinates
    cities = {
        // North America
        {"New York, USA", 40.7128, -74.0060},
        {"Los Angeles, USA", 34.0522, -118.2437},
        {"Chicago, USA", 41.8781, -87.6298},
        {"Houston, USA", 29.7604, -95.3698},
        {"Phoenix, USA", 33.4484, -112.0740},
        {"Philadelphia, USA", 39.9526, -75.1652},
        {"San Antonio, USA", 29.4241, -98.4936},
        {"San Diego, USA", 32.7157, -117.1611},
        {"Dallas, USA", 32.7767, -96.7970},
        {"San Jose, USA", 37.3382, -121.8863},
        {"Austin, USA", 30.2672, -97.7431},
        {"Jacksonville, USA", 30.3322, -81.6557},
        {"San Francisco, USA", 37.7749, -122.4194},
        {"Columbus, USA", 39.9612, -82.9988},
        {"Fort Worth, USA", 32.7555, -97.3308},
        {"Indianapolis, USA", 39.7684, -86.1581},
        {"Charlotte, USA", 35.2271, -80.8431},
        {"Seattle, USA", 47.6062, -122.3321},
        {"Denver, USA", 39.7392, -104.9903},
        {"Washington, USA", 38.9072, -77.0369},
        {"Boston, USA", 42.3601, -71.0589},
        {"Nashville, USA", 36.1627, -86.7816},
        {"Detroit, USA", 42.3314, -83.0458},
        {"Portland, USA", 45.5152, -122.6784},
        {"Las Vegas, USA", 36.1699, -115.1398},
        {"Memphis, USA", 35.1495, -90.0490},
        {"Louisville, USA", 38.2527, -85.7585},
        {"Baltimore, USA", 39.2904, -76.6122},
        {"Milwaukee, USA", 43.0389, -87.9065},
        {"Albuquerque, USA", 35.0844, -106.6504},
        {"Tucson, USA", 32.2226, -110.9747},
        {"Fresno, USA", 36.7378, -119.7871},
        {"Sacramento, USA", 38.5816, -121.4944},
        {"Kansas City, USA", 39.0997, -94.5786},
        {"Mesa, USA", 33.4152, -111.8315},
        {"Atlanta, USA", 33.7490, -84.3880},
        {"Omaha, USA", 41.2565, -95.9345},
        {"Miami, USA", 25.7617, -80.1918},
        {"Toronto, Canada", 43.6532, -79.3832},
        {"Montreal, Canada", 45.5017, -73.5673},
        {"Vancouver, Canada", 49.2827, -123.1207},
        {"Calgary, Canada", 51.0447, -114.0719},
        {"Edmonton, Canada", 53.5461, -113.4938},
        {"Ottawa, Canada", 45.4215, -75.6972},
        {"Winnipeg, Canada", 49.8951, -97.1384},
        {"Quebec City, Canada", 46.8139, -71.2080},
        {"Hamilton, Canada", 43.2557, -79.8711},
        {"Mexico City, Mexico", 19.4326, -99.1332},
        {"Guadalajara, Mexico", 20.6597, -103.3496},
        {"Monterrey, Mexico", 25.6866, -100.3161},
        {"Puebla, Mexico", 19.0414, -98.2063},
        {"Tijuana, Mexico", 32.5149, -117.0382},
        {"León, Mexico", 21.1619, -101.6827},
        {"Cancún, Mexico", 21.1619, -86.8515},

        // South America
        {"São Paulo, Brazil", -23.5505, -46.6333},
        {"Rio de Janeiro, Brazil", -22.9068, -43.1729},
        {"Brasília, Brazil", -15.8267, -47.9218},
        {"Salvador, Brazil", -12.9714, -38.5014},
        {"Fortaleza, Brazil", -3.7172, -38.5433},
        {"Belo Horizonte, Brazil", -19.9167, -43.9345},
        {"Manaus, Brazil", -3.1190, -60.0217},
        {"Curitiba, Brazil", -25.4284, -49.2733},
        {"Recife, Brazil", -8.0476, -34.8770},
        {"Buenos Aires, Argentina", -34.6037, -58.3816},
        {"Córdoba, Argentina", -31.4201, -64.1888},
        {"Rosario, Argentina", -32.9442, -60.6505},
        {"Mendoza, Argentina", -32.8895, -68.8458},
        {"Lima, Peru", -12.0464, -77.0428},
        {"Bogotá, Colombia", 4.7110, -74.0721},
        {"Medellín, Colombia", 6.2476, -75.5658},
        {"Cali, Colombia", 3.4516, -76.5320},
        {"Santiago, Chile", -33.4489, -70.6693},
        {"Valparaíso, Chile", -33.0472, -71.6127},
        {"Caracas, Venezuela", 10.4806, -66.9036},
        {"Quito, Ecuador", -0.1807, -78.4678},
        {"Guayaquil, Ecuador", -2.1709, -79.9224},
        {"La Paz, Bolivia", -16.5000, -68.1500},
        {"Santa Cruz, Bolivia", -17.8146, -63.1561},
        {"Asunción, Paraguay", -25.2637, -57.5759},
        {"Montevideo, Uruguay", -34.9011, -56.1645},

        // Europe
        {"London, UK", 51.5074, -0.1278},
        {"Manchester, UK", 53.4808, -2.2426},
        {"Birmingham, UK", 52.4862, -1.8904},
        {"Glasgow, UK", 55.8642, -4.2518},
        {"Liverpool, UK", 53.4084, -2.9916},
        {"Edinburgh, UK", 55.9533, -3.1883},
        {"Paris, France", 48.8566, 2.3522},
        {"Marseille, France", 43.2965, 5.3698},
        {"Lyon, France", 45.7640, 4.8357},
        {"Toulouse, France", 43.6047, 1.4442},
        {"Nice, France", 43.7102, 7.2620},
        {"Nantes, France", 47.2184, -1.5536},
        {"Berlin, Germany", 52.5200, 13.4050},
        {"Hamburg, Germany", 53.5511, 9.9937},
        {"Munich, Germany", 48.1351, 11.5820},
        {"Cologne, Germany", 50.9375, 6.9603},
        {"Frankfurt, Germany", 50.1109, 8.6821},
        {"Stuttgart, Germany", 48.7758, 9.1829},
        {"Düsseldorf, Germany", 51.2277, 6.7735},
        {"Dortmund, Germany", 51.5136, 7.4653},
        {"Essen, Germany", 51.4556, 7.0116},
        {"Leipzig, Germany", 51.3397, 12.3731},
        {"Rome, Italy", 41.9028, 12.4964},
        {"Milan, Italy", 45.4642, 9.1900},
        {"Naples, Italy", 40.8518, 14.2681},
        {"Turin, Italy", 45.0703, 7.6869},
        {"Palermo, Italy", 38.1157, 13.3615},
        {"Genoa, Italy", 44.4056, 8.9463},
        {"Bologna, Italy", 44.4949, 11.3426},
        {"Florence, Italy", 43.7696, 11.2558},
        {"Venice, Italy", 45.4408, 12.3155},
        {"Madrid, Spain", 40.4168, -3.7038},
        {"Barcelona, Spain", 41.3851, 2.1734},
        {"Valencia, Spain", 39.4699, -0.3763},
        {"Seville, Spain", 37.3891, -5.9845},
        {"Zaragoza, Spain", 41.6488, -0.8891},
        {"Málaga, Spain", 36.7213, -4.4214},
        {"Bilbao, Spain", 43.2630, -2.9350},
        {"Amsterdam, Netherlands", 52.3676, 4.9041},
        {"Rotterdam, Netherlands", 51.9225, 4.47917},
        {"The Hague, Netherlands", 52.0705, 4.3007},
        {"Brussels, Belgium", 50.8503, 4.3517},
        {"Antwerp, Belgium", 51.2194, 4.4025},
        {"Vienna, Austria", 48.2082, 16.3738},
        {"Zurich, Switzerland", 47.3769, 8.5417},
        {"Geneva, Switzerland", 46.2044, 6.1432},
        {"Copenhagen, Denmark", 55.6761, 12.5683},
        {"Stockholm, Sweden", 59.3293, 18.0686},
        {"Oslo, Norway", 59.9139, 10.7522},
        {"Helsinki, Finland", 60.1699, 24.9384},
        {"Warsaw, Poland", 52.2297, 21.0122},
        {"Krakow, Poland", 50.0647, 19.9450},
        {"Prague, Czech Republic", 50.0755, 14.4378},
        {"Budapest, Hungary", 47.4979, 19.0402},
        {"Bucharest, Romania", 44.4268, 26.1025},
        {"Sofia, Bulgaria", 42.6977, 23.3219},
        {"Athens, Greece", 37.9838, 23.7275},
        {"Thessaloniki, Greece", 40.6401, 22.9444},
        {"Lisbon, Portugal", 38.7223, -9.1393},
        {"Porto, Portugal", 41.1579, -8.6291},
        {"Dublin, Ireland", 53.3498, -6.2603},
        {"Moscow, Russia", 55.7558, 37.6173},
        {"Saint Petersburg, Russia", 59.9311, 30.3609},
        {"Novosibirsk, Russia", 55.0084, 82.9357},
        {"Yekaterinburg, Russia", 56.8389, 60.6057},
        {"Kazan, Russia", 55.8304, 49.0661},
        {"Kiev, Ukraine", 50.4501, 30.5234},
        {"Minsk, Belarus", 53.9006, 27.5590},

        // Asia
        {"Tokyo, Japan", 35.6762, 139.6503},
        {"Yokohama, Japan", 35.4437, 139.6380},
        {"Osaka, Japan", 34.6937, 135.5023},
        {"Nagoya, Japan", 35.1815, 136.9066},
        {"Sapporo, Japan", 43.0642, 141.3469},
        {"Fukuoka, Japan", 33.5904, 130.4017},
        {"Kobe, Japan", 34.6901, 135.1955},
        {"Kyoto, Japan", 35.0116, 135.7681},
        {"Beijing, China", 39.9042, 116.4074},
        {"Shanghai, China", 31.2304, 121.4737},
        {"Guangzhou, China", 23.1291, 113.2644},
        {"Shenzhen, China", 22.5431, 114.0579},
        {"Chengdu, China", 30.5728, 104.0668},
        {"Chongqing, China", 29.4316, 106.9123},
        {"Tianjin, China", 39.3434, 117.3616},
        {"Wuhan, China", 30.5928, 114.3055},
        {"Hangzhou, China", 30.2741, 120.1551},
        {"Xi'an, China", 34.3416, 108.9398},
        {"Nanjing, China", 32.0603, 118.7969},
        {"Hong Kong", 22.3193, 114.1694},
        {"Seoul, South Korea", 37.5665, 126.9780},
        {"Busan, South Korea", 35.1796, 129.0756},
        {"Incheon, South Korea", 37.4563, 126.7052},
        {"Daegu, South Korea", 35.8714, 128.6014},
        {"Mumbai, India", 19.0760, 72.8777},
        {"Delhi, India", 28.7041, 77.1025},
        {"Bangalore, India", 12.9716, 77.5946},
        {"Hyderabad, India", 17.3850, 78.4867},
        {"Ahmedabad, India", 23.0225, 72.5714},
        {"Chennai, India", 13.0827, 80.2707},
        {"Kolkata, India", 22.5726, 88.3639},
        {"Pune, India", 18.5204, 73.8567},
        {"Jaipur, India", 26.9124, 75.7873},
        {"Surat, India", 21.1702, 72.8311},
        {"Lucknow, India", 26.8467, 80.9462},
        {"Bangkok, Thailand", 13.7563, 100.5018},
        {"Singapore", 1.3521, 103.8198},
        {"Kuala Lumpur, Malaysia", 3.1390, 101.6869},
        {"Jakarta, Indonesia", -6.2088, 106.8456},
        {"Surabaya, Indonesia", -7.2575, 112.7521},
        {"Bandung, Indonesia", -6.9175, 107.6191},
        {"Manila, Philippines", 14.5995, 120.9842},
        {"Quezon City, Philippines", 14.6760, 121.0437},
        {"Hanoi, Vietnam", 21.0285, 105.8542},
        {"Ho Chi Minh City, Vietnam", 10.8231, 106.6297},
        {"Taipei, Taiwan", 25.0330, 121.5654},
        {"Karachi, Pakistan", 24.8607, 67.0011},
        {"Lahore, Pakistan", 31.5204, 74.3587},
        {"Islamabad, Pakistan", 33.6844, 73.0479},
        {"Dhaka, Bangladesh", 23.8103, 90.4125},
        {"Colombo, Sri Lanka", 6.9271, 79.8612},
        {"Yangon, Myanmar", 16.8661, 96.1951},
        {"Phnom Penh, Cambodia", 11.5564, 104.9282},
        {"Vientiane, Laos", 17.9757, 102.6331},
        {"Tel Aviv, Israel", 32.0853, 34.7818},
        {"Jerusalem, Israel", 31.7683, 35.2137},
        {"Dubai, UAE", 25.2048, 55.2708},
        {"Abu Dhabi, UAE", 24.4539, 54.3773},
        {"Riyadh, Saudi Arabia", 24.7136, 46.6753},
        {"Jeddah, Saudi Arabia", 21.4858, 39.1925},
        {"Doha, Qatar", 25.2854, 51.5310},
        {"Kuwait City, Kuwait", 29.3759, 47.9774},
        {"Muscat, Oman", 23.5880, 58.3829},
        {"Manama, Bahrain", 26.2285, 50.5860},
        {"Amman, Jordan", 31.9454, 35.9284},
        {"Beirut, Lebanon", 33.8938, 35.5018},
        {"Damascus, Syria", 33.5138, 36.2765},
        {"Baghdad, Iraq", 33.3152, 44.3661},
        {"Tehran, Iran", 35.6892, 51.3890},
        {"Istanbul, Turkey", 41.0082, 28.9784},
        {"Ankara, Turkey", 39.9334, 32.8597},
        {"Izmir, Turkey", 38.4237, 27.1428},
        {"Tashkent, Uzbekistan", 41.2995, 69.2401},
        {"Almaty, Kazakhstan", 43.2220, 76.8512},
        {"Baku, Azerbaijan", 40.4093, 49.8671},
        {"Tbilisi, Georgia", 41.7151, 44.8271},
        {"Yerevan, Armenia", 40.1792, 44.4991},

        // Africa
        {"Cairo, Egypt", 30.0444, 31.2357},
        {"Alexandria, Egypt", 31.2001, 29.9187},
        {"Lagos, Nigeria", 6.5244, 3.3792},
        {"Kano, Nigeria", 12.0022, 8.5920},
        {"Ibadan, Nigeria", 7.3775, 3.9470},
        {"Kinshasa, DR Congo", -4.4419, 15.2663},
        {"Luanda, Angola", -8.8390, 13.2894},
        {"Johannesburg, South Africa", -26.2041, 28.0473},
        {"Cape Town, South Africa", -33.9249, 18.4241},
        {"Durban, South Africa", -29.8587, 31.0218},
        {"Pretoria, South Africa", -25.7479, 28.2293},
        {"Nairobi, Kenya", -1.2864, 36.8172},
        {"Mombasa, Kenya", -4.0435, 39.6682},
        {"Addis Ababa, Ethiopia", 9.0320, 38.7469},
        {"Dar es Salaam, Tanzania", -6.7924, 39.2083},
        {"Khartoum, Sudan", 15.5007, 32.5599},
        {"Algiers, Algeria", 36.7538, 3.0588},
        {"Casablanca, Morocco", 33.5731, -7.5898},
        {"Rabat, Morocco", 34.0209, -6.8416},
        {"Marrakech, Morocco", 31.6295, -7.9811},
        {"Tunis, Tunisia", 36.8065, 10.1815},
        {"Accra, Ghana", 5.6037, -0.1870},
        {"Abidjan, Côte d'Ivoire", 5.3600, -4.0083},
        {"Dakar, Senegal", 14.7167, -17.4677},
        {"Kampala, Uganda", 0.3476, 32.5825},
        {"Lusaka, Zambia", -15.3875, 28.3228},
        {"Harare, Zimbabwe", -17.8252, 31.0335},
        {"Maputo, Mozambique", -25.9655, 32.5832},
        {"Antananarivo, Madagascar", -18.8792, 47.5079},
        {"Tripoli, Libya", 32.8872, 13.1913},

        // Oceania
        {"Sydney, Australia", -33.8688, 151.2093},
        {"Melbourne, Australia", -37.8136, 144.9631},
        {"Brisbane, Australia", -27.4698, 153.0251},
        {"Perth, Australia", -31.9505, 115.8605},
        {"Adelaide, Australia", -34.9285, 138.6007},
        {"Gold Coast, Australia", -28.0167, 153.4000},
        {"Canberra, Australia", -35.2809, 149.1300},
        {"Auckland, New Zealand", -36.8485, 174.7633},
        {"Wellington, New Zealand", -41.2865, 174.7762},
        {"Christchurch, New Zealand", -43.5321, 172.6362},
        {"Suva, Fiji", -18.1416, 178.4419},
        {"Port Moresby, Papua New Guinea", -9.4438, 147.1803}
    };

    updateCityList();
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
            emit locationSelected(city.name, city.latitude, city.longitude);
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
    int index = cityComboBox->currentIndex();
    if (index >= 0 && index < cities.size()) {
        return qMakePair(cities[index].latitude, cities[index].longitude);
    }
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
