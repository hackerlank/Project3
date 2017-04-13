#ifndef APP_H
#define APP_H

#include <QObject>
#include <tufao-1/Tufao/HttpServer>
#include <tufao-1/Tufao/HttpServerRequest>
#include <tufao-1/Tufao/HttpServerResponse>
#include <tufao-1/Tufao/Headers>

using namespace Tufao;

class App : public QObject
{
    Q_OBJECT
public:
    explicit App(QObject *parent = 0);
    ~App();

    void handlePostData(Tufao::HttpServerRequest& request,Tufao::HttpServerResponse& response);
    void handleRegister(QString& username, QString& password);//register
    void handleLogin(QJsonObject& obj, QString& username, QString& password);//login
    void selectDatabase(QString usename, QString password);//select database
private:
    HttpServer *server;
    App(const App&);
    App& operator =(const App&);

signals:

public slots:
    void slotRequestReady(Tufao::HttpServerRequest& request,Tufao::HttpServerResponse& response);
};

#endif // APP_H
