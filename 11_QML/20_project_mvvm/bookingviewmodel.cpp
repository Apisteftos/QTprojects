#include "bookingviewmodel.h"

BookingViewModel::BookingViewModel(QObject *parent)
    : QObject(parent)
    , m_list(m_service)
{
    recalculate();
}

Booking BookingViewModel::current() const
{
    return Booking{m_guest.toStdString(), m_nights, m_breakfast};
}

QString BookingViewModel::priceText(int cents) const
{
    return QString::number(cents / 100.0, 'f', 2);
}

QString BookingViewModel::textFor(Problem problem)
{
    switch (problem) {
    case Problem::None:
        return {};
    case Problem::GuestTooShort:
        return tr("The name needs at least 2 characters");
    case Problem::GuestTooLong:
        return tr("The name has more than 30 characters");
    case Problem::NightsOutOfRange:
        return tr("Choose 1 to %1 nights").arg(BookingService::kMaxNights);
    }
    return {};
}

void BookingViewModel::recalculate()
{
    const Booking booking = current();
    const Problem problem = BookingService::check(booking);

    m_totalText = priceText(BookingService::price(booking));
    // Do not scold the user before he has typed anything
    m_error = m_guest.isEmpty() ? QString() : textFor(problem);
    m_canSave = problem == Problem::None;
    emit derivedChanged();
}

void BookingViewModel::setGuest(const QString &guest)
{
    if (guest == m_guest)
        return;
    m_guest = guest;
    emit guestChanged();
    recalculate();
}

void BookingViewModel::setNights(int nights)
{
    if (nights == m_nights)
        return;
    m_nights = nights;
    emit nightsChanged();
    recalculate();
}

void BookingViewModel::setBreakfast(bool breakfast)
{
    if (breakfast == m_breakfast)
        return;
    m_breakfast = breakfast;
    emit breakfastChanged();
    recalculate();
}

void BookingViewModel::setMessage(const QString &message)
{
    if (message == m_message)
        return;
    m_message = message;
    emit messageChanged();
}

void BookingViewModel::save()
{
    // The View disables the button, but the ViewModel does not trust that: the rule is checked again.
    if (!m_canSave)
        return;

    const Booking booking = current();
    m_list.append(booking);
    emit countChanged();
    setMessage(tr("Saved: %1, %2 nights, %3").arg(m_guest.trimmed()).arg(m_nights).arg(priceText(BookingService::price(booking))));

    // A new, empty form
    setGuest(QString());
    setNights(1);
    setBreakfast(false);
}

void BookingViewModel::remove(int row)
{
    const int before = count();
    m_list.removeAt(row);
    if (count() != before) {
        emit countChanged();
        setMessage(tr("Booking removed"));
    }
}

void BookingViewModel::clearAll()
{
    if (count() == 0)
        return;
    m_list.clear();
    emit countChanged();
    setMessage(tr("All bookings removed"));
}
