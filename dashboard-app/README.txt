TNY-360 Dashboard Desktop App
==============================

A native Windows desktop application for controlling the TNY-360 quadruped robot
without needing a browser or switching network connections manually.


HOW IT WORKS
-------------
- The app bundles the existing WebPortal (Nuxt/Vue.js) and serves it locally via
  a built-in HTTP server on http://127.0.0.1:3333
- It connects to the TNY-360 via WebSocket on ws://192.168.4.1:5621 (default AP IP)
- The robot IP is set automatically via localStorage on first launch


REQUIREMENTS
-------------
- Windows 10/11
- Node.js and npm (for building)
- TNY-360 robot powered on and in Access Point (AP) WiFi mode
- PC connected to the TNY-360 WiFi network


BUILD FROM SOURCE
------------------
1. Build the WebPortal:
   cd Firmware/extras/WebPortal
   npm install
   npm run generate

2. Build the Electron app:
   cd dashboard-app
   npm install
   npx electron-builder

   Output: dashboard-app/dist/TNY-360 Dashboard Setup 1.0.0.exe

3. Run the installer and launch "TNY-360 Dashboard" from the desktop or start menu.


USAGE
------
1. Power on the TNY-360
2. Connect your PC to the TNY-360 WiFi network
3. Open the TNY-360 Dashboard app
4. The app will automatically connect to the robot at 192.168.4.1


PROJECT STRUCTURE
------------------
dashboard-app/
  main.js        - Electron main process, HTTP server, window setup
  preload.js     - Electron preload script (bridge for future USB Serial)
  package.json   - Node.js project config + electron-builder config

The WebPortal files are bundled from:
  Firmware/extras/WebPortal/.output/public/
(generated via "npm run generate" in the WebPortal directory)


TECHNICAL NOTES
----------------
- The app uses a local HTTP server instead of file:// protocol because Nuxt
  generates absolute asset paths (/_nuxt/...) that don't work with Electron's
  file:// loader.
- On first load, the app injects localStorage.setItem('fallback_ip', '192.168.4.1')
  so the WebPortal SDK connects to the correct robot IP instead of 127.0.0.1.
- USB Serial support is planned for a future version (SerialTransport.hpp is
  already implemented on the firmware side).


PLANNED FEATURES
-----------------
- USB Serial connection (no WiFi needed)
- WiFi <-> USB toggle in the UI
- Custom IP input dialog


CREDITS
--------
- TNY-360 robot & WebPortal: Paul Loisil (FurWaz) / TNY Robotics
  https://github.com/TNY-Robotics/TNY-360
- Dashboard Desktop App: Community contribution
