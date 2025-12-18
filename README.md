# Virtual IoT Sensor Node on RISC-V (Zephyr RTOS)

##  Project Overview
This project simulates a **Real-Time IoT Sensor Node** running on the **Zephyr Real-Time Operating System (RTOS)**. It targets a virtual **RISC-V 32-bit** processor (via QEMU) to demonstrate embedded systems programming without physical hardware.

The system mimics a "Smart Weather Station" where a **Sensor Thread** generates data and a **Cloud Uplink Thread** processes it, communicating safely via **Inter-Process Communication (IPC)** mechanisms.

##  Key Features
* **Multi-Threading:** Implements concurrent Producer (Sensor) and Consumer (Cloud) threads.
* **IPC (Message Queues):** Uses `k_msgq` for thread-safe data transfer, preventing race conditions.
* **Real-Time Latency:** Measures system latency (sub-millisecond) to validate real-time performance.
* **Robustness:** Implements a "Ring Buffer" strategy to purge old data if the network is slow, prioritizing fresh data.
* **Custom Data Structures:** Transmits complex packets (`struct`) containing ID, Temperature, and Timestamp.

##  Technical Stack
* **OS:** Zephyr RTOS (v3.7+)
* **Architecture:** RISC-V 32-bit (`qemu_riscv32`)
* **Language:** C
* **Build System:** West (CMake + Ninja)
* **Simulation:** QEMU (Quick Emulator)

##  Project Structure
* `src/main.c`: Core application logic (Thread definitions, IPC implementation).
* `prj.conf`: Kernel configuration (Enables Printk, Assertions, and Heap).
* `CMakeLists.txt`: Build system configuration.
* `run_ipc_demo.sh`: Automation script to setup environment and run simulation.

##  How to Run
Prerequisites: Zephyr SDK and West installed on Linux/WSL.

1.  **Clone the Repository:**
    ```bash
    git clone [https://github.com/taimoorasad01/Zephyr-RISCV-IoT-Project.git](https://github.com/taimoorasad01/Zephyr-RISCV-IoT-Project.git)
    cd Zephyr-RISCV-IoT-Project
    ```

2.  **Build for RISC-V (QEMU):**
    ```bash
    west build -b qemu_riscv32
    ```

3.  **Run Simulation:**
    ```bash
    west build -t run
    ```

4.  **Exit Simulation:**
    Press `Ctrl + A`, then `X`.

##  Sample Output
```text
*** Booting Zephyr OS build ... ***
[Cloud] RX: ID=0    | Temp=20C | Latency=0ms
[Cloud] RX: ID=1    | Temp=21C | Latency=0ms
[Sensor] ALERT: Queue full! Purged old packet ID 2
[Cloud] RX: ID=3    | Temp=23C | Latency=0ms

##  OS Concepts Demonstrated
1.  **Cooperative Scheduling:** Threads yield CPU using `k_msleep()` and blocking API calls.
2.  **Synchronization:** Kernel-level locking in Message Queues ensures data integrity.
3.  **Resource Management:** Efficient stack usage and memory alignment for embedded structs.

---
