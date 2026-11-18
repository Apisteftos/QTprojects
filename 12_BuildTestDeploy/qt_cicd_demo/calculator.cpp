#include "calculator.h"
#include <QRegularExpression>
#include <cmath>

Calculator::Calculator(QObject* parent)
    : QObject(parent), m_lastResult(0.0)
{}

double Calculator::add(double a, double b) {
    double result = a + b;
    m_lastResult = result;
    clearError();
    QString expr = QString("%1 + %2 = %3").arg(a).arg(b).arg(result);
    recordHistory(expr, result);
    emit resultReady(result, expr);
    return result;
}

double Calculator::subtract(double a, double b) {
    double result = a - b;
    m_lastResult = result;
    clearError();
    QString expr = QString("%1 - %2 = %3").arg(a).arg(b).arg(result);
    recordHistory(expr, result);
    emit resultReady(result, expr);
    return result;
}

double Calculator::multiply(double a, double b) {
    double result = a * b;
    m_lastResult = result;
    clearError();
    QString expr = QString("%1 * %2 = %3").arg(a).arg(b).arg(result);
    recordHistory(expr, result);
    emit resultReady(result, expr);
    return result;
}

double Calculator::divide(double a, double b, bool* ok) {
    if (qFuzzyIsNull(b)) {
        m_lastError = "Division by zero";
        emit errorOccurred(m_lastError);
        if (ok) *ok = false;
        return 0.0;
    }
    double result = a / b;
    m_lastResult = result;
    clearError();
    QString expr = QString("%1 / %2 = %3").arg(a).arg(b).arg(result);
    recordHistory(expr, result);
    emit resultReady(result, expr);
    if (ok) *ok = true;
    return result;
}

double Calculator::evaluate(const QString& expression, bool* ok) {
    // Simple two-operand expression parser: "a op b"
    // Supports: +, -, *, /
    QString expr = expression.trimmed();

    // Match: number operator number
    QRegularExpression re(
        R"(^\s*(-?\d+\.?\d*)\s*([+\-*/])\s*(-?\d+\.?\d*)\s*$)");
    QRegularExpressionMatch match = re.match(expr);

    if (!match.hasMatch()) {
        m_lastError = QString("Invalid expression: '%1'").arg(expr);
        emit errorOccurred(m_lastError);
        if (ok) *ok = false;
        return 0.0;
    }

    double a  = match.captured(1).toDouble();
    QString op = match.captured(2);
    double b  = match.captured(3).toDouble();

    if (ok) *ok = true;

    if (op == "+") return add(a, b);
    if (op == "-") return subtract(a, b);
    if (op == "*") return multiply(a, b);
    if (op == "/") return divide(a, b, ok);

    m_lastError = "Unknown operator";
    emit errorOccurred(m_lastError);
    if (ok) *ok = false;
    return 0.0;
}

void Calculator::clearHistory() {
    m_history.clear();
    emit historyChanged();
}

void Calculator::clearError() {
    m_lastError.clear();
}

void Calculator::recordHistory(const QString& expression, double result) {
    Q_UNUSED(result)
    m_history.prepend(expression);
    if (m_history.size() > MAX_HISTORY)
        m_history.removeLast();
    emit historyChanged();
}
