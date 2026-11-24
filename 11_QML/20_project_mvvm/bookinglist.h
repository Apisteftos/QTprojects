#ifndef BOOKINGLIST_H
#define BOOKINGLIST_H

#include "booking.h"

#include <QAbstractListModel>

// The list of saved bookings as a Qt model (lesson 14). It shows the data of the BookingService.
// The service is changed only through this class, between beginInsertRows/endInsertRows,
// so that every view hears about every change.
class BookingList : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Role { GuestRole = Qt::UserRole + 1, NightsRole, BreakfastRole, PriceRole };

    explicit BookingList(BookingService &service, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void append(const Booking &booking);
    void removeAt(int row);
    void clear();

private:
    BookingService &m_service;
};

#endif // BOOKINGLIST_H
