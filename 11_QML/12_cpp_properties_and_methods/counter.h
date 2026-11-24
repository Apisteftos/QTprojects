#ifndef COUNTER_H
#define COUNTER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

// A C++ class that QML can use as a type: "Counter { }".
// The logic lives here. QML only shows it and calls it.
class Counter : public QObject
{
    Q_OBJECT
    QML_ELEMENT   // makes the class available in QML under its own name (the module is the URI of CMake)

    // A property has a type, a name and up to four parts:
    //   READ    the getter         WRITE  the setter (leave out for read-only)
    //   NOTIFY  the signal that is emitted when the value changes (needed for bindings to update)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int step READ step WRITE setStep NOTIFY stepChanged)
    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    Q_PROPERTY(bool atLimit READ atLimit NOTIFY valueChanged STORED false)   // calculated, read-only
    Q_PROPERTY(QString summary READ summary NOTIFY summaryChanged)           // calculated text for a binding

public:
    explicit Counter(QObject *parent = nullptr);

    int value() const { return m_value; }
    void setValue(int value);

    int step() const { return m_step; }
    void setStep(int step);

    int limit() const { return m_limit; }
    void setLimit(int limit);

    bool atLimit() const { return m_value >= m_limit; }
    QString summary() const { return describe(QStringLiteral("points")); }

    // Q_INVOKABLE makes a normal member function callable from QML.
    Q_INVOKABLE bool increment();           // returns false if the limit stopped it
    Q_INVOKABLE void decrement();
    Q_INVOKABLE void reset();
    Q_INVOKABLE QString describe(const QString &unit) const;

signals:
    void valueChanged();
    void stepChanged();
    void limitChanged();
    void summaryChanged();   // emitted when value or limit change

private:
    int m_value = 0;
    int m_step = 1;
    int m_limit = 10;
};

#endif // COUNTER_H
