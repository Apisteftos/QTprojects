// 04_Event_Loop: what exec() does, and why a slow handler freezes everything.

#include <QCoreApplication>
#include <QDebug>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QThread>
#include <QTimer>

static void say(const QString &text) { qDebug().noquote() << text; }

static void title(const QString &text)
{
    qDebug().noquote() << "";
    qDebug().noquote() << "=== " + text + " ===";
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // === EXAMPLE 1: nothing runs until the loop runs ===
    title("Example 1: events wait for the loop");
    {
        QEventLoop loop;
        say("a: before exec()");
        QTimer::singleShot(0, &loop, [&loop] {
            say("c: runs inside the loop");
            loop.quit();
        });
        say("b: the timer is set, but nothing has run yet");
        loop.exec();
        say("d: exec() returned because quit() was called");
    }

    // === EXAMPLE 2: a repeating timer ===
    title("Example 2: a repeating timer");
    {
        QEventLoop loop;
        QTimer timer;
        timer.setInterval(50);
        int ticks = 0;
        QObject::connect(&timer, &QTimer::timeout, &loop, [&] {
            ++ticks;
            say(QString("tick %1").arg(ticks));
            if (ticks == 3) {
                timer.stop();
                loop.quit();
            }
        });
        timer.start();
        loop.exec();
        say("the timer stopped after 3 ticks");
    }

    // === EXAMPLE 3: a slow handler blocks the loop ===
    title("Example 3: a slow handler blocks everything");
    {
        QEventLoop loop;
        QElapsedTimer clock;
        clock.start();

        QTimer::singleShot(100, &loop, [] {
            say("A runs at 100 ms and then sleeps for 300 ms");
            QThread::msleep(300);
        });
        QTimer::singleShot(150, &loop, [&] {
            say(QString("B was due at 150 ms, it ran after 400 ms or more: %1")
                    .arg(clock.elapsed() >= 400 ? "true" : "false"));
            loop.quit();
        });
        loop.exec();
    }

    // === EXAMPLE 4: queued calls ===
    title("Example 4: a queued call is only posted");
    {
        QMetaObject::invokeMethod(
            &app, [] { say("the queued call runs now"); }, Qt::QueuedConnection);
        say("invokeMethod() returned, the call has not run yet");
        QCoreApplication::processEvents();
        say("processEvents() returned");
    }

    // === EXAMPLE 5: the exit code ===
    title("Example 5: exit() and the return value of exec()");
    QTimer::singleShot(0, &app, [] { QCoreApplication::exit(7); });
    const int code = app.exec();
    say(QString("app.exec() returned %1").arg(code));

    return 0;
}
