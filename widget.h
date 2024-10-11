#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QMessageBox>
#include <QPainter>
#include <QThread>

#include "cityutil.h"
#include "todayWeather.h"
#include "dayWeather.h"
#include "dayWeatherComponents.h"
#include "hoursWeatherComponents.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::Widget *ui;

    QMenu *menuQuit=nullptr;
    QPoint mouseWindowsOffset;

    CityUtil *cityUtil=nullptr;
    TodayWeather *todayWeather=nullptr;
    DayWeather *dayWeather[6]={nullptr};
    HoursWeatherComponents * hoursWeatherComponents[6]={nullptr};
    DayWeatherComponents *dayWeatherComponents[6]={nullptr};

    QMap<QString,QString> weatherTypeMap;

    QNetworkAccessManager *manager=nullptr;
    QNetworkReply *reply=nullptr;
    QByteArray rawData;
    QString weaApi;
    int hoursSize=6;
    int daysSize=6;

private slots:
    void processHTTPReply();
    void on_button_search_clicked();
    void on_lineEdit_inputSite_returnPressed();

private:
    void initWeatherTypeMap();
    void initUIComponents();
    void parseJsonWeatherData(QByteArray rawData);
    QString getWeatherIconByWeatherType(QString WeatherType, int hour);
    void updateUI();
    void drawHighTempLine();
    void drawLowTempLine();
    void drawTest();


};
#endif // WIDGET_H
