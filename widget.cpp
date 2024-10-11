#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent) , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // cityUtil
    cityUtil = new CityUtil();
    todayWeather = new TodayWeather();

    // 窗口设置
    // this->setFont(QFont("Arial",19));
    setFixedSize(490,926);
    setWindowFlag(Qt::FramelessWindowHint);

    // 退出菜单设置
    menuQuit = new QMenu(this);
    menuQuit->setStyleSheet("QMenu::item {color:red}");
    // 为菜单添加一个关闭动作
    QAction *closeAct = new QAction(QIcon(":/res/icons/tip/close.png"), tr("关闭程序"), this);
    menuQuit->addAction(closeAct);
    // 绑定信号和槽，菜单item被按下则关闭窗口
    connect(menuQuit, &QMenu::triggered, this, [=]{this->close();});


    // Network配置
    manager = new QNetworkAccessManager(this);
    // 天气API配置  http://tianqiapi.com  damll0, 9  限免3000次调用
    // weaApi = "http://v1.yiketianqi.com/api?unescape=1&version=v91&appid=27468277&appsecret=wsxTYj5G";
    // 测试 weaApi = "http://v0.yiketianqi.com/free/v2031?adcode=130200000000&appid=27468277&appsecret=wsxTYj5G&hours=1&cityid=CN101120206";
    weaApi = "http://v0.yiketianqi.com/free/v2031?adcode=130200000000&appid=27468277&appsecret=wsxTYj5G&hours=1";
    QNetworkRequest req(QUrl(weaApi+"&cityid=CN101120206"));  // 黄岛区cityid
    qDebug() << weaApi+"&cityid=CN101120206";
    reply = manager->get(req);
    // 绑定信号和槽
    connect(manager, &QNetworkAccessManager::finished, this, &Widget::processHTTPReply);

    initWeatherTypeMap();
    initUIComponents();
}


Widget::~Widget()
{
    delete ui;
}


void Widget::initWeatherTypeMap()
{
    // 天气类型，天气图标
    weatherTypeMap.insert("晴",":/res/icons/main/晴.png");
    weatherTypeMap.insert("夜晴",":/res/icons/main/夜晴.png");
    weatherTypeMap.insert("阴",":/res/icons/main/阴.png");
    weatherTypeMap.insert("多云",":/res/icons/main/多云.png");
    weatherTypeMap.insert("夜多云",":/res/icons/main/夜多云.png");
    weatherTypeMap.insert("雾",":/res/icons/main/雾.png");
    weatherTypeMap.insert("雾霾",":/res/icons/main/雾霾.png");
    weatherTypeMap.insert("小雨",":/res/icons/main/小雨.png");
    weatherTypeMap.insert("中雨",":/res/icons/main/中雨.png");
    weatherTypeMap.insert("大雨",":/res/icons/main/大雨.png");
    weatherTypeMap.insert("暴雨",":/res/icons/main/暴雨.png");
    weatherTypeMap.insert("大暴雨",":/res/icons/main/大暴雨.png");
    weatherTypeMap.insert("特大暴雨",":/res/icons/main/特大暴雨.png");
    weatherTypeMap.insert("小雪",":/res/icons/main/小雪.png");
    weatherTypeMap.insert("中雪",":/res/icons/main/中雪.png");
    weatherTypeMap.insert("大雪",":/res/icons/main/大雪.png");
    weatherTypeMap.insert("暴雪",":/res/icons/main/暴雪.png");
    weatherTypeMap.insert("雷阵雨",":/res/icons/main/雷阵雨.png");
    weatherTypeMap.insert("阵雨",":/res/icons/main/阵雨.png");
    weatherTypeMap.insert("阵雪",":/res/icons/main/阵雪.png");
    weatherTypeMap.insert("雨夹雪",":/res/icons/main/雨夹雪.png");
    weatherTypeMap.insert("冻雨",":/res/icons/main/冻雨.png");
    weatherTypeMap.insert("沙尘",":/res/icons/main/沙尘.png");
    weatherTypeMap.insert("沙尘暴",":/res/icons/main/沙尘暴.png");
    weatherTypeMap.insert("强沙尘暴",":/res/icons/main/强沙尘暴.png");
    weatherTypeMap.insert("浮尘",":/res/icons/main/浮尘.png");
    weatherTypeMap.insert("扬沙",":/res/icons/main/扬沙.png");
    weatherTypeMap.insert("未知",":/res/icons/main/未知.png");
}

void Widget::initUIComponents()
{
    // 未来6小时预报组件
    hoursWeatherComponents[0] = new HoursWeatherComponents();
    hoursWeatherComponents[1] = new HoursWeatherComponents();
    hoursWeatherComponents[2] = new HoursWeatherComponents();
    hoursWeatherComponents[3] = new HoursWeatherComponents();
    hoursWeatherComponents[4] = new HoursWeatherComponents();
    hoursWeatherComponents[5] = new HoursWeatherComponents();

    hoursWeatherComponents[0]->m_time=ui->label_h1_time;
    hoursWeatherComponents[0]->m_wea_icon=ui->label_h1_icon;
    hoursWeatherComponents[0]->m_temp=ui->label_h1_temp;

    hoursWeatherComponents[1]->m_time=ui->label_h2_time;
    hoursWeatherComponents[1]->m_wea_icon=ui->label_h2_icon;
    hoursWeatherComponents[1]->m_temp=ui->label_h2_temp;

    hoursWeatherComponents[2]->m_time=ui->label_h3_time;
    hoursWeatherComponents[2]->m_wea_icon=ui->label_h3_icon;
    hoursWeatherComponents[2]->m_temp=ui->label_h3_temp;

    hoursWeatherComponents[3]->m_time=ui->label_h4_time;
    hoursWeatherComponents[3]->m_wea_icon=ui->label_h4_icon;
    hoursWeatherComponents[3]->m_temp=ui->label_h4_temp;

    hoursWeatherComponents[4]->m_time=ui->label_h5_time;
    hoursWeatherComponents[4]->m_wea_icon=ui->label_h5_icon;
    hoursWeatherComponents[4]->m_temp=ui->label_h5_temp;

    hoursWeatherComponents[5]->m_time=ui->label_h6_time;
    hoursWeatherComponents[5]->m_wea_icon=ui->label_h6_icon;
    hoursWeatherComponents[5]->m_temp=ui->label_h6_temp;


    // 未来6天预报组件
    dayWeatherComponents[0] = new DayWeatherComponents();
    dayWeatherComponents[1] = new DayWeatherComponents();
    dayWeatherComponents[2] = new DayWeatherComponents();
    dayWeatherComponents[3] = new DayWeatherComponents();
    dayWeatherComponents[4] = new DayWeatherComponents();
    dayWeatherComponents[5] = new DayWeatherComponents();

    dayWeatherComponents[0]->m_day=ui->label_w1_day;
    dayWeatherComponents[0]->m_date=ui->label_w1_date;
    dayWeatherComponents[0]->m_wea_icon=ui->label_w1_icon;
    dayWeatherComponents[0]->m_wea=ui->label_w1_wea;
    dayWeatherComponents[0]->m_air_level=ui->label_w1_air_level;
    dayWeatherComponents[0]->m_air_value=ui->label_w1_air_value;
    dayWeatherComponents[0]->m_wind_direction=ui->label_w1_wind_level;
    dayWeatherComponents[0]->m_wind_level=ui->label_w1_wind_level;
    dayWeatherComponents[0]->m_wind_speed=ui->label_w1_wind_speed;

    dayWeatherComponents[1]->m_day=ui->label_w2_day;
    dayWeatherComponents[1]->m_date=ui->label_w2_date;
    dayWeatherComponents[1]->m_wea_icon=ui->label_w2_icon;
    dayWeatherComponents[1]->m_wea=ui->label_w2_wea;
    dayWeatherComponents[1]->m_air_level=ui->label_w2_air_level;
    dayWeatherComponents[1]->m_air_value=ui->label_w2_air_value;
    dayWeatherComponents[1]->m_wind_direction=ui->label_w2_wind_direction;
    dayWeatherComponents[1]->m_wind_level=ui->label_w2_wind_level;
    dayWeatherComponents[1]->m_wind_speed=ui->label_w2_wind_speed;

    dayWeatherComponents[2]->m_day=ui->label_w3_day;
    dayWeatherComponents[2]->m_date=ui->label_w3_date;
    dayWeatherComponents[2]->m_wea_icon=ui->label_w3_icon;
    dayWeatherComponents[2]->m_wea=ui->label_w3_wea;
    dayWeatherComponents[2]->m_air_level=ui->label_w3_air_level;
    dayWeatherComponents[2]->m_air_value=ui->label_w3_air_value;
    dayWeatherComponents[2]->m_wind_direction=ui->label_w3_wind_direction;
    dayWeatherComponents[2]->m_wind_level=ui->label_w3_wind_level;
    dayWeatherComponents[2]->m_wind_speed=ui->label_w3_wind_speed;

    dayWeatherComponents[3]->m_day=ui->label_w4_day;
    dayWeatherComponents[3]->m_date=ui->label_w4_date;
    dayWeatherComponents[3]->m_wea_icon=ui->label_w4_icon;
    dayWeatherComponents[3]->m_wea=ui->label_w4_wea;
    dayWeatherComponents[3]->m_air_level=ui->label_w4_air_level;
    dayWeatherComponents[3]->m_air_value=ui->label_w4_air_value;
    dayWeatherComponents[3]->m_wind_direction=ui->label_w4_wind_direction;
    dayWeatherComponents[3]->m_wind_level=ui->label_w4_wind_level;
    dayWeatherComponents[3]->m_wind_speed=ui->label_w4_wind_speed;

    dayWeatherComponents[4]->m_day=ui->label_w5_day;
    dayWeatherComponents[4]->m_date=ui->label_w5_date;
    dayWeatherComponents[4]->m_wea_icon=ui->label_w5_icon;
    dayWeatherComponents[4]->m_wea=ui->label_w5_wea;
    dayWeatherComponents[4]->m_air_level=ui->label_w5_air_level;
    dayWeatherComponents[4]->m_air_value=ui->label_w5_air_value;
    dayWeatherComponents[4]->m_wind_direction=ui->label_w5_wind_direction;
    dayWeatherComponents[4]->m_wind_level=ui->label_w5_wind_level;
    dayWeatherComponents[4]->m_wind_speed=ui->label_w5_wind_speed;

    dayWeatherComponents[5]->m_day=ui->label_w6_day;
    dayWeatherComponents[5]->m_date=ui->label_w6_date;
    dayWeatherComponents[5]->m_wea_icon=ui->label_w6_icon;
    dayWeatherComponents[5]->m_wea=ui->label_w6_wea;
    dayWeatherComponents[5]->m_air_level=ui->label_w6_air_level;
    dayWeatherComponents[5]->m_air_value=ui->label_w6_air_value;
    dayWeatherComponents[5]->m_wind_direction=ui->label_w6_wind_direction;
    dayWeatherComponents[5]->m_wind_level=ui->label_w6_wind_level;
    dayWeatherComponents[5]->m_wind_speed=ui->label_w6_wind_speed;

    for(int i=0; i<6; ++i) dayWeather[i]=new DayWeather();

    ui->widget_w_high_temp->installEventFilter(this);
    ui->widget_w_low_temp->installEventFilter(this);
}


void Widget::parseJsonWeatherData(QByteArray data)
{
    qDebug() << "parseJsonWeatherData";
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);
    if( !jsonDocument.isNull() && jsonDocument.isObject()){
        QJsonObject jsonObject = jsonDocument.object();
        // qDebug() << jsonRoot;
        todayWeather->m_cityName=jsonObject["city"].toString();
        todayWeather->m_dateDay=QDateTime().currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        todayWeather->m_updateTime=jsonObject["update_time"].toString();

        // qDebug() << todayWeather->m_cityName << todayWeather->m_dateDay << todayWeather->m_updateTime;

        QJsonObject todayObject = jsonObject["day"].toObject();
        todayWeather->m_wea=todayObject["wea"].toString();
        qDebug() << "todayWeather->m_wea: " << todayWeather->m_wea;
        todayWeather->m_wea_icon=getWeatherIconByWeatherType(todayWeather->m_wea, QTime().currentTime().hour());
        todayWeather->m_temp=todayObject["tem"].toString();
        todayWeather->m_temp_low=todayObject["tem2"].toString();
        todayWeather->m_temp_high=todayObject["tem1"].toString();
        todayWeather->m_sunrise=todayObject["sunrise"].toString();
        todayWeather->m_sunset=todayObject["sunset"].toString();


        QJsonArray hoursArray = jsonObject["week"].toArray()[0].toObject()["hours"].toArray();
        // qDebug() << "hoursArray.size(): " << hoursArray.size();
        QTime currentTime = QTime::currentTime();
        QString currentTimeStr = currentTime.toString("hh:mm:ss");
        int curHour=currentTimeStr.split(':')[0].toInt();
        int nextHour = curHour==23 ? 0 : curHour+1;
        // qDebug() << "nextHour: " << nextHour;
        int index=0;
        for(; index<hoursArray.size(); ++index){
            // qDebug() << "hoursArray[index].toObject()[hours].toString().left(2): " << hoursArray[index].toObject()["hours"].toString().left(2);
            if(hoursArray[index].toObject()["hours"].toString().left(2).toInt()==nextHour) break;
        }
        // index+=1; // 再加一避免特殊情况
        // qDebug() << "index: " << index;
        hoursSize = hoursArray.size()>6 ? 6 : hoursArray.size();
        // qDebug() << "hoursSize: " << hoursSize;
        for(int i = 0; i <hoursSize; i++){
            QJsonObject hourObject = hoursArray[i+index].toObject();
            todayWeather->hoursWeather[i].time=hourObject["hours"].toString();
            int hour = hourObject["hours"].toString().left(2).toInt();
            todayWeather->hoursWeather[i].wea_icon=getWeatherIconByWeatherType(hourObject["wea"].toString(), hour);
            todayWeather->hoursWeather[i].temp=hourObject["tem"].toString();
            // qDebug() << todayWeather->hoursWeather[i].time \
            //          << todayWeather->hoursWeather[i].wea_icon \
            //          << todayWeather->hoursWeather[i].temp;
        }

        QJsonArray alarmsArray = todayObject["alarm"].toArray();
        if(!alarmsArray.isEmpty()){
            QString tip="天气预警: ";
            for(QJsonValue obj : alarmsArray){
                tip+=obj.toObject()["alarm_type"].toString() + " ";
                tip+=obj.toObject()["alarm_level"].toString() + "预警 ";
            }
            todayWeather->m_tip=tip;
        }else{
            todayWeather->m_tip=("温馨提示: 天气多变，请注意增减衣物，不要感冒哦。");
        }

        // QJsonObject aqiObject = todayObject["aqi"].toObject();
        // todayWeather->m_tip=aqiObject["air_tips"].toString();
        // todayWeather->m_tip=aqiObject["yundong"].toString() + "  " \
        //                       + aqiObject["waichu"].toString() + "  " \
        //                       + aqiObject["kaichuang"].toString();
        // qDebug() << todayWeather->m_tip;
        todayWeather->m_wind_direction=todayObject["win"].toString();
        // todayWeather->m_wind_level=todayObject["win_speed"].toString();
        QString today_wind_level = todayObject["win_speed"].toString();
        int todat_index = today_wind_level.indexOf("转");
        if(todat_index != -1){
            todayWeather->m_wind_level=today_wind_level.left(todat_index);
        }else{
            todayWeather->m_wind_level=today_wind_level;
        }
        todayWeather->m_wind_speed=todayObject["win_meter"].toString();
        todayWeather->m_air_level=todayObject["air_level"].toString();
        todayWeather->m_air_value=todayObject["air"].toString();
        todayWeather->m_humidity=todayObject["humidity"].toString();
        todayWeather->m_visibility=todayObject["visibility"].toString();
        todayWeather->m_pm25=todayObject["air_pm25"].toString();

        // qDebug() << todayWeather->printTodayWeather();

        QJsonArray weeksArray = jsonObject["week"].toArray();
        daysSize = weeksArray.size()>6 ? 6 : weeksArray.size();
        qDebug() << "daysSize: " << daysSize;
        for(int i = 0; i <daysSize; i++){
            // dayWeather[i]=new DayWeather();
            QJsonObject dayObject = weeksArray[i].toObject();
            dayWeather[i]->m_day=dayObject["week"].toString();
            dayWeather[i]->m_date=dayObject["date"].toString().right(5);
            dayWeather[i]->m_wea_icon=getWeatherIconByWeatherType(dayObject["wea"].toString(), 8);
            dayWeather[i]->m_wea=dayObject["wea"].toString();
            dayWeather[i]->m_temp_high=dayObject["tem1"].toString();
            dayWeather[i]->m_temp_low=dayObject["tem2"].toString();
            dayWeather[i]->m_air_level=dayObject["air_level"].toString();
            dayWeather[i]->m_air_value=dayObject["air"].toString();
            dayWeather[i]->m_wind_direction=dayObject["win"].toArray()[0].toString() + "/" + dayObject["win"].toArray()[1].toString();
            QString day_wind_level = dayObject["win_speed"].toString();
            int day_index = day_wind_level.indexOf("转");
            if(day_index != -1){
                dayWeather[i]->m_wind_level=day_wind_level.left(day_index);
            }else{
                dayWeather[i]->m_wind_level=day_wind_level;
            }
            dayWeather[i]->m_wind_speed=dayObject["win_meter"].toString();

            // qDebug() << dayWeather[i]->printDayWeather();
            // qDebug() << dayObject["win_meter"].toString();
        }
        // dayWeather[0]->m_day="今天";
        // dayWeather[1]->m_day="明天";
        // dayWeather[2]->m_day="后天";
    }
    updateUI();
}

QString Widget::getWeatherIconByWeatherType(QString WeatherType, int hour)
{
    if((hour<6 || hour>18) && (WeatherType=="晴" || WeatherType=="多云")) WeatherType = "夜" + WeatherType;
    int index = WeatherType.indexOf("转");
    if(index != -1){
        // qDebug() << weatherTypeMap[WeatherType.left(index)];
        // ui->label_weathert_icon->setPixmap(weatherTypeMap[WeatherType.left(index)]);
        return weatherTypeMap[WeatherType.left(index)];
    }else{
        // qDebug() << weatherTypeMap[WeatherType];
        // ui->label_weathert_icon->setPixmap(weatherTypeMap[WeatherType]);
        if(WeatherType=="雨") return weatherTypeMap["小雨"];
        return weatherTypeMap[WeatherType];
    }
}

void Widget::updateUI()
{
    qDebug() << "updateUI";
    QPixmap pixmap;
    QFont font;

    // 日期 星期几
    ui->label_date_day->setText(todayWeather->m_dateDay + "     ");
    font = ui->label_date_day->font();  // 获取当前字体
    font.setPointSize(12);     // 设置字体大小
    ui->label_date_day->setFont(font);

    // 城市名称
    ui->label_city_name->setText(todayWeather->m_cityName);
    font = ui->label_city_name->font();  // 获取当前字体
    font.setPointSize(26);     // 设置字体大小
    ui->label_city_name->setFont(font);

    // 天气类型
    ui->label_weather_name->setText(todayWeather->m_wea);
    font = ui->label_weather_name->font();  // 获取当前字体
    font.setPointSize(22);     // 设置字体大小
    ui->label_weather_name->setFont(font);

    // 天气图标
    ui->label_weathert_icon->setPixmap(todayWeather->m_wea_icon);
    // 温度
    ui->label_temp->setText(todayWeather->m_temp+"℃");
    font = ui->label_temp->font();  // 获取当前字体
    font.setPointSize(24);     // 设置字体大小
    ui->label_temp->setFont(font);

    // 温度范围
    ui->label_temp_range->setText(todayWeather->m_temp_low + "℃～" + todayWeather->m_temp_high + "℃");
    font = ui->label_temp_range->font();  // 获取当前字体
    font.setPointSize(16);     // 设置字体大小
    ui->label_temp_range->setFont(font);

    // 日出日落
    ui->label_sun->setText(todayWeather->m_sunrise + "/" + todayWeather->m_sunset);
    font = ui->label_sun->font();  // 获取当前字体
    font.setPointSize(16);     // 设置字体大小
    ui->label_sun->setFont(font);

    // 数据更新时间
    ui->label_update_time->setText("更新于" + todayWeather->m_updateTime);
    font = ui->label_update_time->font();  // 获取当前字体
    font.setPointSize(10);     // 设置字体大小
    ui->label_update_time->setFont(font);
    ui->label_update_time->setAlignment(Qt::AlignCenter);
    // ui->label_update_time->setAlignment(Qt::AlignBottom);

    // 天气类型
    ui->label_tip->setText(todayWeather->m_tip);
    font = ui->label_tip->font();  // 获取当前字体
    font.setPointSize(10);     // 设置字体大小
    ui->label_tip->setFont(font);
    // ui->label_tip->setAlignment(Qt::AlignCenter);

    ui->label_wind_direction->setText(todayWeather->m_wind_direction);
    font = ui->label_wind_direction->font();  // 获取当前字体
    font.setPointSize(12);     // 设置字体大小
    ui->label_wind_direction->setFont(font);
    ui->label_wind_direction->setAlignment(Qt::AlignCenter);

    ui->label_wind_level->setText(todayWeather->m_wind_level);
    font = ui->label_wind_level->font();  // 获取当前字体
    font.setPointSize(12);     // 设置字体大小
    ui->label_wind_level->setFont(font);
    ui->label_wind_level->setAlignment(Qt::AlignCenter);

    ui->label_wind_speed->setText(todayWeather->m_wind_speed);
    font = ui->label_wind_speed->font();  // 获取当前字体
    font.setPointSize(12);     // 设置字体大小
    ui->label_wind_speed->setFont(font);
    ui->label_wind_speed->setAlignment(Qt::AlignCenter);

    ui->label_air_level->setText(todayWeather->m_air_level);
    font = ui->label_air_level->font();  // 获取当前字体
    font.setPointSize(12);     // 设置字体大小
    ui->label_air_level->setFont(font);
    ui->label_air_level->setAlignment(Qt::AlignCenter);

    ui->label_air_value->setText(todayWeather->m_air_value);
    font = ui->label_air_value->font();  // 获取当前字体
    font.setPointSize(12);     // 设置字体大小
    ui->label_air_value->setFont(font);
    ui->label_air_value->setAlignment(Qt::AlignCenter);


    ui->label_humidity_value->setText(todayWeather->m_humidity);
    font = ui->label_humidity_value->font();  // 获取当前字体
    font.setPointSize(12);     // 设置字体大小
    ui->label_humidity_value->setFont(font);
    ui->label_humidity_value->setAlignment(Qt::AlignCenter);

    ui->label_vision_value->setText(todayWeather->m_visibility);
    font = ui->label_vision_value->font();  // 获取当前字体
    font.setPointSize(12);     // 设置字体大小
    ui->label_vision_value->setFont(font);
    ui->label_vision_value->setAlignment(Qt::AlignCenter);

    ui->label_pm25_value->setText(todayWeather->m_pm25);
    font = ui->label_pm25_value->font();  // 获取当前字体
    font.setPointSize(12);     // 设置字体大小
    ui->label_pm25_value->setFont(font);
    ui->label_pm25_value->setAlignment(Qt::AlignCenter);


    // 未来6小时预报
    // int hoursSize = sizeof(todayWeather->hoursWeather)/sizeof(HoursWeather);
    // qDebug() << "updateUI" << "hoursSize: " << hoursSize;
    for(int i=0; i<hoursSize; ++i){
        // qDebug() << todayWeather->hoursWeather[i].time \
        //          << todayWeather->hoursWeather[i].wea_icon \
        //          << todayWeather->hoursWeather[i].temp;

        hoursWeatherComponents[i]->m_time->setText(todayWeather->hoursWeather[i].time);
        hoursWeatherComponents[i]->m_wea_icon->setPixmap(todayWeather->hoursWeather[i].wea_icon);
        hoursWeatherComponents[i]->m_temp->setText(todayWeather->hoursWeather[i].temp+"℃");
    }

    // 未来6天预报
    // int daysSize = 0;
    // for(DayWeather* temp : dayWeather){
    //     if(temp!=nullptr) daysSize++;
    // }
    // qDebug() << "updateUI" << "daysSize: " << daysSize;
    for(int i=0; i<daysSize; ++i){
        dayWeatherComponents[i]->m_day->setText(dayWeather[i]->m_day);
        dayWeatherComponents[i]->m_date->setText(dayWeather[i]->m_date);

        // dayWeatherComponents[i]->m_wea_icon->setPixmap(dayWeather[i]->m_wea_icon);
        pixmap = dayWeather[i]->m_wea_icon;
        pixmap.scaled(dayWeatherComponents[i]->m_wea_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        dayWeatherComponents[i]->m_wea_icon->setMaximumHeight(55);
        dayWeatherComponents[i]->m_wea_icon->setMinimumHeight(55);
        dayWeatherComponents[i]->m_wea_icon->setMaximumWidth(55);
        dayWeatherComponents[i]->m_wea_icon->setMinimumWidth(55);
        dayWeatherComponents[i]->m_wea_icon->setPixmap(pixmap);

        dayWeatherComponents[i]->m_wea->setText(dayWeather[i]->m_wea);
        dayWeatherComponents[i]->m_air_level->setText(dayWeather[i]->m_air_level);

        QString air_level=dayWeather[i]->m_air_level;
        dayWeatherComponents[i]->m_air_value->setText(dayWeather[i]->m_air_value);
        if( air_level == "优"){
            dayWeatherComponents[i]->m_air_level->setStyleSheet("background-color: rgb(150, 213, 32);border-radius: 7px;border-bottom-right-radius:0px;border-bottom-left-radius:0px;");
            dayWeatherComponents[i]->m_air_value->setStyleSheet("background-color: rgb(150, 213, 32);border-radius: 7px;border-top-right-radius:0px;border-top-left-radius:0px;");
        }else if( air_level == "良"){
            dayWeatherComponents[i]->m_air_level->setStyleSheet("background-color: rgb(255, 199, 199);border-radius: 7px;border-bottom-right-radius:0px;border-bottom-left-radius:0px;");
            dayWeatherComponents[i]->m_air_value->setStyleSheet("background-color: rgb(255, 199, 199);border-radius: 7px;border-top-right-radius:0px;border-top-left-radius:0px;");
        }else if( air_level == "轻度污染"){
            dayWeatherComponents[i]->m_air_level->setStyleSheet("background-color: rgb(208, 107, 39);border-radius: 7px;border-bottom-right-radius:0px;border-bottom-left-radius:0px;");
            dayWeatherComponents[i]->m_air_value->setStyleSheet("background-color: rgb(208, 107, 39);border-radius: 7px;border-top-right-radius:0px;border-top-left-radius:0px;");
        }else if( air_level == "中度污染"){
            dayWeatherComponents[i]->m_air_level->setStyleSheet("background-color: rgb(255, 17, 17);border-radius: 7px;border-bottom-right-radius:0px;border-bottom-left-radius:0px;");
            dayWeatherComponents[i]->m_air_value->setStyleSheet("background-color: rgb(255, 17, 17);border-radius: 7px;border-top-right-radius:0px;border-top-left-radius:0px;");
        }else if( air_level == "重度污染"){
            dayWeatherComponents[i]->m_air_level->setStyleSheet("background-color: rgb(153, 0, 0);border-radius: 7px;border-bottom-right-radius:0px;border-bottom-left-radius:0px;");
            dayWeatherComponents[i]->m_air_value->setStyleSheet("background-color: rgb(153, 0, 0);border-radius: 7px;border-top-right-radius:0px;border-top-left-radius:0px;");
        }else{
            dayWeatherComponents[i]->m_air_level->setStyleSheet("background-color: rgb(175,238,238);border-radius: 7px;border-bottom-right-radius:0px;border-bottom-left-radius:0px;");
            dayWeatherComponents[i]->m_air_value->setStyleSheet("background-color: rgb(175,238,238);border-radius: 7px;border-top-right-radius:0px;border-top-left-radius:0px;");
        }

        dayWeatherComponents[i]->m_wind_direction->setText(dayWeather[i]->m_wind_direction);
        dayWeatherComponents[i]->m_wind_level->setText(dayWeather[i]->m_wind_level);
        dayWeatherComponents[i]->m_wind_speed->setText(dayWeather[i]->m_wind_speed.isEmpty() ? "--" : dayWeather[i]->m_wind_speed);


        dayWeatherComponents[i]->m_day->setAlignment(Qt::AlignCenter);
        dayWeatherComponents[i]->m_date->setAlignment(Qt::AlignCenter);
        dayWeatherComponents[i]->m_wea->setAlignment(Qt::AlignCenter);
        dayWeatherComponents[i]->m_air_level->setAlignment(Qt::AlignCenter);
        dayWeatherComponents[i]->m_air_value->setAlignment(Qt::AlignCenter);
        dayWeatherComponents[i]->m_wind_direction->setAlignment(Qt::AlignCenter);
        dayWeatherComponents[i]->m_wind_level->setAlignment(Qt::AlignCenter);
        dayWeatherComponents[i]->m_wind_speed->setAlignment(Qt::AlignCenter);
    }
    // drawHighTempLine();
    // drawLowTempLine();
    update();
}

void Widget::drawHighTempLine()
{
    qDebug() << "drawHighTempLine";
    QPainter painter(ui->widget_w_high_temp);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // painter.setBrush(QColor("#CD5C5C"));
    // painter.setPen(QColor("#CD5C5C"));
    painter.setBrush(QColor("#B22222"));
    painter.setPen(QColor("#B22222"));

    int ave;
    int sum = 0;
    int offSet = 0;
    // int middle = ui->widget_w_high_temp->y() + ui->widget_w_high_temp->height()/2;
    int middle = ui->widget_w_high_temp->height()*0.7;
    // qDebug() << "middle: " << middle;


    // qDebug() << "daysSize: " << daysSize;
    for(int i = 0; i < daysSize; i++){
        // qDebug() << dayWeather[i]->m_temp_high;
        sum += dayWeather[i]->m_temp_high.toInt();
    }
    ave = sum / daysSize;
    // qDebug() << "high_temp_avg: " << ave;

    //定义出daysSize个点
    QPoint points[daysSize];
    for(int i = 0; i < daysSize; i++){
        points[i].setX(dayWeatherComponents[i]->m_wea->x() + dayWeatherComponents[i]->m_wea->width()/2);
        offSet = (dayWeather[i]->m_temp_high.toInt() - ave)*4;
        points[i].setY(middle - offSet);
        // qDebug() << "high point_x: " << points[i].x() << "point_y: " << points[i].y()  ;
        //画出daysSize个点
        painter.drawEllipse(QPoint(points[i]), 3, 3);
        //画出当天温度
        painter.drawText(points[i].x()-15, points[i].y()-15, dayWeather[i]->m_temp_high+"℃");
    }

    for(int i = 0; i < daysSize-1; i++){
        painter.drawLine(points[i], points[i+1]);
    }
}

void Widget::drawLowTempLine()
{
    qDebug() << "drawLowTempLine";
    QPainter painter(ui->widget_w_low_temp);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::blue);
    painter.setPen(Qt::blue);

    int ave;
    int sum = 0;
    int offSet = 0;
    // int middle = ui->widget_w_low_temp->y() + ui->widget_w_low_temp->height()/2;
    int middle = ui->widget_w_low_temp->height()*0.7;
    // qDebug() << "middle: " << middle;

    for(int i = 0; i < daysSize; i++){
        // qDebug() << dayWeather[i]->m_temp_low;
        sum += dayWeather[i]->m_temp_low.toInt();
    }
    ave = sum / daysSize;
    // qDebug() << "low_temp_avg: " << ave;

    //定义出daysSize个点
    QPoint points[daysSize];
    for(int i = 0; i < daysSize; i++){
        points[i].setX(dayWeatherComponents[i]->m_wea->x() + dayWeatherComponents[i]->m_wea->width()/2);
        offSet = (dayWeather[i]->m_temp_low.toInt() - ave)*4;
        points[i].setY(middle - offSet);
        // qDebug() << "low point_x: " << points[i].x() << "point_y: " << points[i].y()  ;
        //画出daysSize个点
        painter.drawEllipse(QPoint(points[i]), 3, 3);
        //画出当天温度
        painter.drawText(points[i].x()-15, points[i].y()-15, dayWeather[i]->m_temp_low+"℃");
    }

    for(int i = 0; i < daysSize-1; i++){
        painter.drawLine(points[i], points[i+1]);
    }
}

void Widget::drawTest()
{
    QPainter painter(ui->widget_w_high_temp);
    painter.setPen(Qt::yellow);
    painter.drawLine(QPoint(10, 10), QPoint(30, 30));
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->widget_w_high_temp && event->type() == QEvent::Paint){
        // QThread::msleep(3000);  // 睡眠1000毫秒，即1秒
        drawHighTempLine();
        // drawTest();
        return true;  //表示事件已被处理
    }
    if(watched == ui->widget_w_low_temp && event->type() == QEvent::Paint){
        // QThread::msleep(1000);  // 睡眠1000毫秒，即1秒
        drawLowTempLine();
        return true;  //表示事件已被处理
    }
    return QWidget::eventFilter(watched, event);
}

void Widget::processHTTPReply()
{
    // qDebug() << "processHTTPReply...";
    // QNetworkReply *reply=nullptr;
    int resCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << resCode;
    if( reply->error() == QNetworkReply::NoError && resCode == 200){
        //大多数服务器返回的字符编码格式是utf-8
        rawData = reply->readAll();
        // qDebug() << QString::fromUtf8(rawData);
        parseJsonWeatherData(rawData);
    }else{
        // QMessageBox::warning(this,"错误","网络请求失败",QMessageBox::Ok);
        QMessageBox mes;
        mes.setWindowTitle("错误");
        mes.setText("网络请求失败");
        mes.setStyleSheet("QPushButton {color:red}");
        mes.setStandardButtons(QMessageBox::Ok);
        mes.exec();
    }
}

void Widget::on_button_search_clicked()
{
    QString cityName = ui->lineEdit_inputSite->text();
    QString cityId = cityUtil->getCityIdByCityName(cityName);
    // qDebug() << cityName << cityId;
    if(!cityId.isNull()){
        QString weaApiTemp = weaApi +"&cityid=" + cityId;
        qDebug() << weaApiTemp;
        reply = manager->get(QNetworkRequest(QUrl(weaApiTemp)));
    }else{
        QMessageBox mes;
        mes.setWindowTitle("错误");
        mes.setText("请输入正确的地区名称!");
        mes.setStyleSheet("QPushButton {color:red}");
        mes.setStandardButtons(QMessageBox::Ok);
        mes.exec();
    }
}


void Widget::on_lineEdit_inputSite_returnPressed()
{
    on_button_search_clicked();
}


void Widget::mousePressEvent(QMouseEvent *event)
{
    // 鼠标右键按下
    if(event->button() == Qt::RightButton){
        menuQuit->exec(QCursor::pos());
    }
    // 偏移量=鼠标当前位置 - 窗口当前位置
    if(event->button() == Qt::LeftButton){
        mouseWindowsOffset = event->globalPosition().toPoint()-this->pos();
    }
}


void Widget::mouseMoveEvent(QMouseEvent *event)
{
    // 新窗口位置=鼠标当前位置 - 偏移量
    this->move(event->globalPosition().toPoint() - mouseWindowsOffset);
}
