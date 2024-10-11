#include "todayWeather.h"

TodayWeather::TodayWeather() {}


QString TodayWeather::printTodayWeather()
{
    qDebug() << "m_cityName: " << m_cityName << ", m_dateDay: " << m_dateDay\
             << ", m_wea: " <<m_wea<< ", m_wea_icon: " << m_wea_icon \
             << ", m_temp: " <<m_temp<< ", m_temp_low: " << m_temp_low \
             << ", m_temp_high: " <<m_temp_high<< ", m_sunrise: " << m_sunrise \
             << ", m_sunset: " <<m_sunset<< ", m_updateTime: " << m_updateTime \
             << ", m_tip: " <<m_tip<< ", m_wind_direction: " << m_wind_direction \
             << ", m_wind_level: " <<m_wind_level << ", m_wind_speed: " <<m_wind_speed \
             << ", m_air_value: " << m_air_value << ", m_air_level: " <<m_air_level \
             << ", m_wea_icon: " << m_wea_icon << ", m_humidity: " <<m_humidity \
             << ", m_visibility: " << m_visibility << ", m_pm25: " <<m_pm25 << "END.";
}
