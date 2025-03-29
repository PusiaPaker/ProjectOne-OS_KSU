# Operating Systems KSU - Project One - Antoni Chelstowski

This project implements the classic Producer-Consumer problem using C on Linux with POSIX threads, semaphores, and shared memory.

---

## Compilation
Use the provided `Makefile` to compile the programs:
```bash
make
```
This will build both the `producer` and `consumer` executables.

---

##  Running the Programs
Use the `Makefile` to run both programs together in the background:
```bash
make run
```
This starts both `producer` and `consumer` simultaneously.

To run them manually in separate terminals:
```bash
./producer &
./consumer &
```
You should see console output from both showing the number of items on the shared "table".

---

## Stopping the Programs
Use the `Makefile` target to stop the programs:
```bash
make stop
```
This will find and kill the producer and consumer processes.

Alternatively, use `Ctrl+C` if you're running them in the foreground.

To clean up shared memory and semaphores:
```bash
make purge
```
This removes shared memory and semaphore files from `/dev/shm`.

---