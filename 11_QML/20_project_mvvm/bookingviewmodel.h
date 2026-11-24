#ifndef BOOKINGVIEWMODEL_H
#define BOOKINGVIEWMODEL_H

#include "booking.h"
#include "bookinglist.h"

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

// === THE VIEWMODEL ===
// It is the model of the VIEW: everything the interface shows, and every action it can do.
// - It knows the Model (BookingService). It does NOT know any QML file.
// - The View (QML) knows the ViewModel. It does NOT know the Model, and it contains no rules.
class BookingViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("The ViewModel is created in main.cpp and given to QML")

    // The form: what the user edits
    Q_PROPERTY(QString guest READ guest WRITE setGuest NOTIFY guestChanged)
    Q_PROPERTY(int nights READ nights WRITE setNights NOTIFY nightsChanged)
    Q_PROPERTY(bool breakfast READ breakfast WRITE setBreakfast NOTIFY breakfastChanged)

    // What the form means: calculated from the form by the rules of the Model
    Q_PROPERTY(QString totalText READ totalText NOTIFY derivedChanged)
    Q_PROPERTY(QString error READ error NOTIFY derivedChanged)
    Q_PROPERTY(bool canSave READ canSave NOTIFY derivedChanged)

    // The result
    Q_PROPERTY(BookingList *bookings READ bookings CONSTANT)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)

public:
    explicit BookingViewModel(QObject *parent = nullptr);

    QString guest() const { return m_guest; }
    void setGuest(const QString &guest);

    int nights() const { return m_nights; }
    void setNights(int nights);

    bool breakfast() const { return m_breakfast; }
    void setBreakfast(bool breakfast);

    QString totalText() const { return m_totalText; }
    QString error() const { return m_error; }
    bool canSave() const { return m_canSave; }

    BookingList *bookings() { return &m_list; }
    int count() const { return m_list.rowCount(); }
    QString message() const { return m_message; }

    // The commands of the View
    Q_INVOKABLE void save();
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE void clearAll();
    Q_INVOKABLE QString priceText(int cents) const;    // for the list: cents as text

signals:
    void guestChanged();
    void nightsChanged();
    void breakfastChanged();
    void derivedChanged();
    void countChanged();
    void messageChanged();

private:
    Booking current() const;
    void recalculate();
    void setMessage(const QString &message);
    static QString textFor(Problem problem);

    BookingService m_service;      // the Model. It must be declared BEFORE the list that uses it.
    BookingList m_list;

    QString m_guest;
    int m_nights = 1;
    bool m_breakfast = false;

    QString m_totalText;
    QString m_error;
    bool m_canSave = false;
    QString m_message;
};

#endif // BOOKINGVIEWMODEL_H
