# Systat - C++ Version
C++ reimplementation of my C project (Systat).


## Description
This project is a C++ rewrite of my original C implementation of a system monitoring tool. The goal was to practice this language.


### ⚙️ Requirements
- G++
- Make
- A Unix-like system (Linux recommended)

### :hammer: Build 
To compile all binaries :
```bash
make all
```

### :rocket: Usage
For now, we can only run test :

```bash
./printstat
```
 
The printstat function will call system service and print result in your terminal only once.

### :building_construction: TODO
-   [x] Create system service
-   [x] Create printstat
-   [x] Create socket server
-   [x] Daemonize app
-   [ ] Create client test