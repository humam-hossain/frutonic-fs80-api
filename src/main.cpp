#include <iostream>
#include <libusb-1.0/libusb.h>

#ifdef _WIN32
    #define OS_NAME "Windows"
#else
    #define OS_NAME "Linux/Unix"
#endif

bool check_device(libusb_device *device) {
    libusb_device_descriptor descriptor;

    // Get the device descriptor
    if (libusb_get_device_descriptor(device, &descriptor) == 0) {
        const uint16_t FS80_VID = 0x1491; // Frutonic FS80 Vendor ID
        const uint16_t FS80_PID = 0x0020; // Frutonic FS80 Product ID

        if (descriptor.idVendor == FS80_VID && descriptor.idProduct == FS80_PID) {
            std::cout << "[INFO] Fruitonic FS80 fingerprint scanner detected! Vendor ID: " << std::hex << descriptor.idVendor << ", Product ID: " << descriptor.idProduct << std::dec << std::endl;
            return true;
        }
    }

    return false;
}

void list_devices() {
    libusb_context *context = nullptr;
    libusb_device **device_list = nullptr;
    ssize_t device_count;
    ssize_t frutonic_device_count = 0;
    

    // Initialize libusb
    if (libusb_init(&context) != 0) {
        std::cerr << "[ERROR] Failed to initialize libusb" << std::endl;
        return;
    }

    // Get the list of USB devices
    device_count = libusb_get_device_list(context, &device_list);
    if (device_count < 0) {
        std::cerr << "[ERROR] Error getting USB device list" << std::endl;
        libusb_exit(context);
        return;
    }

    std::cout << "[INFO] Operating System: " << OS_NAME << std::endl;
    // std::cout << "[INFO] libusb version: " << libusb_version() << std::endl;
    std::cout << "[INFO] Found " << device_count << " USB devices" << std::endl;
    std::cout << "[INFO] Scanning for USB devices..." << std::endl;

    // Iterate over connected devices
    for (ssize_t i = 0; i < device_count; ++i) {
        if(check_device(device_list[i])) {
            ++frutonic_device_count;
        }
    }

    if (frutonic_device_count == 0) {
        std::cout << "[INFO] No Frutonic FS80 fingerprint scanner found" << std::endl;
    }else{
        std::cout << "[INFO] Found " << frutonic_device_count << " Frutonic FS80 fingerprint scanner(s)" << std::endl;
    }

    // Clean up
    libusb_free_device_list(device_list, 1);
    libusb_exit(context);
}

int main() {
    list_devices();
    return 0;
}
