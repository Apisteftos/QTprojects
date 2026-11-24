#include "booking.h"

#include <cctype>

namespace {

// The guest name without spaces at the ends, counted in characters (not in bytes):
// in UTF-8 a letter like "ë" takes two bytes, and the bytes that continue a letter start with 10xxxxxx.
int lengthInCharacters(const std::string &text)
{
    const auto isSpace = [](char c) { return std::isspace(static_cast<unsigned char>(c)) != 0; };

    std::size_t first = 0;
    std::size_t last = text.size();
    while (first < last && isSpace(text[first]))
        ++first;
    while (last > first && isSpace(text[last - 1]))
        --last;

    int characters = 0;
    for (std::size_t i = first; i < last; ++i) {
        if ((static_cast<unsigned char>(text[i]) & 0xC0) != 0x80)
            ++characters;
    }
    return characters;
}

} // namespace

Problem BookingService::check(const Booking &booking)
{
    const int length = lengthInCharacters(booking.guest);
    if (length < 2)
        return Problem::GuestTooShort;
    if (length > 30)
        return Problem::GuestTooLong;
    if (booking.nights < 1 || booking.nights > kMaxNights)
        return Problem::NightsOutOfRange;
    return Problem::None;
}

int BookingService::price(const Booking &booking)
{
    const int perNight = kNightlyRate + (booking.breakfast ? kBreakfastRate : 0);
    return perNight * booking.nights;
}

void BookingService::add(const Booking &booking)
{
    m_bookings.push_back(booking);
}

bool BookingService::removeAt(std::size_t row)
{
    if (row >= m_bookings.size())
        return false;
    m_bookings.erase(m_bookings.begin() + static_cast<std::ptrdiff_t>(row));
    return true;
}
