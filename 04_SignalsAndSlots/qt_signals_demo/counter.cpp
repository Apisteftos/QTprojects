#include "counter.h"

Counter::Counter(QObject* parent)
    : QObject(parent), m_value(0) {}

int Counter::value() const { return m_value; }

void Counter::increment() {
    m_value++;
    emit valueChanged(m_value);
}

void Counter::reset() {
    m_value = 0;
    emit valueChanged(m_value);
}

void Counter::setValue(int v) {
    if (v != m_value) {
        m_value = v;
        emit valueChanged(m_value);
    }
}
