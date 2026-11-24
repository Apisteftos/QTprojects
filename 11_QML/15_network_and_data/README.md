# 15_network_and_data

Most real applications show data that comes from somewhere else: a web service, a file, a device. The rule of this chapter still holds: **C++ gets and understands the data, QML shows it.** This lesson builds a C++ backend that loads a JSON document through `QNetworkAccessManager` and gives it to a QML `ListView` as a model, with a loading state and error messages.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** also built with GCC 13.3 against Qt 6.4.2 (with small changes for that old version, which are not part of this folder; one is an explicit `import network_and_data` in `Main.qml`, which Qt 6.8 does not need). An automated check tested `StationClient` with the JSON resource, with a **local HTTP server started by the test** (a good answer, a 404, an answer that is not JSON, a closed port), the parser alone, and `abort()`. It also clicked through the window (38 checks, all passed). The program never needs the internet. Please build it once with your own 6.8+ kit.

---

## 1. What You Build

A list of weather stations (name, city, temperature) with a status line, a **Reload** button, a busy indicator, and a field where you can type another URL.

The data are in `data/stations.json`, which is **a resource of the program**, so the lesson works offline. The program reads it with the same code that reads a web address. To use a real server, change the URL (in the field, or in `Main.qml`): the backend does not care where the JSON comes from.

---

## 2. The Backend: `StationClient`

| Part | Job |
|------|-----|
| `source` (`QUrl`) | Where the data come from. A normal property |
| `load()`, `abort()` | `Q_INVOKABLE`: start and cancel a request |
| `loading`, `error` | The state that the interface shows |
| `count`, `average`, `updated` | Values taken from the data |
| the model (`rowCount`, `data`, `roleNames`) | The stations as rows, with the roles `name`, `city`, `temperature` |
| `parse(const QByteArray &)` | Turns JSON bytes into rows. It needs **no network**, so it can be tested alone |

The class is a `QAbstractListModel` (lesson 14) **and** has properties (lesson 12). One object is both the service and the model, which is fine for a small program. In a bigger one, the service and the model are two classes.

The interface has **no** network code, no JSON, no error codes. It binds to `stations.loading`, `stations.error` and the model, and calls `stations.load()`.

---

## 3. Asking for Data: `QNetworkAccessManager`

```cpp
QNetworkRequest request(m_source);
request.setTransferTimeout(5000);
m_reply = m_network.get(request);
connect(m_reply, &QNetworkReply::finished, this, [this, reply = m_reply.data()] { finished(reply); });
```

- **Asynchronous**: `get()` returns at once with a `QNetworkReply`. The program goes on (the event loop runs), and the reply sends `finished()` when it is ready. Never wait in a loop for a network answer: the interface would freeze. (Chapter `10_IO_and_Data`, lesson 04 is about HTTP in more detail.)
- **One manager** is enough for the whole program (it is a member here). It also does the connection reuse and the cache.
- **Timeout**: without `setTransferTimeout`, a request to a dead server can wait for a very long time.
- **Same code for URLs of every kind**: `https://...`, `http://...`, `file:///...` and `qrc:/...` all work with `get()`. The sample data use `qrc:/data/stations.json`.
- Needs the Qt module **Network**: `find_package(Qt6 REQUIRED COMPONENTS Quick Network)` and `Qt6::Network` in `target_link_libraries`. For `https` the Qt installation needs a TLS backend (OpenSSL or the system one); a Qt build from the installer has it.

### 3.1 The answer

```cpp
void StationClient::finished(QNetworkReply *reply)
{
    reply->deleteLater();                 // never "delete reply" inside its own signal
    if (reply != m_reply) return;         // an old request that was aborted
    ...
    if (reply->error() != QNetworkReply::NoError) { setError(reply->errorString()); return; }
    setError(parse(reply->readAll()));
}
```

- `reply->error()` is **not** `NoError` for a refused connection, a timeout, and also for an HTTP status like 404 or 500. The test saw `Connection refused`, `... server replied: X` (the text of the status line the test server sent), and `Error opening :/data/missing.json: No such file or directory`. The wording of the messages depends on the system and the Qt version.
- `reply->deleteLater()` is the correct way to free a reply. The reply is **yours**: if you forget it, the program leaks it.
- **Ignore the old answer.** `abort()` clears `m_reply` first. When the aborted reply finishes anyway, `finished()` compares it with `m_reply` and returns. Without this, a slow old answer could replace the data of a newer request.
- A `QPointer<QNetworkReply>` becomes `nullptr` by itself when the reply is deleted, so the pointer is never dangling.

---

## 4. Understanding the Data: JSON

`parse()` is the same as in chapter `10_IO_and_Data`, lesson 01:

```cpp
const QJsonDocument doc = QJsonDocument::fromJson(json, &parseError);
const QJsonArray array = doc.object().value("stations").toArray();
```

Three rules for data that come from outside:

1. **Never trust them.** `fromJson` can fail (the test sent `<html>hi</html>`), the key can be missing, and a value can have another type. `parse()` returns a message for the first two and uses defaults for the third (`toDouble()` of a missing value is `0`).
2. **Replace the data in one step.** The new list is built first, and then `beginResetModel()` / `endResetModel()` swap it in. If the data are bad, the old list stays: the test saw that after the 404 the five old stations were still there.
3. **Say what went wrong in words** a user can read. `error` is shown in red in the interface.

---

## 5. The State in QML

```qml
text: stations.loading ? qsTr("Loading ...")
    : stations.error !== "" ? qsTr("Error: %1").arg(stations.error)
    : qsTr("%1 stations, average %2 °C, updated %3").arg(...)
```

An interface that loads data always has **four situations**: nothing yet, loading, an error, and a result. Design all four. Here they come from two properties (`loading` and `error`) and the count. `BusyIndicator { running: stations.loading }` and the button `enabled: !stations.loading` are bindings on the same state.

`Component.onCompleted: stations.load()` starts the first request (lesson 11).

---

## 6. Other Sources of Data

| Source | Tool |
|--------|------|
| A web service (REST, JSON) | `QNetworkAccessManager`, as here. In QML only, `XMLHttpRequest` exists, but the logic belongs in C++ |
| A TCP or UDP connection | `QTcpSocket`, `QUdpSocket` (chapter `10_IO_and_Data`, lesson 03) |
| A WebSocket | `QWebSocket` from the module Qt WebSockets |
| A local file | `QFile` (or the same `get()` with `file:///`) |
| A database | Qt SQL (chapter `10_IO_and_Data`, lesson 02) |
| A serial port or Bluetooth | Qt Serial Port, Qt Bluetooth |

For all of them, the pattern is the same: a C++ class owns the connection, turns the data into properties and models, and sends signals. QML only shows it.

Be careful with **secrets**: do not write an API key or a password into a QML file or the source of a public repository. A QML file in a resource can be read by anyone who has the program.

---

## 7. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app15_network_and_data
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Run the program and press **Reload**.
2. Type `qrc:/data/nothing.json` into the field and press **Load URL**. Read the red message. Then type the right URL again.
3. Type `file:///` and the full path of another JSON file with the same layout (a `stations` list), and load it.
4. Make a copy of `stations.json` with a missing comma. What does the interface say?
5. Start a local server in a terminal, in a folder with the JSON file: `python3 -m http.server 8000`, and load `http://127.0.0.1:8000/stations.json`.
6. Add a role `city` filter: a `TextField` that sets a property `filter` of the client, so that only matching stations are shown. (Or a `QSortFilterProxyModel`: chapter `07_ModelView`.)

---

## 8. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| Linker error `undefined reference to QNetworkAccessManager` | `Qt6::Network` is not linked | Add it to `find_package` and `target_link_libraries` |
| `TLS initialization failed` / `https` does not work | The Qt installation has no TLS backend | Use a Qt build with OpenSSL or the system TLS support |
| The window freezes while loading | The code waits for the reply in a loop | Use the signal `finished` |
| The list is empty and there is no error | The JSON has other key names | Check the keys (`"stations"`, `"name"`) |
| The program uses more and more memory | `deleteLater()` is missing for the replies | Call it in every case |
| An old answer replaces a new one | Two requests are running, and the first finishes last | `abort()` the old one, and ignore replies that are not `m_reply` |
| Crash in the `finished` slot | The reply was deleted inside its own signal | `deleteLater()`, not `delete` |
| The request never ends | There is no timeout | `setTransferTimeout()` |
| `qrc:/data/...` is not found | The resource is not in CMake, or the prefix is different | Check `qt_add_resources` and use `QFile::exists(":/data/stations.json")` |
| CORS or certificate errors | These belong to browsers and to wrong certificates | In C++, fix the certificate; never switch the check off in a real program |

---

## 9. Reference

| Topic | Where to look |
|-------|---------------|
| Qt Network | <https://doc.qt.io/qt-6/qtnetwork-index.html> |
| `QNetworkAccessManager` | <https://doc.qt.io/qt-6/qnetworkaccessmanager.html> |
| `QNetworkReply`, `QNetworkRequest` | <https://doc.qt.io/qt-6/qnetworkreply.html>, <https://doc.qt.io/qt-6/qnetworkrequest.html> |
| JSON support | <https://doc.qt.io/qt-6/json.html> |
| `QAbstractListModel` | <https://doc.qt.io/qt-6/qabstractlistmodel.html> |
| `qt_add_resources` | <https://doc.qt.io/qt-6/qt-add-resources.html> |
| `BusyIndicator` | <https://doc.qt.io/qt-6/qml-qtquick-controls-busyindicator.html> |
| Module | Qt Quick and Qt Network (`find_package(Qt6 REQUIRED COMPONENTS Quick Network)`) |
| Headers | `<QNetworkAccessManager>`, `<QNetworkReply>`, `<QNetworkRequest>`, `<QJsonDocument>`, `<QJsonObject>`, `<QJsonArray>`, `<QAbstractListModel>` |
| Imports | `import QtQuick`, `import QtQuick.Controls`, `import QtQuick.Layouts` |
| Minimum Qt version | Qt 6.8 |
