#include "dayWeather.h"
#include <qdebug.h>

DayWeather::DayWeather() {}

QString DayWeather::printDayWeather()
{
    qDebug() << "m_day: " << m_day << ", m_date: " << m_date \
             << "m_wea_icon: " << m_wea_icon << ", m_wea: " << m_wea \
             << "m_temp_high: " << m_temp_high << ", m_temp_low: " << m_temp_low \
             << "m_air_level: " << m_air_level << ", m_air_value: " << m_air_value \
             << "m_wind_direction: " << m_wind_direction << ", m_wind_level: " << m_wind_level \
             << "m_wind_speed: " << m_wind_speed << "END.";
}
