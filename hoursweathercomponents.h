#ifndef HOURSWEATHERCOMPONENTS_H
#define HOURSWEATHERCOMPONENTS_H

#include <QLabel>


class HoursWeatherComponents
{
public:
    HoursWeatherComponents();
    QLabel*  m_time;
    QLabel* m_wea_icon;
    QLabel* m_temp;
};

#endif // HOURSWEATHERCOMPONENTS_H
