# 🚗 AUTOSAR-Inspired Multi-ECU Communication System using CAN & UDS

## 📌 Overview

This project demonstrates an **AUTOSAR-inspired automotive software architecture** with multi-ECU communication using **CAN (Controller Area Network)** and **UDS (Unified Diagnostic Services)**.

The system simulates real-world ECU interaction where one ECU detects a driver-related fault and communicates it over CAN, while another ECU processes the message and responds using diagnostic services.

---

## 🧠 Key Features

* ✅ AUTOSAR-style Software Component (SWC)
* ✅ Runtime Environment (RTE) simulation
* ✅ Multi-ECU communication over CAN
* ✅ SocketCAN implementation (Linux)
* ✅ UDS-based diagnostic response (0x22 / 0x62)
* ✅ Cross-platform development (macOS + Linux VM)

---

## 🏗️ System Architecture

```
          +----------------------+
          |      ECU 1           |
          |----------------------|
          |  SWC (Driver Alert)  |
          |  RTE                |
          +----------+-----------+
                     |
                     | CAN (0x123)
                     ↓
          +----------------------+
          |      ECU 2           |
          |----------------------|
          |  CAN Receiver        |
          |  UDS Handler         |
          +----------+-----------+
                     |
                     | CAN (0x456)
                     ↓
          +----------------------+
          |      ECU 1           |
          |----------------------|
          |  UDS Response Handler|
          +----------------------+
```

---

## ⚙️ Technologies Used

* **Programming Language:** C
* **Automotive Concepts:** AUTOSAR (SWC, RTE), ECU, CAN, UDS
* **Communication:** SocketCAN (Linux)
* **Tools:** GCC, Linux (Ubuntu VM), macOS

---

## 📂 Project Structure

```
autosar-ecu-project/
│
├── ecu1/                  # Sender ECU (SWC + CAN Tx + UDS Rx)
│   └── main_ecu1.c
│
├── ecu2/                  # Receiver ECU (CAN Rx + UDS Tx)
│   └── main_ecu2.c
│
├── src/                   # Core modules
│   ├── swc_driver_alert.c
│   ├── rte.c
│   ├── uds.c
│   ├── can.c
│
├── include/               # Header files
│
└── README.md
```

---

## 🔁 System Workflow

1. ECU1 simulates sensor input (speed + drowsiness)
2. SWC processes data and detects a fault condition
3. ECU1 sends warning via CAN (ID: `0x123`)
4. ECU2 receives message and processes fault
5. ECU2 sends UDS diagnostic response (ID: `0x456`)
6. ECU1 receives and displays the diagnostic result

---

## 🧪 How to Run

### 🐧 On Linux (Required for CAN)

#### 1. Install dependencies

```
sudo apt update
sudo apt install build-essential can-utils
```

#### 2. Enable virtual CAN

```
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

#### 3. Compile

```
gcc ecu1/main_ecu1.c src/swc_driver_alert.c -Iinclude -o ecu1_app
gcc ecu2/main_ecu2.c -o ecu2_app
```

#### 4. Run

Terminal 1:

```
./ecu2_app
```

Terminal 2:

```
./ecu1_app
```

---

## ✅ Expected Output

### ECU2:

```
[ECU2] Waiting for message...
[ECU2] Received CAN ID: 0x123
[ECU2] Warning detected! Fault: 1
[ECU2] Sent UDS Response (Fault: 1)
```

### ECU1:

```
[ECU1] Sent Warning (Fault: 1)
[ECU1] Waiting for UDS response...
[ECU1] Received UDS Response: Fault = 1
```

---

## 💻 macOS Development

* Core logic (SWC, RTE, UDS) can be developed on macOS
* CAN communication is simulated using print statements
* Full CAN functionality is tested in Linux using SocketCAN

---

## 🚀 Learning Outcomes

* Understanding of AUTOSAR architecture (SWC, RTE)
* Hands-on experience with CAN communication
* Implementation of UDS diagnostic services
* Multi-ECU system design and interaction
* Cross-platform embedded system development

---

## 📈 Future Improvements

* Add DTC (Diagnostic Trouble Code) memory
* Implement more UDS services (0x10, 0x19)
* Integrate cloud telemetry (AWS)
* Add real-time dashboard visualization

---

## 👤 Author

**Deep Vaghani**
M.Sc. Automotive Software Engineering
Technische Universität Chemnitz

---

## ⭐ Acknowledgment

This project is inspired by real-world automotive ECU communication systems and AUTOSAR architecture principles.

---