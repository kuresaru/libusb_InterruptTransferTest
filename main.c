#include <stdio.h>
#include <stdint.h>
#include <libusb-1.0/libusb.h>

#define VID 0x1234
#define PID 0x5678

int main(int argc, char **argv)
{
    libusb_context *ctx;
    libusb_device_handle *hdev;
    int r;

    char buf[16];
    int len;

    r = libusb_init(&ctx);
    if (r < 0)
    {
        printf("Init Libusb Error\n");
        return 1;
    }
    libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_INFO);
    hdev = libusb_open_device_with_vid_pid(ctx, VID, PID);
    if (hdev == NULL)
    {
        printf("Open Device Error\n");
        goto exit_lib;
    }
    printf("Open Device OK\n");
    r = libusb_claim_interface(hdev, 0);
    if (r < 0)
    {
        printf("Claim Interface Error\n");
        goto exit_device;
    }
    printf("Claim Interface OK\n");

    for (int i = 0; i < 400; i++)
    {
        r = libusb_interrupt_transfer(hdev, 0x81, buf, 1, &len, 1000);
        if (len > 0)
        {
            printf("%d %d\n", i, buf[0]);
            buf[0] = !(i / 10 % 2);
            libusb_interrupt_transfer(hdev, 0x01, buf, 1, &len, 100);
        }
    }

exit_device:
    libusb_close(hdev);
exit_lib:
    libusb_exit(ctx);
    return 0;
}