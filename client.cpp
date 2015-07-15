#include <QtNetwork>
#include "client.h"

Client::Client(QWidget *parent):QWidget(parent)
{    
    lightColor = QColor(0, 255, 255); // ��������� ���� ������, ������ ������ ����� ����
    isLightOn = false; // ���������� ������ ��������
    isConnected = false; // ���������� � �������� ���� �����������
    wasConnected = false;
    commandComplited = true;

    ServerPort = 9999;
    ServerHost = "127.0.0.1";
    // ���� � ����� ������ ������, � ���������� ����� ������ ���������� ��� ������� �����

    setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
    setFixedSize(600, 400);
    setWindowTitle("FlashLight");
    
    repaint();
    show();

    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
    this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(stateChanged (QAbstractSocket::SocketState)),
    this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));

    connectToServer();
}
Client::~Client()
{
    tcpSocket->disconnectFromHost();
    delete tcpSocket;
}

void Client::readInformation()
{    
    if(commandComplited) {
        if(tcpSocket->bytesAvailable() < 3) // ���� ������� �������� ������ �� �������
            return;
        
        readArray = tcpSocket->read(3);
        QDataStream dataStream(readArray);
        dataStream >> commandType;
        dataStream >> commandLength;

        if(commandLength != 0) {
            if(tcpSocket->bytesAvailable() < commandLength) {
                commandComplited = false;
                return;
            } else {
                readArray = tcpSocket->read(commandLength);
            }
        }
    } else { //������� value �������        
        if(tcpSocket->bytesAvailable() < commandLength) {
            commandComplited = false;
            return;
        } else {
            readArray = tcpSocket->read(commandLength);
            commandComplited = true;
        }
    }

    switch(commandType) {
    case 0x12:
        powerOn();
        break;
    case 0x13:
        powerOff();
        break;
    case 0x20:
    {
        // ������ ��� ������ ���������� ��� ������������ ������ �����, ����� �� ������
        // � ������ �� ����� ���������� ������ ������
        quint8 redColor;
        quint8 greenColor;
        quint8 blueColor;
        QDataStream dataStream(readArray);
        dataStream >> redColor;
        dataStream >> greenColor;
        dataStream >> blueColor;        
        changeColor(redColor, greenColor, blueColor);
        break;
    }
    default:
        break;
    }        
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        break;
    case QAbstractSocket::ConnectionRefusedError:
        break;
    default:
        break;
    }
}

void Client::socketStateChanged(QAbstractSocket::SocketState sState)
{
    switch(sState) { // ��� ���������� ������ 2 ���������: ��������� � �� ���������
    case 0: // ����� �� ��������� � �������
        isConnected = false;
        if(wasConnected) {
            wasConnected = false;
            disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readInformation()));
        }
        noConnection();
        break;
    case 3: // ����� ��������� � �������
        isConnected = true;    
        wasConnected = true;
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readInformation()), Qt::QueuedConnection);
        break;
    default:
        break;
    }
}

void Client::powerOn()
{
    isLightOn = true;
    repaint();
}

void Client::powerOff()
{
    isLightOn = false;
    repaint();
}

void Client::changeColor(quint8 redColor, quint8 greenColor, quint8 blueColor)
{
    lightColor = QColor(redColor, greenColor, blueColor);
    repaint();
}

void Client::connectToServer()
{
    tcpSocket->connectToHost(ServerHost, ServerPort);
}

void Client::noConnection()
{
    if(!isConnected) {
        tcpSocket->disconnectFromHost(); // ��������� �� �������, ����� �� ������ ������,
                                         // � ���� ������� ����� � ��� ������ ���� ��������
        switch(QMessageBox::information(this,"Attention!",
        "Server is unavailable.\r\n Try again?\r\n","Yes", "No")) {
        case 0:    // ���������� ������������ �����
            connectToServer();
            break;
        default: // ������ �� ������, ������� ���������
            close();
            break;
        }
    }    
}

void Client::paintEvent(QPaintEvent *e)
{
    QPen myPen;
    myPen = QPen(Qt::black, 3);
    myPen.setCapStyle(Qt::FlatCap);
    myPen.setJoinStyle(Qt::MiterJoin);

    QBrush myBrush;
    myBrush.setColor(Qt::darkRed);
    myBrush.setStyle(Qt::SolidPattern);

    QPainter p;
    p.begin(this);
    p.setPen(myPen);
    p.setBrush(myBrush);

    QVector<QPoint> points;
    points.append(QPoint(200, 100));
    points.append(QPoint(500, 150));
    points.append(QPoint(500, 250));
    points.append(QPoint(200, 300));

    
    p.drawPolygon(points);

    if(isLightOn)
        myBrush.setColor(lightColor);
    else
        myBrush.setColor(Qt::black);

    p.setBrush(myBrush);
    p.drawEllipse(QPoint(200, 200), 80, 100);   

    p.end();
}