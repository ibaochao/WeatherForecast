#ifndef TODAYWEATHER_H
#define TODAYWEATHER_H

#include <QString>
#include <QDebug>

struct HoursWeather{
    QString time;
    QString wea_icon;
    QString temp;
};

class TodayWeather
{
public:
    TodayWeather();
public:
    QString m_cityName;
    QString m_dateDay;
    QString m_wea;
    QString m_wea_icon;
    QString m_temp;
    QString m_temp_low;
    QString m_temp_high;
    QString m_sunrise;
    QString m_sunset;
    QString m_updateTime;

    HoursWeather hoursWeather[6];

    QString m_tip;
    QString m_wind_direction;
    QString m_wind_level;
    QString m_wind_speed;
    QString m_air_level;
    QString m_air_value;
    QString m_humidity;
    QString m_visibility;
    QString m_pm25;

    QString printTodayWeather();
};

#endif // TODAYWEATHER_H
