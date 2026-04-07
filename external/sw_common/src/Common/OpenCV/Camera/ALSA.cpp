#include "Common/OpenCV/Camera/ALSA.hpp"

struct udev_device* getParentUSBDevice(struct udev_device* dev) {
    return udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
}

std::optional< std::string > findAudioALSAByCamera(const std::string& symlink_path) {
    std::optional< std::string > found_audio_device;

    /* libudev init */
    struct udev* ctx = udev_new();
    if (!ctx)
        throw std::runtime_error("Failed to create udev context");

    /* Get information about video device via symlink */
    struct stat dev_stat;
    /* Use stat() instead of lstat() to get info about /dev/videoX not the symlink itself */
    if (stat(symlink_path.c_str(), &dev_stat) != 0) {
        udev_unref(ctx);
        throw std::runtime_error("Failed to get stat for " + symlink_path);
    }

    if (!S_ISCHR(dev_stat.st_mode)) {
        udev_unref(ctx);
        throw std::runtime_error(symlink_path + " isn't symbolic link");
    }

    struct udev_device* video_dev = udev_device_new_from_devnum(ctx, 'c', dev_stat.st_rdev);
    if (!video_dev) {
        udev_unref(ctx);
        throw std::runtime_error("Failed to find device by devnum");
    }

    /* Find parent USB-device for VIDEO */
    struct udev_device* video_usb_parent = getParentUSBDevice(video_dev);
    if (!video_usb_parent) {
        udev_device_unref(video_dev);
        udev_unref(ctx);
        throw std::runtime_error("Failed to find parent USB-device for " + symlink_path);
    }

    const char* video_parent_syspath = udev_device_get_syspath(video_usb_parent);
    std::string video_path_str(video_parent_syspath);

    /* Iteration through all audio devices */
    struct udev_enumerate* enumerate = udev_enumerate_new(ctx);
    udev_enumerate_add_match_subsystem(enumerate, "sound");
    udev_enumerate_add_match_sysname(enumerate, "card*"); /* Looking for cardX only */
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry* dev_list_entry;

    udev_list_entry_foreach(dev_list_entry, devices) {
        const char*         path           = udev_list_entry_get_name(dev_list_entry);
        struct udev_device* audio_card_dev = udev_device_new_from_syspath(ctx, path);

        if (!audio_card_dev)
            continue;

        /* Find parent USB-device for AUDIO */
        struct udev_device* audio_usb_parent = getParentUSBDevice(audio_card_dev);

        if (audio_usb_parent) {
            const char* audio_parent_syspath = udev_device_get_syspath(audio_usb_parent);

            if (video_path_str == audio_parent_syspath) {
                /* Audio found */
                const char* card_sysname = udev_device_get_sysname(audio_card_dev);
                if (card_sysname) {
                    std::string card_name(card_sysname);
                    /* Remove "card" from begin */
                    std::string card_num = card_name.substr(4);
                    found_audio_device   = "hw:" + card_num + ",0";
                }

                udev_device_unref(audio_card_dev);
                break; /* Stop searching */
            }
        }
        udev_device_unref(audio_card_dev);
    }

    /* Clear resources */
    udev_enumerate_unref(enumerate);
    udev_device_unref(video_dev);
    udev_unref(ctx);

    return found_audio_device;
}