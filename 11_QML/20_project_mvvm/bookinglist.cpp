#include "bookinglist.h"

BookingList::BookingList(BookingService &service, QObject *parent)
    : QAbstractListModel(parent)
    , m_service(service)
{
}

int BookingList::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_service.bookings().size());
}

QVariant BookingList::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid | CheckIndexOption::ParentIsInvalid))
        return {};

    const Booking &booking = m_service.bookings()[static_cast<std::size_t>(index.row())];
    switch (role) {
    case GuestRole:
        return QString::fromStdString(booking.guest).trimmed();
    case NightsRole:
        return booking.nights;
    case BreakfastRole:
        return booking.breakfast;
    case PriceRole:
        return BookingService::price(booking);       // cents; the ViewModel's helper makes text for the view
    default:
        return {};
    }
}

QHash<int, QByteArray> BookingList::roleNames() const
{
    return {
        {GuestRole, "guest"},
        {NightsRole, "nights"},
        {BreakfastRole, "breakfast"},
        {PriceRole, "price"},
    };
}

void BookingList::append(const Booking &booking)
{
    const int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    m_service.add(booking);
    endInsertRows();
}

void BookingList::removeAt(int row)
{
    if (row < 0 || row >= rowCount())
        return;
    beginRemoveRows(QModelIndex(), row, row);
    m_service.removeAt(static_cast<std::size_t>(row));
    endRemoveRows();
}

void BookingList::clear()
{
    beginResetModel();
    while (m_service.removeAt(0)) {}
    endResetModel();
}
