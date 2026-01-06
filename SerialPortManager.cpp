#include "SerialPortManager.h"

SerialPortManager::SerialPortManager(QObject *parent) : QObject(parent)
{
    connect(&m_serial, &QSerialPort::readyRead, this, &SerialPortManager::onReadyRead);
    connect(&m_serial, &QSerialPort::errorOccurred, this, &SerialPortManager::onError);
}

bool SerialPortManager::open(const QString &portName, int baudRate)
{
    if (m_serial.isOpen())
        m_serial.close();

    m_serial.setPortName(portName);
    m_serial.setBaudRate(baudRate);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serial.open(QIODevice::ReadWrite)) {
        emit errorOccurred(m_serial.errorString());
        // notify property change in case UI wants to react
        emit isOpenChanged();
        return false;
    }
    emit isOpenChanged();
    return true;
}

void SerialPortManager::close()
{
    if (m_serial.isOpen()) {
        m_serial.close();
        emit isOpenChanged();
    }
}

bool SerialPortManager::send(const QByteArray &data)
{
    if (!m_serial.isOpen())
        return false;

    return m_serial.write(data) == data.size();
}

bool SerialPortManager::isOpen() const
{
    return m_serial.isOpen();
}

void SerialPortManager::onReadyRead()
{
    QByteArray data = m_serial.readAll();
    emit dataReceived(data.toHex(' ').toUpper());
}

void SerialPortManager::onError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError)
        emit errorOccurred(m_serial.errorString());
}

QStringList SerialPortManager::availablePorts() const
{
    QStringList list;
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        list << info.portName();
    }
    return list;
}
