// NoWiFi - Tesla / Ultrahand overlay that toggles Wi-Fi off and on.
//
// Single mechanism: nifm SetWirelessCommunicationEnabled (nifm:a, falling back to nifm:s).
// This is the same call the system's own airplane mode goes through (switchbrew lists
// nifm cmd 16 under its old name, SetFlightModeOnOff).
//
// Bluetooth is deliberately never touched: no btm/btdrv service is opened and no
// set:sys Bluetooth flag is written. Nothing here checks dock state.

#define TESLA_INIT_IMPL
#include <tesla.hpp>

static bool g_nifmOk = false;

static bool wifiIsOff() {
    bool enabled = true;
    if (g_nifmOk && R_SUCCEEDED(nifmIsWirelessCommunicationEnabled(&enabled)))
        return !enabled;
    return false;
}

// Returns true if the read-back matches what was requested.
static bool setWifiOff(bool off) {
    if (!g_nifmOk) return false;
    nifmSetWirelessCommunicationEnabled(!off);
    return wifiIsOff() == off;
}

class MainGui : public tsl::Gui {
public:
    MainGui() {}

    virtual tsl::elm::Element *createUI() override {
        auto *frame = new tsl::elm::OverlayFrame("NoWiFi", "Wi-Fi only - Bluetooth untouched");
        auto *list  = new tsl::elm::List();

        list->addItem(new tsl::elm::CategoryHeader("Wireless"));

        auto *toggle = new tsl::elm::ToggleListItem("Wi-Fi off", wifiIsOff());
        toggle->setStateChangedListener([toggle](bool off) {
            if (!setWifiOff(off)) {
                // The change didn't take: snap the toggle back to the real state.
                toggle->setState(wifiIsOff());
            }
        });
        list->addItem(toggle);

        if (!g_nifmOk)
            list->addItem(new tsl::elm::CategoryHeader("Could not reach nifm"));

        frame->setContent(list);
        return frame;
    }
};

class NoWifiOverlay : public tsl::Overlay {
public:
    virtual void initServices() override {
        tsl::hlp::doWithSmSession([] {
            g_nifmOk = R_SUCCEEDED(nifmInitialize(NifmServiceType_Admin)) ||
                       R_SUCCEEDED(nifmInitialize(NifmServiceType_System));
        });
    }

    virtual void exitServices() override {
        if (g_nifmOk) nifmExit();
    }

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<MainGui>();
    }
};

int main(int argc, char **argv) {
    return tsl::loop<NoWifiOverlay>(argc, argv);
}
