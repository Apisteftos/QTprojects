#ifndef TEST_CALCULATOR_H
#define TEST_CALCULATOR_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QtMath>
#include "calculator.h"

// ============================================================
// TestCalculator — unit tests run by CI on every push
//
// These tests run automatically on GitHub Actions:
//   1. Developer pushes code
//   2. GitHub spins up Ubuntu runner
//   3. Qt6 installed
//   4. cmake --build
//   5. ./qt_cicd_demo_tests (this file)
//   6. Green ✅ or Red ❌ badge on repo
// ============================================================
class TestCalculator : public QObject
{
    Q_OBJECT

private slots:
    void init() {
        m_calc = new Calculator();
    }

    void cleanup() {
        delete m_calc;
        m_calc = nullptr;
    }

    // ── Addition ──────────────────────────────────────────
    void test_add_positives() {
        QCOMPARE(m_calc->add(2, 3), 5.0);
    }

    void test_add_negatives() {
        QCOMPARE(m_calc->add(-2, -3), -5.0);
    }

    void test_add_mixed() {
        QCOMPARE(m_calc->add(-5, 10), 5.0);
    }

    void test_add_zero() {
        QCOMPARE(m_calc->add(42, 0), 42.0);
    }

    void test_add_decimals() {
        QVERIFY(qAbs(m_calc->add(1.1, 2.2) - 3.3) < 0.0001);
    }

    // ── Subtraction ───────────────────────────────────────
    void test_subtract_basic() {
        QCOMPARE(m_calc->subtract(10, 3), 7.0);
    }

    void test_subtract_negative_result() {
        QCOMPARE(m_calc->subtract(3, 10), -7.0);
    }

    void test_subtract_same_values() {
        QCOMPARE(m_calc->subtract(5, 5), 0.0);
    }

    // ── Multiplication ────────────────────────────────────
    void test_multiply_basic() {
        QCOMPARE(m_calc->multiply(4, 5), 20.0);
    }

    void test_multiply_by_zero() {
        QCOMPARE(m_calc->multiply(999, 0), 0.0);
    }

    void test_multiply_negatives() {
        QCOMPARE(m_calc->multiply(-3, -4), 12.0);
    }

    void test_multiply_mixed_signs() {
        QCOMPARE(m_calc->multiply(-3, 4), -12.0);
    }

    // ── Division ──────────────────────────────────────────
    void test_divide_basic() {
        bool ok;
        QCOMPARE(m_calc->divide(10, 2, &ok), 5.0);
        QVERIFY(ok);
    }

    void test_divide_by_zero_fails() {
        bool ok;
        m_calc->divide(10, 0, &ok);
        QVERIFY(!ok);
        QVERIFY(m_calc->hasError());
        QVERIFY(m_calc->lastError().contains("zero"));
    }

    void test_divide_decimal_result() {
        bool ok;
        double result = m_calc->divide(1, 3, &ok);
        QVERIFY(ok);
        QVERIFY(qAbs(result - 0.3333) < 0.001);
    }

    // ── Expression evaluator ──────────────────────────────
    void test_evaluate_addition() {
        bool ok;
        QCOMPARE(m_calc->evaluate("10 + 5", &ok), 15.0);
        QVERIFY(ok);
    }

    void test_evaluate_subtraction() {
        bool ok;
        QCOMPARE(m_calc->evaluate("20 - 8", &ok), 12.0);
        QVERIFY(ok);
    }

    void test_evaluate_multiplication() {
        bool ok;
        QCOMPARE(m_calc->evaluate("6 * 7", &ok), 42.0);
        QVERIFY(ok);
    }

    void test_evaluate_division() {
        bool ok;
        QCOMPARE(m_calc->evaluate("15 / 3", &ok), 5.0);
        QVERIFY(ok);
    }

    void test_evaluate_invalid_expression() {
        bool ok;
        m_calc->evaluate("hello world", &ok);
        QVERIFY(!ok);
        QVERIFY(m_calc->hasError());
    }

    void test_evaluate_division_by_zero() {
        bool ok;
        m_calc->evaluate("10 / 0", &ok);
        QVERIFY(!ok);
    }

    void test_evaluate_decimals() {
        bool ok;
        double result = m_calc->evaluate("3.5 * 2", &ok);
        QVERIFY(ok);
        QCOMPARE(result, 7.0);
    }

    // ── History ───────────────────────────────────────────
    void test_history_empty_initially() {
        QVERIFY(m_calc->history().isEmpty());
    }

    void test_history_records_operations() {
        m_calc->add(1, 2);
        m_calc->subtract(5, 3);
        QCOMPARE(m_calc->history().size(), 2);
    }

    void test_history_newest_first() {
        m_calc->add(1, 1);
        m_calc->add(2, 2);
        // Most recent (2+2) should be at index 0
        QVERIFY(m_calc->history().first().contains("4"));
    }

    void test_history_cleared() {
        m_calc->add(1, 2);
        m_calc->add(3, 4);
        m_calc->clearHistory();
        QVERIFY(m_calc->history().isEmpty());
    }

    // ── Signals ───────────────────────────────────────────
    void test_signal_resultReady_emitted() {
        QSignalSpy spy(m_calc, &Calculator::resultReady);
        m_calc->add(1, 2);
        QCOMPARE(spy.count(), 1);
    }

    void test_signal_errorOccurred_on_divByZero() {
        QSignalSpy spy(m_calc, &Calculator::errorOccurred);
        bool ok;
        m_calc->divide(5, 0, &ok);
        QCOMPARE(spy.count(), 1);
    }

    void test_signal_historyChanged_on_operation() {
        QSignalSpy spy(m_calc, &Calculator::historyChanged);
        m_calc->multiply(3, 4);
        QCOMPARE(spy.count(), 1);
    }

    void test_signal_historyChanged_on_clear() {
        m_calc->add(1, 2);
        QSignalSpy spy(m_calc, &Calculator::historyChanged);
        m_calc->clearHistory();
        QCOMPARE(spy.count(), 1);
    }

private:
    Calculator* m_calc = nullptr;
};

#endif // TEST_CALCULATOR_H
