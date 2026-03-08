# Systat - C++ Version
Systat is a system monitoring tool written in C++, designed as a modern replacement for my original C Systat project.

This project collects **system metrics (CPU, memory usage, uptime) and runs as a daemon compatible with systemd**, using modern C++ practices like RAII.

<br/>

## :book: Description
Systat is a system monitoring tool written in C++, designed as a modern replacement for my original C implementation.

Collects CPU usage, memory usage, and system uptime.

Runs as a daemon using a socket server to allow local clients to query metrics.

Provides a graphical interface built with FLTK (Fast ight Toolkit) library.

Fully RAII-compliant for safer resource management.

Compatible with systemd services for automatic startup.

<br/>

## ⚙️ Requirements
- G++
- Make
- A Unix-like system (Linux recommended)
- FLTK library (Fast Light Toolkit)

<br/>

## :hammer_and_wrench: Build 
To compile all binaries
```bash
make all
```

This will produce :

- sys-stat → the daemon
- sys-stat-ui -> graphical interface
- client-test → test client to ping the daemon
- print-stat → command-line utility to print metrics once

<br/>

## :rocket: Usage
### 1️⃣ Start the daemon
```bash
./sys-stat
```

- The daemon will fork and run in the background.
- It listens for local client connections via a socket.
- Logs are written to /tmp/sys-stat.log (or /run/user/<uid>/sys-stat.log if configured).


### 2️⃣ Start the GUI
```bash
./sys-stat-ui
```

- The graphical interface connects to the daemon through the client socket.
- It requests system metrics from the daemon and displays them in real time.
- The GUI does not collect metrics itself; it only queries the daemon.
- The daemon must be running before starting the GUI.

<br/>

## :test_tube: Test
### 1️⃣ Test the client / daemon communication

```bash
./client-test
```

- Simulates a client similar to the GUI.
- Connects to the daemon every 2 seconds through the socket.
- Requests system metrics and prints the response in the console.
- Useful to verify that the daemon is running and responding correctly.

### 2️⃣ Print metrics directly (no daemon required)

```bash
./print-stat
```

- Fetches system metrics once and prints them to the console.
- Runs independently without connecting to the daemon.
- Useful for debugging the metric collection logic.

<br/>

## :satellite: Run daemon service
### 1️⃣ Copy the daemon binary

```bash
cp sys-stat /usr/bin/sys-stat
chmod +x /usr/bin/sys-stat
```

### 2️⃣ Copy the systemd unit file

```bash
cp sys-stat.service /usr/lib/systemd/system/sys-stat.service
```

### 3️⃣ Start the service

```bash
systemctl start sys-stat
```
You need to adapt the user configuration in "sys-stat.service" before (option "User" in the service section).

### 4️⃣ Check satus log

```bash
systemctl status sys-stat
journalctl -u sys-stat -f
cat /tmp/sys-stat.log
```

<br/>

## :desktop_computer: Run GUI service
### 1️⃣ Copy the gui binary

```bash
cp sys-stat-ui /usr/bin/sys-stat-ui
chmod +x /usr/bin/sys-stat-ui
```

### 2️⃣ Copy the systemd unit file to the **user session**

```bash
cp sys-stat-ui.service ~/.config/systemd/user/sys-stat-ui.service
```

### 3️⃣ Start the GUI service

```bash
systemctl --user start sys-stat-ui
```
The service must be run within the user’s graphical session to inherit all necessary environment variables (e.g., WAYLAND_DISPLAY, DBUS_SESSION_BUS_ADDRESS ...).

### 4️⃣ Check satus log

```bash
systemctl status sys-stat-ui
journalctl -u sys-stat-ui
```

<br/>

## :building_construction: TODO
-   [x] Create system service
-   [x] Create printstat
-   [x] Create socket server
-   [x] Daemonize app
-   [x] Move structure for RAII
-   [x] Create client test
-   [x] Create systemd service
-   [x] Create ui
-   [x] Create systemd service for gui

<br/>

## :bulb: Notes
- PID files must be writable by the current user running the daemon. `chmod +x ...`
- The daemon will not start if an instance is already running (checked via the PID file).  
  If it still fails to start after stopping the process, check whether the **socket file** needs to be removed before launching a new instance: `/tmp/sys-stat.sock`