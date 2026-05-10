# Claude Workspace Guide for Mesh-Gotchi

Use this repository as a monorepo:

1. `apps/hardware` for PlatformIO/C++ embedded implementation.
2. `apps/frontend` for display/gameplay UI prototyping.
3. `apps/3d-code` for 3D experiments and animation concepts.

## Expected implementation flow

- Bring up hardware peripherals first (TFT + MPU6050).
- Keep render logic sprite-based for low-flicker drawing.
- Use a state machine (`SLEEP`, `IDLE`, `ALERT`, `MENU`) for runtime behavior.
- Add BLE client handling for Meshtastic notifications.

## Validation commands

- Firmware build: `cd apps/hardware && pio run`
- Frontend manual check: open `apps/frontend/index.html`
- 3D manual check: open `apps/3d-code/index.html`
