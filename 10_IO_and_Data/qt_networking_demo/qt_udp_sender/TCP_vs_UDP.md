# TCP vs UDP — Complete Guide

A detailed comparison of the two fundamental transport protocols,
with diagrams, code examples, and real-world use cases.

---

## 1. The Big Picture

```mermaid
flowchart TD
    IP[IP — Internet Protocol\nroutes packets between machines] 

    TCP[TCP\nTransmission Control Protocol\nordered · reliable · connected]
    UDP[UDP\nUser Datagram Protocol\nfast · lightweight · connectionless]

    APP1[Web · Email · File Transfer\nChat · SSH · Databases]
    APP2[Video Streaming · VoIP · Gaming\nRadar · Telemetry · DNS]

    IP --> TCP
    IP --> UDP
    TCP --> APP1
    UDP --> APP2

    style IP fill:#404040,color:#fff
    style TCP fill:#1F4E79,color:#fff
    style UDP fill:#2E75B6,color:#fff
```

Both TCP and UDP sit on top of IP. They solve the same basic problem —
getting data from one machine to another — but with completely different
trade-offs.

---

## 2. Connection Model

### TCP — Three-Way Handshake

Before any data flows, TCP establishes a connection:

```mermaid
sequenceDiagram
    participant C as Client
    participant S as Server

    Note over C,S: Three-Way Handshake
    C->>S: SYN (I want to connect)
    S->>C: SYN-ACK (OK, I acknowledge)
    C->>S: ACK (confirmed)

    Note over C,S: Connection established — data flows
    C->>S: Data packet 1
    S->>C: ACK (received packet 1)
    C->>S: Data packet 2
    S->>C: ACK (received packet 2)

    Note over C,S: Teardown
    C->>S: FIN (I'm done)
    S->>C: ACK + FIN
    C->>S: ACK
```

**Cost:** ~1.5 round trips before first byte of data.
**Benefit:** Both sides know the connection is alive and ready.

---

### UDP — No Handshake

```mermaid
sequenceDiagram
    participant S as Sender
    participant R as Receiver

    Note over S,R: No setup — just send
    S->>R: Datagram 1
    S->>R: Datagram 2
    S->>R: Datagram 3
    Note over R: Receiver may or may not get them
    Note over S: Sender never knows
```

**Cost:** Zero. First packet is sent immediately.
**Trade-off:** No confirmation, no state, no guarantees.

---

## 3. Reliability & Ordering

### TCP — Guaranteed Delivery

```mermaid
sequenceDiagram
    participant C as Client
    participant S as Server

    C->>S: Packet 1
    C->>S: Packet 2
    C->>S: Packet 3
    Note over S: Packet 2 lost in network
    S->>C: ACK 1 received
    S->>C: ACK 3 received (gap detected)
    C->>S: Packet 2 retransmit
    S->>C: ACK 2 received
    Note over S: Delivers to app: 1, 2, 3 in order
```

TCP guarantees:
- Every packet arrives (retransmits if lost)
- Packets delivered to application **in order**
- No duplicates

---

### UDP — Fire and Forget

```mermaid
sequenceDiagram
    participant S as Sender
    participant R as Receiver

    S->>R: Datagram 1 ✅
    S->>R: Datagram 2 ✅
    S--xR: Datagram 3 ❌ lost
    S->>R: Datagram 4 ✅
    S->>R: Datagram 5 ✅
    Note over R: App receives: 1, 2, 4, 5
    Note over R: Datagram 3 is gone forever
    Note over S: Sender has no idea
```

UDP provides:
- No delivery guarantee
- No ordering guarantee
- No duplicate protection
- **Application must handle this if needed**

---

## 4. Data Model — Stream vs Datagram

This is the most important difference for programming.

### TCP — Byte Stream

```mermaid
flowchart LR
    subgraph Sender writes
        W1[write: Hello] 
        W2[write: World]
        W3[write: Foo]
    end

    subgraph Network
        B[byte stream\nHelloWorldFoo]
    end

    subgraph Receiver reads
        R1[read: Hell]
        R2[read: oWor]
        R3[read: ldFoo]
    end

    W1 --> B
    W2 --> B
    W3 --> B
    B --> R1
    B --> R2
    B --> R3
```

TCP has **no message boundaries**. The receiver has no idea where
one `write()` ends and the next begins. You must frame your own messages
(e.g. newline delimiter, length prefix).

```cpp
// Sender writes two messages
socket->write("Hello\n");
socket->write("World\n");

// Receiver might get in ONE read:
// "Hello\nWorld\n"
// OR in two reads:
// "Hell" then "o\nWorld\n"
// You MUST buffer and split on '\n'
```

---

### UDP — Datagrams

```mermaid
flowchart LR
    subgraph Sender sends
        W1[writeDatagram: Hello]
        W2[writeDatagram: World]
        W3[writeDatagram: Foo]
    end

    subgraph Network
        D1[Datagram: Hello]
        D2[Datagram: World]
        D3[Datagram: Foo]
    end

    subgraph Receiver reads
        R1[readDatagram → Hello]
        R2[readDatagram → World]
        R3[readDatagram → Foo]
    end

    W1 --> D1 --> R1
    W2 --> D2 --> R2
    W3 --> D3 --> R3
```

UDP **preserves message boundaries**. One `writeDatagram()` = one
`readDatagram()`. No buffering or framing needed.

```cpp
// Sender
socket->writeDatagram("Hello", addr, port);
socket->writeDatagram("World", addr, port);

// Receiver — each call returns exactly one datagram
data.resize(socket->pendingDatagramSize());
socket->readDatagram(data.data(), data.size(), ...);
// data == "Hello" (complete, no partial reads)
```

---

## 5. Qt API Comparison

### TCP

```cpp
// ── Server side ──────────────────────────────────────────
QTcpServer* server = new QTcpServer(this);
server->listen(QHostAddress::Any, 5000);

connect(server, &QTcpServer::newConnection, this, [=]() {
    QTcpSocket* client = server->nextPendingConnection();
    // client is now a connected socket for this specific client
});

// ── Client side ──────────────────────────────────────────
QTcpSocket* socket = new QTcpSocket(this);
socket->connectToHost("127.0.0.1", 5000);  // must connect first

socket->write("Hello\n");                  // send

connect(socket, &QTcpSocket::readyRead, this, [=]() {
    m_buffer += socket->readAll();          // must buffer!
    while (m_buffer.contains('\n')) { ... } // reassemble messages
});
```

### UDP

```cpp
// ── Receiver side ────────────────────────────────────────
QUdpSocket* socket = new QUdpSocket(this);
socket->bind(QHostAddress::Any, 5000);     // no QUdpServer needed

connect(socket, &QUdpSocket::readyRead, this, [=]() {
    while (socket->hasPendingDatagrams()) {
        QByteArray   data;
        QHostAddress senderAddr;
        quint16      senderPort;
        data.resize(socket->pendingDatagramSize());
        socket->readDatagram(data.data(), data.size(),
                             &senderAddr, &senderPort);
        // data = one complete message, no buffering needed
        // senderAddr/senderPort = who sent it
    }
});

// ── Sender side ──────────────────────────────────────────
QUdpSocket* socket = new QUdpSocket(this);
// no connect needed — just send directly
socket->writeDatagram("Hello", QHostAddress("127.0.0.1"), 5000);
```

---

## 6. Performance Characteristics

```mermaid
flowchart LR
    subgraph TCP overhead per message
        T1[SYN/ACK\nhandshake\n~1.5 RTT] --> T2[Send data]
        T2 --> T3[Wait for ACK]
        T3 --> T4[Retransmit\nif lost]
    end

    subgraph UDP overhead per message
        U1[Send datagram\nthat's it]
    end
```

| Metric | TCP | UDP |
|---|---|---|
| Connection setup | ~1.5 round trips | Zero |
| Per-packet overhead | ACK required | None |
| Retransmission | Automatic | None |
| Head-of-line blocking | Yes — slow packet blocks all | No |
| Typical latency | Higher | Lower |
| Throughput on good network | Similar | Similar |
| Throughput on lossy network | Degrades (retransmits) | Constant (packets dropped) |

**Head-of-line blocking** is a key TCP problem for real-time data:
if packet 5 is lost, packets 6, 7, 8 are held back until 5 is
retransmitted and received — even though 6, 7, 8 are already there.
For audio/video this causes stuttering. UDP just skips 5 and plays on.

---

## 7. When to Use Which

```mermaid
flowchart TD
    Q1{Can you tolerate\nlost packets?}

    Q1 -->|No — every byte matters| TCP
    Q1 -->|Yes — skip and continue| Q2

    Q2{Is latency\ncritical?}
    Q2 -->|Yes — real-time| UDP
    Q2 -->|No — reliability matters more| TCP

    TCP --> T1[File transfer\nHTTP · HTTPS]
    TCP --> T2[Database queries\nSQL · MongoDB]
    TCP --> T3[Email · SSH\nChat · REST APIs]
    TCP --> T4[Call signaling\nSIP over TLS]

    UDP --> U1[Audio/Video streaming\nRTP · WebRTC]
    UDP --> U2[Online gaming\nposition updates]
    UDP --> U3[Radar · Telemetry\nsensor streams]
    UDP --> U4[DNS lookups\nSNMP · NTP]
    UDP --> U5[Voice media\naudio over RTP/UDP]

    style TCP fill:#1F4E79,color:#fff
    style UDP fill:#2E75B6,color:#fff
```

---

## 8. Real-World Industry Examples

| Domain | Protocol | Why |
|---|---|---|
| VoIP call signaling (SIP) | TCP/TLS | SIP must be reliable — call setup can't be lost |
| VoIP call audio (RTP) | UDP | RTP audio — latency over reliability |
| Radar data stream | UDP | High rate, real-time — old frames are useless |
| SCADA sensor telemetry | UDP | High frequency, tolerable loss |
| Industrial PLC commands | TCP | Commands must arrive — machine safety |
| OTA firmware update | TCP | Every byte must arrive correctly |
| Vehicle position broadcast | UDP | Latest position matters, old ones don't |
| DNS query | UDP | Single request/response — fast, no connection |
| HTTPS web traffic | TCP | Every byte of the page must arrive |
| Video conference media | UDP | Latency over perfection — QUIC also UDP-based |

---

## 9. Summary Cheat Sheet

```mermaid
flowchart LR
    subgraph TCP
        direction TB
        A1[✅ Guaranteed delivery]
        A2[✅ Ordered packets]
        A3[✅ Error detection + retransmit]
        A4[✅ Flow control]
        A5[❌ Higher latency]
        A6[❌ Connection required]
        A7[❌ Head-of-line blocking]
        A8[❌ More overhead]
    end

    subgraph UDP
        direction TB
        B1[✅ Low latency]
        B2[✅ No connection overhead]
        B3[✅ Datagram boundaries preserved]
        B4[✅ Can broadcast/multicast]
        B5[❌ No delivery guarantee]
        B6[❌ No ordering guarantee]
        B7[❌ App handles reliability if needed]
        B8[❌ Max datagram size ~65KB]
    end
```

| | TCP | UDP |
|---|---|---|
| Qt server class | `QTcpServer` | None needed |
| Qt socket class | `QTcpSocket` | `QUdpSocket` |
| Send method | `write(data)` | `writeDatagram(data, addr, port)` |
| Receive method | `readAll()` + buffer | `readDatagram()` — no buffer |
| Message framing | Manual (`\n` or length prefix) | Built-in (datagram = message) |
| Know who sent it | From connection | From `readDatagram(&addr, &port)` |
| CMake module | `Qt6::Network` | `Qt6::Network` |
