#include <stdio.h>
#include "lusb0_usb.h"

#define FS80_VID 0x1491
#define FS80_PID 0x0020

int check_device(struct usb_device *dev) {
    const struct usb_device_descriptor *desc = &(dev->descriptor);
    
    if (desc->idVendor == FS80_VID && desc->idProduct == FS80_PID) {
        printf("[INFO] Frutonic FS80 detected!");
        printf("  VID: 0x%04x", desc->idVendor);
        printf("  PID: 0x%04x\n", desc->idProduct);
        return 1;
    }
    return 0;
}

void list_usb_devices() {
    usb_init();
    
    printf("Finding busses...\n");
    if(usb_find_busses() < 0) {
        printf("Error finding busses!\n");
        return;
    }
    
    printf("Finding devices...\n");
    if(usb_find_devices() < 0) {
        printf("Error finding devices!\n");
        return;
    }

    struct usb_bus *bus = usb_get_busses();
    if(!bus) {
        printf("No busses found!\n");
        return;
    }

    printf("\n=== USB Devices ===\n");
    while(bus) {
        printf("Bus: %s\n", bus->dirname);
        struct usb_device *dev = bus->devices;
        
        if(!dev) {
            printf("  (No devices)\n");
        }
        
        while(dev) {
            printf("  ├─ VID: 0x%04x PID: 0x%04x\n", 
                  dev->descriptor.idVendor,
                  dev->descriptor.idProduct);
            dev = dev->next;
        }
        bus = bus->next;
    }
}

void find_frutonic_devices() {
    struct usb_bus *bus;
    struct usb_device *dev;
    int count = 0;
    
    usb_init();
    usb_find_busses();
    usb_find_devices();
    
    for (bus = usb_get_busses(); bus; bus = bus->next) {
        for (dev = bus->devices; dev; dev = dev->next) {
            if (check_device(dev)) count++;
        }
    }
    
    printf("[INFO] Found %d FS80 device(s)\n", count);
}
int main() {

    find_frutonic_devices();
    return 0;
}