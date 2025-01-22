# Frutonic FS80 API

## Project Plan
* C++ for the core logic and create bindings for Python or a REST API for ease of integration.

## Project Structure:
```bash
├── core/                 # C++ Core Logic
│   ├── fs80_driver/      # Platform-specific FS80 interaction
│   ├── lib/              # Cross-platform core logic (e.g., algorithms)
│   └── include/          # Headers
├── bindings/             # Python bindings (Pybind11 or Cython)
├── api/                  # REST API (CPP HTTP library or Python FastAPI)
├── build/                # Build outputs
└── CMakeLists.txt        # Cross-platform build configuration
```
## Requirements

### Linux
```bash
sudo apt-get install libusb-dev gtk2.0-dev
```

### Windows
- libusb: https://sourceforge.net/projects/libusb-win32/
