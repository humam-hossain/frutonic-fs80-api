#include <vector>
#include <cstdio>
#include <libusb-1.0/libusb.h>

const uint16_t FS80_VID = 0x1491;
const uint16_t FS80_PID = 0x0020;

struct FS80Device {
    libusb_device* device;
    uint8_t bus_number;
    uint8_t device_address;
};

const char* transfer_type_str(uint8_t attributes) {
    switch(attributes & LIBUSB_TRANSFER_TYPE_MASK) {
        case LIBUSB_TRANSFER_TYPE_CONTROL: return "Control";
        case LIBUSB_TRANSFER_TYPE_ISOCHRONOUS: return "Isochronous";
        case LIBUSB_TRANSFER_TYPE_BULK: return "Bulk";
        case LIBUSB_TRANSFER_TYPE_INTERRUPT: return "Interrupt";
        default: return "Unknown";
    }
}

const char* class_code_str(uint8_t class_code) {
    switch(class_code) {
        case LIBUSB_CLASS_PER_INTERFACE: return "Per Interface";
        case LIBUSB_CLASS_AUDIO: return "Audio";
        case LIBUSB_CLASS_COMM: return "Communications";
        case LIBUSB_CLASS_HID: return "HID";
        case LIBUSB_CLASS_PHYSICAL: return "Physical";
        case LIBUSB_CLASS_PRINTER: return "Printer";
        case LIBUSB_CLASS_PTP: return "PTP";
        case LIBUSB_CLASS_MASS_STORAGE: return "Mass Storage";
        case LIBUSB_CLASS_HUB: return "Hub";
        case LIBUSB_CLASS_DATA: return "Data";
        case LIBUSB_CLASS_VENDOR_SPEC: return "Vendor Specific";
        default: return "Unknown";
    }
}

void print_endpoint(const libusb_endpoint_descriptor *endpoint) {
    printf("      ENDPOINT 0x%02x: %s %s ===============================\n",
           endpoint->bEndpointAddress,
           (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) ? "IN" : "OUT",
           transfer_type_str(endpoint->bmAttributes));
    
    printf("       bLength          : 0x%02x (%d bytes)\n", 
           endpoint->bLength, endpoint->bLength);
    printf("       bDescriptorType  : 0x%02x\n", endpoint->bDescriptorType);
    printf("       bEndpointAddress : 0x%02x\n", endpoint->bEndpointAddress);
    printf("       bmAttributes     : 0x%02x\n", endpoint->bmAttributes);
    printf("       wMaxPacketSize   : 0x%04x (%d bytes)\n", 
           endpoint->wMaxPacketSize, endpoint->wMaxPacketSize);
    printf("       bInterval        : 0x%02x\n", endpoint->bInterval);
}

void print_interface(const libusb_interface_descriptor *interface) {
    printf("    INTERFACE %d: %s ===========================\n",
           interface->bInterfaceNumber,
           class_code_str(interface->bInterfaceClass));
    
    printf("     bLength            : 0x%02x (%d bytes)\n", 
           interface->bLength, interface->bLength);
    printf("     bDescriptorType    : 0x%02x\n", interface->bDescriptorType);
    printf("     bInterfaceNumber   : 0x%02x\n", interface->bInterfaceNumber);
    printf("     bAlternateSetting  : 0x%02x\n", interface->bAlternateSetting);
    printf("     bNumEndpoints      : 0x%02x\n", interface->bNumEndpoints);
    printf("     bInterfaceClass    : 0x%02x\n", interface->bInterfaceClass);
    printf("     bInterfaceSubClass : 0x%02x\n", interface->bInterfaceSubClass);
    printf("     bInterfaceProtocol : 0x%02x\n", interface->bInterfaceProtocol);
    printf("     iInterface         : 0x%02x\n", interface->iInterface);

    for(int ep = 0; ep < interface->bNumEndpoints; ep++) {
        print_endpoint(&interface->endpoint[ep]);
    }
}

void print_configuration(const libusb_config_descriptor *config) {
    printf("  CONFIGURATION %d: %d mA ==================================\n",
           config->bConfigurationValue,
           config->MaxPower * 2);
    
    printf("   bLength              : 0x%02x (%d bytes)\n", 
           config->bLength, config->bLength);
    printf("   bDescriptorType      : 0x%02x\n", config->bDescriptorType);
    printf("   wTotalLength         : 0x%04x (%d bytes)\n", 
           config->wTotalLength, config->wTotalLength);
    printf("   bNumInterfaces       : 0x%02x\n", config->bNumInterfaces);
    printf("   bConfigurationValue  : 0x%02x\n", config->bConfigurationValue);
    printf("   iConfiguration       : 0x%02x\n", config->iConfiguration);
    printf("   bmAttributes         : 0x%02x\n", config->bmAttributes);
    printf("   bMaxPower            : 0x%02x (%d mA)\n", 
           config->MaxPower, config->MaxPower * 2);

    for(int i = 0; i < config->bNumInterfaces; i++) {
        const libusb_interface *interface = &config->interface[i];
        for(int alt = 0; alt < interface->num_altsetting; alt++) {
            print_interface(&interface->altsetting[alt]);
        }
    }
}

void print_device_info(libusb_device *dev) {
    libusb_device_descriptor desc;
    int ret = libusb_get_device_descriptor(dev, &desc);
    if (ret != LIBUSB_SUCCESS) {
        fprintf(stderr, "Error getting device descriptor: %s\n", libusb_error_name(ret));
        return;
    }

    printf("\nDEVICE ID %04x:%04x on Bus %03d Address %03d =================\n",
           desc.idVendor, desc.idProduct,
           libusb_get_bus_number(dev),
           libusb_get_device_address(dev));

    printf(" bLength                : 0x%02x (%d bytes)\n", desc.bLength, desc.bLength);
    printf(" bDescriptorType        : 0x%02x\n", desc.bDescriptorType);
    printf(" bcdUSB                 : 0x%04x USB %d.%d\n", 
           desc.bcdUSB, desc.bcdUSB >> 8, desc.bcdUSB & 0xff);
    printf(" bDeviceClass           : 0x%02x\n", desc.bDeviceClass);
    printf(" bDeviceSubClass        : 0x%02x\n", desc.bDeviceSubClass);
    printf(" bDeviceProtocol        : 0x%02x\n", desc.bDeviceProtocol);
    printf(" bMaxPacketSize0        : 0x%02x (%d bytes)\n", 
           desc.bMaxPacketSize0, desc.bMaxPacketSize0);
    printf(" idVendor               : 0x%04x\n", desc.idVendor);
    printf(" idProduct              : 0x%04x\n", desc.idProduct);
    printf(" bcdDevice              : 0x%04x Device %d.%d\n",
           desc.bcdDevice, desc.bcdDevice >> 8, desc.bcdDevice & 0xff);
    printf(" iManufacturer          : 0x%02x\n", desc.iManufacturer);
    printf(" iProduct               : 0x%02x\n", desc.iProduct);
    printf(" iSerialNumber          : 0x%02x\n", desc.iSerialNumber);
    printf(" bNumConfigurations     : 0x%02x\n", desc.bNumConfigurations);

    for(uint8_t cfg = 0; cfg < desc.bNumConfigurations; cfg++) {
        libusb_config_descriptor *config;
        if(libusb_get_config_descriptor(dev, cfg, &config) == LIBUSB_SUCCESS) {
            print_configuration(config);
            libusb_free_config_descriptor(config);
        }
    }
}

FS80Device check_device(libusb_device *device) {
    libusb_device_descriptor desc;
    FS80Device fs80_device = { nullptr, 0, 0 };  // Initialize with null/invalid values

    int ret = libusb_get_device_descriptor(device, &desc);
    
    if (ret != LIBUSB_SUCCESS) {
        fprintf(stderr, "[ERROR] Error getting descriptor: %s\n", libusb_error_name(ret));
        return fs80_device;  // Return invalid device
    }

    if (desc.idVendor == FS80_VID && desc.idProduct == FS80_PID) {
        libusb_ref_device(device);
        fs80_device.device = device;
        fs80_device.bus_number = libusb_get_bus_number(device);
        fs80_device.device_address = libusb_get_device_address(device);
    }

    return fs80_device;  // Returns valid device if found, invalid otherwise
}

std::vector<FS80Device> get_frutonic_devices() {
    std::vector<FS80Device> fs80_devices;
    libusb_context *ctx = nullptr;
    libusb_device **devices = nullptr;
    
    int ret = libusb_init(&ctx);
    if (ret != LIBUSB_SUCCESS) {
        fprintf(stderr, "[ERROR] Init failed: %s\n", libusb_error_name(ret));
        return fs80_devices;
    }

    // USB device list
    ssize_t count = libusb_get_device_list(ctx, &devices);
    if (count < 0) {
        fprintf(stderr, "[ERROR] Failed to get devices: %s\n", libusb_error_name((int)count));
        libusb_exit(ctx);
        return fs80_devices;
    }
    printf("[INFO] Scanning %zd USB devices...\n", count);
    
    for (ssize_t i = 0; i < count; i++) {
        FS80Device fs80_device = check_device(devices[i]);
        if(fs80_device.device != nullptr) {
            fs80_devices.push_back(fs80_device);
        }
    }

    libusb_free_device_list(devices, 1);
    libusb_exit(ctx);

    return fs80_devices;
}

int main() {
    std::vector<FS80Device> fs80_devices = get_frutonic_devices();

    if(fs80_devices.empty()){
        printf("[INFO] No Frutonic FS80 devices found.\n");
        return 0;
    }

    printf("[INFO] Found %zu FS80 devices\n", fs80_devices.size());

    for(const auto& fs80_device : fs80_devices){
        print_device_info(fs80_device.device);
    }

    return 0;
}