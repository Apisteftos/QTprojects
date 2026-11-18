#ifndef COUNTER_H
#define COUNTER_H

#include <QObject>

class Counter : public QObject
{
    Q_OBJECT

public:
    explicit Counter(QObject* parent = nullptr);
    int value() const;

public slots:
    void increment();
    void reset();
    void setValue(int v);

signals:
    void valueChanged(int newValue);

private:
    int m_value;
};

#endif // COUNTER_H
