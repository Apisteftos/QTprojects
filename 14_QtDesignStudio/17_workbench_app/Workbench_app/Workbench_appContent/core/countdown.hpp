#pragma once

#include <string>

namespace wb {

// "MM:SS" for a number of seconds, for example 272 -> "04:32".
// Negative values are shown as 00:00. Minutes can grow beyond 59.
std::string formatClock(int seconds);

// A countdown in whole seconds. It does not know about time: the caller
// calls tick() once per second (a QTimer in the application, a loop in a test).
class Countdown {
public:
    // Sets the total time and resets the countdown.
    void setTotal(int seconds);

    void start();
    void stop();
    void reset();

    // Moves time forward. Returns true exactly once, when the time reaches 0.
    // Does nothing while the countdown is stopped.
    bool tick(int seconds = 1);

    int total() const { return m_total; }
    int remaining() const { return m_remaining; }
    bool running() const { return m_running; }

    // 0.0 at the start, 1.0 when finished. 0.0 if the total is 0.
    double progress() const;

    // "MM:SS", for example "04:32".
    std::string text() const;

private:
    int m_total = 0;
    int m_remaining = 0;
    bool m_running = false;
};

} // namespace wb
