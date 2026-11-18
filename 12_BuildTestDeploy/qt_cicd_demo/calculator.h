#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QString>
#include <QStack>

// ============================================================
// Calculator — business logic class
//
// Intentionally simple so the CI/CD pipeline is the focus,
// not the app complexity.
//
// Supports:
//   - Basic arithmetic: +, -, *, /
//   - History of calculations
//   - Error handling (division by zero, invalid input)
//   - Clear / undo
// ============================================================
class Calculator : public QObject
{
    Q_OBJECT

public:
    explicit Calculator(QObject* parent = nullptr);

    // Operations
    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b, bool* ok = nullptr);

    // Expression evaluator
    double evaluate(const QString& expression, bool* ok = nullptr);

    // State
    double         lastResult()  const { return m_lastResult; }
    QStringList    history()     const { return m_history; }
    QString        lastError()   const { return m_lastError; }
    bool           hasError()    const { return !m_lastError.isEmpty(); }
    void           clearHistory();
    void           clearError();

signals:
    void resultReady(double result, const QString& expression);
    void errorOccurred(const QString& error);
    void historyChanged();

private:
    void        recordHistory(const QString& expression, double result);

    double      m_lastResult = 0.0;
    QString     m_lastError;
    QStringList m_history;

    static const int MAX_HISTORY = 50;
};

#endif // CALCULATOR_H
