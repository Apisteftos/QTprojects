#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QTimer>
#include <QtQml/qqmlregistration.h>

// A simulated sensor. A timer in C++ produces a new reading again and again,
// and QML reacts to the signals. There is exactly one Sensor: it is a singleton.
class Sensor : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON   // QML does not create "Sensor { }". It uses the one object under the name "Sensor".

    Q_PROPERTY(double value READ value NOTIFY valueChanged)
    Q_PROPERTY(int readings READ readings NOTIFY valueChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(double threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)

public:
    explicit Sensor(QObject *parent = nullptr);

    double value() const { return m_value; }
    int readings() const { return m_readings; }
    bool running() const { return m_timer.isActive(); }

    int interval() const { return m_timer.interval(); }
    void setInterval(int milliseconds);

    double threshold() const { return m_threshold; }
    void setThreshold(double threshold);

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void advance();   // one reading now (the timer calls it, and so can a test)

signals:
    void valueChanged();
    void runningChanged();
    void intervalChanged();
    void thresholdChanged();

    // A signal with arguments: an event that carries data. QML gets the arguments by name.
    void alarm(double value, int reading);

private:
    static double readingAt(int number);   // the simulated measurement, the same every run

    QTimer m_timer;
    double m_value = 20.0;
    int m_readings = 0;
    double m_threshold = 30.0;
};

#endif // SENSOR_H
