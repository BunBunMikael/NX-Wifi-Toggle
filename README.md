# NX-Wifi-Toggle (A Tesla / Ultrahand overlay) A single toggle for all your wi-fi needs

- ON  -> Wi-Fi is disabled
- OFF -> Wi-Fi is re-enabled
- Bluetooth is never touched in either direction (no `btm`/`btdrv` service is opened, no `set:sys` Bluetooth flag is written).

## What this does

The overlay calls `nifm` `SetWirelessCommunicationEnabled` (`nifm:a`) to switch wifi states. Only this call is used.
Nothing in the code checks docked/handheld mode.

## You can get an already built .ovl from the release tab

## What you need to build it yourself

DevkitPro (`switch-dev`) and libtesla

## Installation

Copy `NX-Wifi-Toggle.ovl` to `sdmc:/switch/.overlays/` and open it from Ultrahand (or Tesla).

## Notes

- To undo: toggle off in the overlay, or re-enable Wi-Fi from System Settings.
