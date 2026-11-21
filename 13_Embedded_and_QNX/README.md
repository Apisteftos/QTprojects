# 13_Embedded_and_QNX

Running Qt applications on embedded devices: cross-compiling, embedded Linux, and the QNX real-time operating system.

**Status: planned.** This README describes the goals, the tools and the order of the topics. The topic folders are added one by one as they are written and tested.

**Prerequisites:** `01_Fundamentals`, `11_QML` (at least topics 01 to 11) and `12_BuildTestDeploy`. You should be comfortable with CMake, the command line and `ssh`/`scp` on Linux.

---

## 1. What Is Embedded Development with Qt?

An embedded device (a control panel, a gateway, a display in a machine or a vehicle) is usually too small to run a compiler and an IDE. So you do not build on the device. You build on your PC and copy the result to the device.

| Term | Meaning |
|------|---------|
| **Host** | The computer you develop on (your Linux PC) |
| **Target** | The device the program runs on |
| **Cross-compiling** | Building on the host a program that runs on a different processor or operating system |
| **Toolchain** | The compiler, linker and related tools that produce code for the target |
| **Sysroot** | A copy of the target's headers and libraries, which the toolchain links against |
| **Toolchain file** | A CMake file (`CMAKE_TOOLCHAIN_FILE`) that tells CMake which compiler and sysroot to use |
| **Deploy** | Copying the program (and the Qt libraries it needs) to the target |
| **Remote debugging** | Running the program on the target while you debug it from the host |

Qt itself must also exist **for the target**. The tools that run during the build (`moc`, `rcc`, `qmlcachegen`, `qsb`) run on the host, so a normal Qt installation for your PC is needed as well. The Qt documentation for QNX describes this host and target setup.

---

## 2. What Is QNX?

QNX Neutrino is a **real-time operating system (RTOS)** based on a microkernel, and it follows the POSIX standard. It is used in places where timing and reliability matter, such as automotive systems, robotic systems and medical devices.

You develop for it with the **QNX Software Development Platform (SDP)**. It contains the operating system, the compiler toolchain and tools for debugging and profiling. QNX SDP 8.0 is the current version, and the older SDP 7.1 is still supported.

Because it is POSIX based, a lot of Linux knowledge carries over: files and processes, threads, sockets, `ssh` and `scp`. The main differences are the toolchain, the system services and the graphics subsystem.

---

## 3. Qt on QNX

Qt runs on top of QNX Neutrino. To use it you follow these steps:

1. Install a QNX SDP on your host. This needs a license.
2. Have a **host build of Qt** (for the build tools).
3. Create a **toolchain file** for QNX. The Qt documentation has an example for the processor types `armv7le`, `aarch64le` and `x86_64`.
4. Configure and build Qt for QNX with that toolchain file, and install it into a staging folder on the host.
5. Build your application with the `qt-cmake` script from that staging folder.
6. Deploy the result to the device and run it.

In Qt Creator you add a **QNX device** under Preferences → Devices → Devices, select it in your kit and press **Run**.

### Licensing

| Part | Terms |
|------|-------|
| QNX SDP 8.0 | A free, personal, **non-commercial** license is available with a myQNX account. For a commercial project there is a 30-day evaluation, and a commercial license after that |
| Qt for QNX | The Qt documentation states that Qt for QNX is available under the **Qt for Device Creation Professional** license |

Read the current terms before you plan a product. Nothing in this repository replaces them.

> **Rule for this repository:** it contains only our own source files and CMake files. It never contains QNX or Qt-for-QNX headers, libraries, images or other files from the SDP.

---

## 4. Embedded Linux

Before QNX, the first target is an embedded Linux device. Qt offers several ways to show a user interface there:

| Platform plugin | What it does |
|-----------------|--------------|
| **EGLFS** | Runs Qt applications on EGL and OpenGL ES without a display server. For boards with a GPU |
| **LinuxFB** | Draws directly into the framebuffer. Software rendering only |
| **Wayland** | Needs a Wayland compositor and gives a full multi-window system |

The same cross-compiling ideas from section 1 apply, so the embedded Linux topics come first.

---

## 5. Topics

These are the planned topics. The names may change while the lessons are written.

| # | Folder | What it teaches |
|---|--------|-----------------|
| 01 | `01_cross_compiling_basics` | Host and target, toolchain file, sysroot, a small C++23 program built on the host and run on a target |
| 02 | `02_qt_for_embedded_linux` | A Qt Quick application for an embedded Linux target: EGLFS or Wayland, deployment with `scp` |
| 03 | `03_qnx_sdp_setup` | myQNX account, non-commercial license, QNX Software Center, SDP environment, a QNX target in a virtual machine |
| 04 | `04_qnx_hello_cpp` | A plain C++ program on QNX, built with the SDP toolchain and CMake |
| 05 | `05_qnx_hello_qt` | A minimal Qt program on QNX |
| 06 | `06_qnx_qml_app` | A QML application on a QNX target |
| 07 | `07_remote_run_and_debug` | Qt Creator with a QNX device and kit, running and debugging on the target |
| 08 | `08_resources_and_performance` | Memory, start-up time and profiling on small devices |

Topics 05 to 08 need a Qt build for QNX, so they depend on having a suitable Qt license. If it is not available, they stay as plans.

---

## 6. Tools You Need

| Item | Used for |
|------|----------|
| Linux PC (the host) | Building, as in the other chapters (Qt 6.8 or newer, CMake 3.21 or newer, C++23 compiler) |
| Qt Creator | Kits, devices, deployment and debugging |
| `ssh` and `scp` | Copying files to the target and running commands there |
| QNX SDP 8.0 | Toolchain and tools for QNX (topics 03 to 08) |
| Qt for QNX | The Qt libraries for the target (topics 05 to 08) |
| A target | A virtual machine (QEMU) or a development board. QNX offers target images for QEMU and Raspberry Pi in its free QNX Everywhere program |

---

## 7. Known Notes from the Qt Documentation

These points come from the Qt documentation for building Qt for QNX. Check the current page for your Qt version, because they can change.

| Note | What to do |
|------|------------|
| Experimental Wayland packages from the QNX Software Center can cause a header conflict when Qt is configured | Do not add them to the SDP 8.0 installation for building Qt |
| CMake does not pass the variables `STRIP`, `AR`, `RANLIB` and `NM` correctly on QNX | If you use them, set them in the toolchain file |
| Debug output may not appear in Qt Creator's Application Output | Qt Creator needs an SSH connection to the device. This works only if QNX Momentics is not running and the SSH key is a 4096-bit key |
| The board support package of a device may lack tools that Qt Creator needs for running and debugging | Check the Qt Creator documentation for the required command-line tools |

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Qt for QNX (overview and license) | <https://doc.qt.io/qt-6/qnx.html> |
| Building Qt for QNX | <https://doc.qt.io/qt-6/building-qt-for-qnx.html> |
| Qt for Embedded Linux | <https://doc.qt.io/qt-6/embedded-linux.html> |
| Run on QNX devices (Qt Creator) | <https://doc.qt.io/qtcreator/creator-how-to-run-on-qnx.html> |
| Free non-commercial QNX SDP 8.0 license | <https://qnx.com/getqnx> |
| QNX Everywhere introduction | <https://www.qnx.com/developers/docs/qnxeverywhere/introduction.html> |
