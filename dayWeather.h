#ifndef DAYWEATHER_H
#define DAYWEATHER_H

#include <QString>


class DayWeather
{
public:
    DayWeather();
public:
    QString m_day;
    QString m_date;

    QString m_wea_icon;
    QString m_wea;

    QString m_temp_high;
    QString m_temp_low;

    QString m_air_level;
    QString m_air_value;

    QString m_wind_direction;
    QString m_wind_level;
    QString m_wind_speed;

    QString printDayWeather();
};

#endif // DAYWEATHER_H
