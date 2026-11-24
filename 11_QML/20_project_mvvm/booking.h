#ifndef BOOKING_H
#define BOOKING_H

#include <cstddef>
#include <string>
#include <vector>

// === THE MODEL ===
// Plain C++. No Qt, no QObject, no signals: the rules of the business and the data.
// It can be tested and used in a program without any interface.

struct Booking
{
    std::string guest;
    int nights = 1;
    bool breakfast = false;
};

// The model says WHAT is wrong, not HOW to say it. The words belong to the ViewModel (translation!).
enum class Problem
{
    None,
    GuestTooShort,
    GuestTooLong,
    NightsOutOfRange
};

class BookingService
{
public:
    static constexpr int kNightlyRate = 8900;     // cents per night
    static constexpr int kBreakfastRate = 1200;   // cents per night
    static constexpr int kMaxNights = 14;

    static Problem check(const Booking &booking);   // the rules
    static int price(const Booking &booking);       // in cents

    void add(const Booking &booking);
    bool removeAt(std::size_t row);
    const std::vector<Booking> &bookings() const { return m_bookings; }

private:
    std::vector<Booking> m_bookings;
};

#endif // BOOKING_H
