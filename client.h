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
    void readInformation(); // ���� ��� ��������� ������ �� �������
    void displayError(QAbstractSocket::SocketError socketError);
    void socketStateChanged(QAbstractSocket::SocketState sState);
    void connectToServer();
    void noConnection(); // �������, ������������ ��������� ������� �����������, ���� ��� ���.

private:
    QByteArray readArray; // �� ������, ���� ������� ������ �������
    quint8 commandType;
    quint16 commandLength;
    QColor lightColor; // ���� ������
    bool isLightOn; // ������� �� ������
    bool isConnected; // ���� �� ���������� ����� �������� � �������� ������
    bool wasConnected; // ������������ ������� ��� ����� ������� �����
    bool commandComplited; // ��������� �� ���������� �������
    
    quint16 ServerPort; // ���� ��� ����������� � �������
    QString ServerHost; // IP-����� �������    
    QTcpSocket *tcpSocket;    

    void changeColor(quint8 r, quint8 g, quint8 b); // ������� ���� ������
    void powerOn(); // �������� ������ 
    void powerOff(); // ��������� ������ //�� ��������� � ���� ������� ����� ��� �����������
    void paintEvent(QPaintEvent *e);
};

#endif

