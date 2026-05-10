# Mesh-Gotchi Monorepo

This repository is prepared as a monorepo with 3 focused workspaces:

- `apps/hardware` – PlatformIO firmware scaffold for ESP32-C6 + ST7789 + MPU6050 + NimBLE
- `apps/3d-code` – browser-based 3D concept playground
- `apps/frontend` – browser UI prototype for pet state/display testing

## Quick start

### Hardware firmware (PlatformIO)

```bash
cd apps/hardware
pio run
```

### Frontend prototype

Open `apps/frontend/index.html` in a browser.

### 3D prototype

Open `apps/3d-code/index.html` in a browser.
