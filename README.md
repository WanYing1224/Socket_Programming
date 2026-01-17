# Socket Programming

This repository contains the implementation of Client-Server socket programming in C for USC EE533 course. The project demonstrates network communication using both TCP (Stream) and UDP (Datagram) protocols, as well as local Inter-Process Communication (IPC) using Unix Domain Sockets.

## Project Overview

The main objective of this lab is to understand the fundamentals of network sockets, process management, and signal handling in a Linux environment.

**Key Features:**
* **Concurrent TCP Server:** Uses `fork()` to handle multiple clients simultaneously without blocking.
* **Zombie Process Cleanup:** Implements a `SIGCHLD` signal handler to automatically reap dead child processes and prevent resource leaks.
* **UDP Implementation:** Demonstrates connectionless communication using datagrams.
* **Unix Domain Sockets:** Demonstrates local communication between processes on the same machine using file system paths.

## Files Included

* **TCP Implementation:**
    * `server_tcp.c` - Concurrent TCP server (handles multiple clients).
    * `client_tcp.c` - TCP client that connects to the server.
* **UDP Implementation:**
    * `server_udp.c` - Connectionless UDP server.
    * `client_udp.c` - UDP client.
* **Unix Domain Implementation:**
    * `server_unix.c` - Local IPC server using `AF_UNIX`.
    * `client_unix.c` - Local IPC client using `AF_UNIX`.

## Prerequisites

* Linux Environment (Ubuntu recommended)
* GCC Compiler (`sudo apt install gcc`)

## Compilation Instructions

You can compile all files using the standard GCC command:

```bash
# TCP
gcc server_tcp.c -o server_tcp
gcc client_tcp.c -o client_tcp

# UDP
gcc server_udp.c -o server_udp
gcc client_udp.c -o client_udp

# Unix Domain
gcc server_unix.c -o server_unix
gcc client_unix.c -o client_unix
