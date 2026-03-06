# Systat - C++ Version
Systat is a system monitoring tool written in C++, designed as a modern replacement of my original C Systat project.

This project collects system metrics (CPU, memory usage, uptime) and runs as a daemon compatible with systemd, using modern C++ practices like RAII.


## :book: Description
Systat is a system monitoring tool written in C++, designed as a modern replacement of my original C implementation.

Collects CPU usage, memory usage, and system uptime.

Runs as a daemon using a socket server to allow local clients to query metrics.

Fully RAII-compliant for safer resource management.

Compatible with systemd services for automatic startup.


### ⚙️ Requirements
- G++
- Make
- A Unix-like system (Linux recommended)

### :hammer: Build 
To compile all binaries :
```bash
make all
```

This will produce:

- sys-stat → the daemon
- client-test → test client to ping the daemon
- print-stat → command-line utility to print metrics once


### :rocket: Usage
1️⃣ Start the daemon
```bash
./sys-stat
```

- The daemon will fork and run in the background.
- It listens for local client connections via a socket.
- Logs are written to /tmp/sys-stat.log (or /run/user/<uid>/sys-stat.log if configured).


2️⃣ Run a client test 

```bash
./client-test
```

- The client connects to the daemon every 2 seconds.
- The daemon responds with current system metrics.
- As the project is still in the development stage, the result is displayed in the console.


3️⃣ Run test

```bash
./printstat
```
 
- Fetches system metrics once and prints to the console.
- Useful for debugging or quick checks without running the daemon.


### :hammer_and_wrench: Run service
1️⃣ Copy the daemon binary

```bash
cp sys-stat /usr/bin/sys-stat
```

2️⃣ Copy the systemd unit file

```bash
cp sys-stat.service /usr/lib/systemd/system/sys-stat.service
```

3️⃣ Start the service

```bash
systemctl start sys-stat
```
You need to adapt the user name before.

4️⃣ Check satus log

```bash
systemctl status sys-stat
journalctl -u sys-stat -f
cat /tmp/sys-stat.log
```

### :building_construction: TODO
-   [x] Create system service
-   [x] Create printstat
-   [x] Create socket server
-   [x] Daemonize app
-   [x] Move structure for RAII
-   [x] Create client test
-   [x] Create systemd service
-   [ ] Create ui


### :bulb: Notes
- Logs and PID files must be writable by the current user running the daemon. In my case : (User=chouchou for systemd).
- The daemon will not start if an instance is already running (checked via the PID file).  
  If it still fails to start after stopping the process, check whether the **socket file** needs to be removed before launching a new instance: `/tmp/sysstat.sock`.