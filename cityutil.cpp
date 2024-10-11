#include "cityutil.h"

CityUtil::CityUtil() {
    init();
}

void CityUtil::init()
{
    qDebug() << "init" << cityJson;
    QFile file(cityJson);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "文件: " << cityJson << "打开失败!!!";
        return;
    }
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = jsonDocument.array();
    // qDebug() << jsonArray.size();  // 3201
    for(QJsonValue obj : jsonArray){
        QString cityName= obj["cityZh"].toString();
        QString cityId= obj["id"].toString();
        cityMap.insert(cityName, cityId);
        // qDebug() << cityName << cityId;
    }
}

QString CityUtil::getCityIdByCityName(QString cityName)
{
    // 若没有查询需求，则不先初始化
    if(cityMap.isEmpty()){
        init();
    }
    return cityMap[cityName];
}
