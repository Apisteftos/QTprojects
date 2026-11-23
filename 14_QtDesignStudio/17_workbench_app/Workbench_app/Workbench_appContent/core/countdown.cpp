#include "countdown.hpp"

#include <algorithm>
#include <format>

namespace wb {

std::string formatClock(int seconds)
{
    seconds = std::max(0, seconds);
    return std::format("{:02}:{:02}", seconds / 60, seconds % 60);
}

void Countdown::setTotal(int seconds)
{
    m_total = std::max(0, seconds);
    m_remaining = m_total;
    m_running = false;
}

void Countdown::start()
{
    // Start again from the full time if the last run is finished.
    if (m_remaining == 0)
        m_remaining = m_total;
    m_running = m_remaining > 0;
}

void Countdown::stop()
{
    m_running = false;
}

void Countdown::reset()
{
    m_remaining = m_total;
    m_running = false;
}

bool Countdown::tick(int seconds)
{
    if (!m_running || seconds <= 0)
        return false;

    m_remaining = std::max(0, m_remaining - seconds);
    if (m_remaining == 0) {
        m_running = false;
        return true;
    }
    return false;
}

double Countdown::progress() const
{
    if (m_total == 0)
        return 0.0;
    return 1.0 - static_cast<double>(m_remaining) / static_cast<double>(m_total);
}

std::string Countdown::text() const
{
    return formatClock(m_remaining);
}

} // namespace wb
