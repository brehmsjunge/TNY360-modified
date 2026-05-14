# Firmware

Welcome to the TNY-360 Firmware directory! This is where all the code that powers the TNY-360 robot lives.

The firmware is built on **PlatformIO** using the **ESP-IDF** framework.

## Quick Start

### Prerequisites
1.  **VS Code** installed.
2.  **PlatformIO** extension installed in VS Code.

### Installation
1.  **Clone** this repository:
    ```bash
    git clone --recursive https://github.com/TNY-Robotics/TNY-360.git
    ```
2.  **Open** the `Firmware/` folder in VS Code.
3.  **Connect** your ESP32-S3 N16R8 via USB.
4.  **Upload:** Click the generic "Arrow" icon (➡️) in the PlatformIO bottom bar to Build & Flash.

> **Note:** If you are missing drivers, run `git submodule update --init --recursive` to pull the TNY-Robotics custom components.

---