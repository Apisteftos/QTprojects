// ============================================================
// tst_booking.cpp
// Tests of the Model and the ViewModel. There is no window and no QML here:
// that is the gain of MVVM. Everything the interface does can be tested in C++.
// ============================================================

#include "booking.h"
#include "bookinglist.h"
#include "bookingviewmodel.h"

#include <QAbstractItemModelTester>
#include <QSignalSpy>
#include <QtTest>

Q_DECLARE_METATYPE(Problem)

class TstBooking : public QObject
{
    Q_OBJECT

private slots:
    // --- Model ---
    void price_data();
    void price();
    void check_data();
    void check();
    void serviceAddAndRemove();

    // --- BookingList ---
    void listIsAValidModel();

    // --- ViewModel ---
    void startState();
    void typingChangesTheDerivedValues();
    void breakfastChangesThePrice();
    void errorTexts();
    void saveAddsARowAndClearsTheForm();
    void saveIsRefusedWhenInvalid();
    void removeAndClear();
    void signalsAreSentOnce();
};

void TstBooking::price_data()
{
    QTest::addColumn<int>("nights");
    QTest::addColumn<bool>("breakfast");
    QTest::addColumn<int>("cents");

    QTest::newRow("one night") << 1 << false << 8900;
    QTest::newRow("three nights") << 3 << false << 26700;
    QTest::newRow("one night, breakfast") << 1 << true << 10100;
    QTest::newRow("three nights, breakfast") << 3 << true << 30300;
}

void TstBooking::price()
{
    QFETCH(int, nights);
    QFETCH(bool, breakfast);
    QFETCH(int, cents);
    QCOMPARE(BookingService::price(Booking{"Al", nights, breakfast}), cents);
}

void TstBooking::check_data()
{
    QTest::addColumn<QString>("guest");
    QTest::addColumn<int>("nights");
    QTest::addColumn<Problem>("expected");

    QTest::newRow("fine") << "Al" << 1 << Problem::None;
    QTest::newRow("empty") << "" << 1 << Problem::GuestTooShort;
    QTest::newRow("one letter") << "A" << 1 << Problem::GuestTooShort;
    QTest::newRow("spaces do not count") << "  A  " << 1 << Problem::GuestTooShort;
    QTest::newRow("umlaut counts as one") << QString::fromUtf8("Zoë") << 1 << Problem::None;
    QTest::newRow("two letters with umlaut") << QString::fromUtf8("öä") << 1 << Problem::None;
    QTest::newRow("30 characters") << QString(30, 'a') << 1 << Problem::None;
    QTest::newRow("31 characters") << QString(31, 'a') << 1 << Problem::GuestTooLong;
    QTest::newRow("0 nights") << "Al" << 0 << Problem::NightsOutOfRange;
    QTest::newRow("14 nights") << "Al" << 14 << Problem::None;
    QTest::newRow("15 nights") << "Al" << 15 << Problem::NightsOutOfRange;
}

void TstBooking::check()
{
    QFETCH(QString, guest);
    QFETCH(int, nights);
    QFETCH(Problem, expected);
    QCOMPARE(BookingService::check(Booking{guest.toStdString(), nights, false}), expected);
}

void TstBooking::serviceAddAndRemove()
{
    BookingService service;
    service.add({"Al", 2, false});
    service.add({"Bo", 3, true});
    QCOMPARE(service.bookings().size(), std::size_t{2});
    QVERIFY(service.removeAt(0));
    QCOMPARE(service.bookings().size(), std::size_t{1});
    QCOMPARE(QString::fromStdString(service.bookings().front().guest), QString("Bo"));
    QVERIFY(!service.removeAt(5));
}

void TstBooking::listIsAValidModel()
{
    BookingService service;
    BookingList list(service);
    QAbstractItemModelTester tester(&list);      // checks the rules of a model while we use it

    QSignalSpy inserted(&list, &QAbstractItemModel::rowsInserted);
    QSignalSpy removed(&list, &QAbstractItemModel::rowsRemoved);
    list.append({"Al", 2, false});
    list.append({"Bo", 3, true});
    QCOMPARE(list.rowCount(), 2);
    QCOMPARE(inserted.count(), 2);
    QCOMPARE(list.data(list.index(1), BookingList::GuestRole).toString(), QString("Bo"));
    QCOMPARE(list.data(list.index(1), BookingList::PriceRole).toInt(), 30300);
    list.removeAt(0);
    QCOMPARE(removed.count(), 1);
    list.removeAt(7);                              // not there: nothing happens
    QCOMPARE(removed.count(), 1);
    list.clear();
    QCOMPARE(list.rowCount(), 0);
}

void TstBooking::startState()
{
    BookingViewModel vm;
    QCOMPARE(vm.guest(), QString());
    QCOMPARE(vm.nights(), 1);
    QVERIFY(!vm.breakfast());
    QVERIFY(!vm.canSave());
    QCOMPARE(vm.error(), QString());               // no scolding before the user has typed
    QCOMPARE(vm.totalText(), QString("89.00"));
    QCOMPARE(vm.count(), 0);
}

void TstBooking::typingChangesTheDerivedValues()
{
    BookingViewModel vm;
    vm.setGuest("A");
    QVERIFY(!vm.canSave());
    QVERIFY(!vm.error().isEmpty());
    vm.setGuest("Al");
    QVERIFY(vm.canSave());
    QCOMPARE(vm.error(), QString());
    vm.setNights(3);
    QCOMPARE(vm.totalText(), QString("267.00"));
}

void TstBooking::breakfastChangesThePrice()
{
    BookingViewModel vm;
    vm.setNights(2);
    vm.setBreakfast(true);
    QCOMPARE(vm.totalText(), QString("202.00"));
}

void TstBooking::errorTexts()
{
    BookingViewModel vm;
    vm.setGuest("A");
    QCOMPARE(vm.error(), QString("The name needs at least 2 characters"));
    vm.setGuest(QString(31, 'x'));
    QCOMPARE(vm.error(), QString("The name has more than 30 characters"));
    vm.setGuest("Al");
    vm.setNights(0);
    QCOMPARE(vm.error(), QString("Choose 1 to 14 nights"));
    QVERIFY(!vm.canSave());
}

void TstBooking::saveAddsARowAndClearsTheForm()
{
    BookingViewModel vm;
    QSignalSpy countSpy(&vm, &BookingViewModel::countChanged);
    vm.setGuest("  Al  ");
    vm.setNights(3);
    vm.setBreakfast(true);
    vm.save();

    QCOMPARE(vm.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(vm.message(), QString("Saved: Al, 3 nights, 303.00"));
    // the form is empty again
    QCOMPARE(vm.guest(), QString());
    QCOMPARE(vm.nights(), 1);
    QVERIFY(!vm.breakfast());
    QVERIFY(!vm.canSave());
    QCOMPARE(vm.bookings()->data(vm.bookings()->index(0), BookingList::GuestRole).toString(), QString("Al"));
}

void TstBooking::saveIsRefusedWhenInvalid()
{
    BookingViewModel vm;
    vm.setGuest("A");
    vm.save();                                     // the button would be disabled, but save() is called anyway
    QCOMPARE(vm.count(), 0);
    QCOMPARE(vm.message(), QString());
    QCOMPARE(vm.guest(), QString("A"));            // the form is not touched
}

void TstBooking::removeAndClear()
{
    BookingViewModel vm;
    for (const char *name : {"Al", "Bo", "Cy"}) {
        vm.setGuest(name);
        vm.save();
    }
    QCOMPARE(vm.count(), 3);
    vm.remove(1);
    QCOMPARE(vm.count(), 2);
    QCOMPARE(vm.bookings()->data(vm.bookings()->index(1), BookingList::GuestRole).toString(), QString("Cy"));
    vm.remove(9);                                  // not there
    QCOMPARE(vm.count(), 2);
    vm.clearAll();
    QCOMPARE(vm.count(), 0);
    QCOMPARE(vm.message(), QString("All bookings removed"));
}

void TstBooking::signalsAreSentOnce()
{
    BookingViewModel vm;
    QSignalSpy guestSpy(&vm, &BookingViewModel::guestChanged);
    QSignalSpy derivedSpy(&vm, &BookingViewModel::derivedChanged);
    vm.setGuest("Al");
    vm.setGuest("Al");                             // the same value: no signal
    QCOMPARE(guestSpy.count(), 1);
    QCOMPARE(derivedSpy.count(), 1);
}

QTEST_MAIN(TstBooking)
#include "tst_booking.moc"
