#ifndef CLIENT_H
#define CLIENT_H

#include <QtGui>
#include <QTcpSocket>
#include <QByteArray>
#include <QColor>

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = NULL);
    ~Client();    

private slots:
    void readInformation(); // Слот для получения команд от сервера
    void displayError(QAbstractSocket::SocketError socketError);
    void socketStateChanged(QAbstractSocket::SocketState sState);
    void connectToServer();
    void noConnection(); // Функция, предлагающая повторить попытку подключения, если его нет.

private:
    QByteArray readArray; // На случай, если команда придет частями
    quint8 commandType;
    quint16 commandLength;
    QColor lightColor; // Цвет фонаря
    bool isLightOn; // Включен ли фонарь
    bool isConnected; // Есть ли соединение между клиентом и сервером сейчас
    bool wasConnected; // Подключаемся впервые или после разрыва связи
    bool commandComplited; // Полностью ли обработана команда
    
    quint16 ServerPort; // Порт для подключения к серверу
    QString ServerHost; // IP-адрес сервера    
    QTcpSocket *tcpSocket;    

    void changeColor(quint8 r, quint8 g, quint8 b); // Сменить цвет фонаря
    void powerOn(); // Включить фонарь 
    void powerOff(); // Выключить фонарь //Не объединил в одну функцию чисто для наглядности
    void paintEvent(QPaintEvent *e);
};

#endif

