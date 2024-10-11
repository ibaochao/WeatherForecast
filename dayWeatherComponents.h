#ifndef DAYWEATHERCOMPONENTS_H
#define DAYWEATHERCOMPONENTS_H

#include <QLabel>


class DayWeatherComponents
{
public:
    DayWeatherComponents();

    QLabel* m_day;
    QLabel* m_date;

    QLabel* m_wea_icon;
    QLabel* m_wea;

    QLabel* m_air_level;
    QLabel* m_air_value;

    QLabel* m_wind_direction;
    QLabel* m_wind_level;
    QLabel* m_wind_speed;
};

#endif // DAYWEATHERCOMPONENTS_H
