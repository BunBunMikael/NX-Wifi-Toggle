# NoWiFi (Tesla / Ultrahand overlay)

A single toggle: **Wi-Fi off**.

- ON  -> Wi-Fi is disabled
- OFF -> Wi-Fi is re-enabled
- Bluetooth is never touched in either direction (no `btm`/`btdrv` service is opened, no `set:sys` Bluetooth flag is written).

## What it does

Calls `nifm` `SetWirelessCommunicationEnabled` (`nifm:a`, falling back to `nifm:s`), then reads the state back. If the read-back doesn't match, the toggle snaps back to the real state. Only this one call is used, so there is nothing else to race with.

Nothing in the code checks docked/handheld mode. (Untested on hardware.)

## Get a built .ovl without installing anything

1. Create a GitHub repo and push this folder's contents to it.
2. Open the repo's **Actions** tab. The `build` workflow runs on push (or run it by hand).
3. When it finishes, download the `nowifi-ovl` artifact: that's `nowifi.ovl`.

## Or build locally

Needs devkitPro (`switch-dev`) and libtesla in this folder:

    git clone https://github.com/WerWolv/libtesla libtesla
    export DEVKITPRO=/opt/devkitpro
    make

## Install

Copy `nowifi.ovl` to `sdmc:/switch/.overlays/` and open it from Ultrahand (or Tesla).

## Notes

- If Wi-Fi comes back after waking from sleep, toggle it again.
- To undo: toggle off in the overlay, or re-enable Wi-Fi from System Settings.
