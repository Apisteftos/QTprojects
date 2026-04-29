# QTprojects

[![CI](https://github.com/Apisteftos/QTprojects/actions/workflows/ci.yml/badge.svg)](https://github.com/Apisteftos/QTprojects/actions/workflows/ci.yml)

A collection of Qt6 / C++17 demo projects covering signals & slots, MVC, MVVM/QML,
networking, SQL, multithreading, CI/CD, and automated testing.
Built on **Ubuntu 24.04** with **Qt 6** and **CMake**.

---

## Repository Structure

```
QTprojects/
├── .github/workflows/      ← GitHub Actions CI pipeline
├── qt_signals_demo/        ← Signals & slots, Observer pattern
├── qt_mvc_demo/            ← MVC architecture with Qt Widgets
├── qt_mvvm_demo/           ← MVVM with QML + C++ ViewModel
├── qt_networking_demo/     ← TCP/UDP client-server (4 projects)
│   ├── qt_client/          ← TCP chat client (MVP pattern)
│   ├── qt_server/          ← TCP server, multi-client broadcast
│   ├── qt_udp_receiver/    ← UDP receiver
│   └── qt_udp_sender/      ← UDP sender
├── qt_sql_demo/            ← SQLite with QSqlDatabase
├── qt_http_demo/           ← HTTP REST with QNetworkAccessManager
├── qt_thread_demo/         ← QThread, worker threads
├── qt_test_demo/           ← QTest unit testing (40 tests)
├── qt_cicd_demo/           ← CI/CD pipeline demo (32 tests)
│   └── tests/              ← QTest calculator test suite
├── Qt_test_design/         ← Qt Design Studio UI prototype
└── qt_qml_dashboard/       ← QML animated dashboard
```

---

## Projects

### qt_signals_demo
Signals & slots fundamentals with three progressively complex examples:
- Basic signal/slot connections between custom `QObject` subclasses
- Lambda slots and connections across threads
- Observer pattern implemented entirely with Qt signals

### qt_mvc_demo
MVC architecture with Qt Widgets:
- `Model` — pure data/business logic, no Qt UI dependency
- `View` — Qt Widgets UI, emits user actions as signals
- `Controller` — connects Model and View, owns the application logic
- Mermaid architecture diagram in project README

### qt_mvvm_demo
MVVM pattern with QML data binding:
- C++ `ViewModel` exposes `Q_PROPERTY` to QML
- QML View binds directly to ViewModel properties — no manual `connect()`
- Demonstrates the difference between Qt Widgets MVP and QML MVVM
- Relevant for embedded HMI and automotive dashboard development

### qt_networking_demo
TCP and UDP networking — four separate projects:

| Project | Protocol | Pattern |
|---------|----------|---------|
| `qt_server` | TCP | Console app, `QTcpServer`, one `ClientHandler` per connection |
| `qt_client` | TCP | Widgets app, full **MVP pattern**, chat UI |
| `qt_udp_sender` | UDP | Datagram sender with configurable target |
| `qt_udp_receiver` | UDP | Datagram receiver, live display |

The TCP client uses stream buffering (`while (buffer.contains('\n'))`) —
the same pattern used in production MCX/MCPTT protocol implementations.

### qt_sql_demo
SQLite database integration:
- `QSqlDatabase` connection management
- `QSqlQuery` for CRUD operations
- `QSqlTableModel` for direct widget binding
- Schema migrations and error handling

### qt_http_demo
HTTP REST client:
- `QNetworkAccessManager` for async GET/POST requests
- JSON parsing with `QJsonDocument`
- Loading indicator and error handling pattern

### qt_thread_demo
Multithreading with Qt:
- `QThread` worker pattern (object moved to thread)
- `QMutex` and `QWaitCondition`
- `QtConcurrent::run` for simple async tasks
- Thread-safe signal/slot connections across threads

### qt_test_demo
Unit testing with QTest — **40 automated tests**:
```
TestNoteRepository (20 tests)   — CRUD, SQL backend, signals
TestNoteModel (20 tests)        — cache layer, search, signals
```

### qt_cicd_demo
CI/CD pipeline demonstration — **32 automated tests**:
```
TestCalculator
├── Addition        (5 tests)
├── Subtraction     (3 tests)
├── Multiplication  (4 tests)
├── Division        (3 tests)
├── Expression      (7 tests)
├── History         (4 tests)
└── Signals         (4 tests) — resultReady, error, historyChanged
```
Every push to `main` triggers the GitHub Actions pipeline.
Both `qt_test_demo` and `qt_cicd_demo` tests run automatically.
**Total: 72 tests on every push.**

### Qt_test_design
UI prototype built with Qt Design Studio:
- Declarative QML layout with custom fonts and images
- Demonstrates the Qt Design Studio → Qt Creator handoff workflow

### qt_qml_dashboard
Animated QML dashboard:
- Live-updating gauges and indicators
- Property animations and state machines in QML
- C++ backend feeding data to QML via `Q_PROPERTY`

---

## CI/CD Pipeline

```yaml
# Runs on every push and pull request to main
# Ubuntu + Qt6 + CMake + QTest
```

The pipeline builds and runs all tests headlessly:
```bash
QT_QPA_PLATFORM=offscreen ./build/qt_test_demo
QT_QPA_PLATFORM=offscreen ./build/qt_cicd_demo_tests
```

See `.github/workflows/ci.yml` for full configuration.

---

## Environment

| Item | Value |
|------|-------|
| OS | Ubuntu 24.04 |
| Qt | Qt 6 |
| C++ Standard | C++17 |
| Build system | CMake |
| IDE | Qt Creator |
| Testing | QTest |
| CI | GitHub Actions |

---

## Learning Path

```
1. qt_signals_demo      ← Start here — Qt fundamentals
2. qt_mvc_demo          ← Architecture patterns
3. qt_mvvm_demo         ← QML + data binding
4. qt_networking_demo   ← TCP/UDP, real protocol patterns
5. qt_sql_demo          ← Persistence layer
6. qt_http_demo         ← REST / async networking
7. qt_thread_demo       ← Concurrency
8. qt_test_demo         ← Testing fundamentals
9. qt_cicd_demo         ← CI/CD pipeline
10. qt_qml_dashboard    ← Advanced QML
11. Qt_test_design      ← Qt Design Studio workflow
```

---

## License

Copyright © 2024–2026 **Konstantinos Angeloulis**

This work is licensed under the
**[Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License (CC BY-NC-ND 4.0)](https://creativecommons.org/licenses/by-nc-nd/4.0/)**.

[![CC BY-NC-ND 4.0](https://licensebuttons.net/l/by-nc-nd/4.0/88x31.png)](https://creativecommons.org/licenses/by-nc-nd/4.0/)

You may read and share this material with attribution, but you may **not**
use it commercially or publish modified versions of it.

See [LICENSE.md](LICENSE.md) for full terms.
