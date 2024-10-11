#ifndef CITYUTIL_H
#define CITYUTIL_H

#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QDebug>


class CityUtil
{
public:
    CityUtil();
    // 根据城市名称获取城市id
    QString getCityIdByCityName(QString cityName);

private:
    // json文件
    QString cityJson=":/res/city_China.json";  // 只含中国城市, 字段较全
    // QString cityJson=":/res/city_World.json";  // 含全世界城市, 只有id, cityZh字段 国外城市天气信息不全不适配UI
    // cityMap对象
    QMap<QString, QString> cityMap;
    // 初始化方法
    void init();
};

#endif // CITYUTIL_H
