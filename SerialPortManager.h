#pragma once

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialPortManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isOpen READ isOpen NOTIFY isOpenChanged)
  public:
    explicit SerialPortManager(QObject *parent = nullptr);

    Q_INVOKABLE bool open(const QString &portName, int baudRate = QSerialPort::Baud115200);
    Q_INVOKABLE void close();
    Q_INVOKABLE bool send(const QByteArray &data);
    bool isOpen() const;
    Q_INVOKABLE QStringList availablePorts() const;

  signals:
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);
    void isOpenChanged();

  private slots:
    void onReadyRead();
    void onError(QSerialPort::SerialPortError error);

  private:
    QSerialPort m_serial;
};
