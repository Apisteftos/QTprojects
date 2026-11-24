#include "counter.h"

#include <algorithm>

Counter::Counter(QObject *parent)
    : QObject(parent)
{
}

void Counter::setValue(int value)
{
    value = std::clamp(value, 0, m_limit);   // the class keeps its own value valid
    if (value == m_value)
        return;                              // nothing changed: no signal, no update in QML
    m_value = value;
    emit valueChanged();
    emit summaryChanged();
}

void Counter::setStep(int step)
{
    step = std::max(1, step);
    if (step == m_step)
        return;
    m_step = step;
    emit stepChanged();
}

void Counter::setLimit(int limit)
{
    limit = std::max(1, limit);
    if (limit == m_limit)
        return;
    m_limit = limit;
    emit limitChanged();
    emit summaryChanged();
    if (m_value > m_limit)
        setValue(m_limit);                   // the value may not be above the new limit
}

bool Counter::increment()
{
    if (atLimit())
        return false;
    setValue(m_value + m_step);
    return true;
}

void Counter::decrement()
{
    setValue(m_value - m_step);
}

void Counter::reset()
{
    setValue(0);
}

QString Counter::describe(const QString &unit) const
{
    return QString("%1 of %2 %3").arg(m_value).arg(m_limit).arg(unit);
}
