#include <stdint.h>
#include "stdio.h"
#include "x86.h"
#include "disk.h"
#include "fat.h"
#include "memdefs.h"
#include "memory.h"
#include "vbe.h"

uint8_t *KernelLoadBuffer = (uint8_t *)MEMORY_LOAD_KERNEL;
uint8_t *Kernel = (uint8_t *)MEMORY_KERNEL_ADDR;

#define COLOR(r, g, b) ((b) | (g << 8) | (r << 16))
#define CORD(x, y, info) (y * info->pitch / 4 + x)

typedef void (*KernelStart)();

static uint16_t *pickedMode = (uint16_t *)MEMORY_PICKED_MODE;

void clearScreen(int w, int h, VbeModeInfo *minf, uint32_t color, uint32_t *fb)
{
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            fb[CORD(x, y, minf)] = color;
        }
    }
}

void setMode()
{
    if (!VBE_SetMode(*pickedMode))
    {
        printf("Invalid Mode");
        return;
    }
}

void __attribute__((cdecl)) start(uint16_t bootDrive)
{
    clear();

    printf("Booting Game Machine...\r\n");
    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive))
    {
        printf("Disk init error\r\n");
        goto end;
    }
    printf("Disk init success\r\n");

    if (!FAT_Initialize(&disk))
    {
        printf("FAT init error\r\n");
        goto end;
    }
    printf("FAT init success\r\n");

    // load kernel
    FAT_File *fd = FAT_Open(&disk, "/kernel.bin");
    uint32_t read;
    uint8_t *kernelBuffer = Kernel;
    while ((read = FAT_Read(&disk, fd, MEMORY_LOAD_SIZE, KernelLoadBuffer)))
    {
        memcpy(kernelBuffer, KernelLoadBuffer, read);
        kernelBuffer += read;
    }
    FAT_Close(fd);
    printf("GameMachine Kernel version 1.0.1 Loaded into memory...\r\n");

    const int desiredWidth = 1024;
    const int desiredHeight = 768;
    const int desiredBpp = 32;

    // Initialize graphics
    VbeInfoBlock *info = (VbeInfoBlock *)MEMORY_VESA_INFO;
    VbeModeInfo *modeInfo = (VbeModeInfo *)MEMORY_MODE_INFO;
    if (VBE_GetControllerInfo(info))
    {
        uint16_t *mode = (uint16_t *)(info->VideoModePtr);
        for (int i = 0; mode[i] != 0xffff; i++)
        {
            if (!VBE_GetModeInfo(mode[i], modeInfo))
            {
                printf("Can't get mode info %x :(\n", mode[i]);
                continue;
            }
            bool hasFB = (modeInfo->attributes & 0x90) == 0x90;
            if (!hasFB || modeInfo->width != desiredWidth || modeInfo->height != desiredHeight || modeInfo->bpp != desiredBpp)
                continue;

            printf("Found mode %x, %dx%dx%d 0x%x\n", mode[i], modeInfo->width, modeInfo->height, modeInfo->bpp, modeInfo->framebuffer);
            *pickedMode = mode[i];
            break;
        }
        // store the picked mode to memory

        if (*pickedMode == 0xffff)
        {
            printf("Failed to pick mode with %dx%dx%d\n", desiredWidth, desiredHeight, desiredBpp);
            return;
        }
        setMode();
    }
    else
    {
        printf("No VBE extensions :(\n");
    }

    // execute kernel
    KernelStart kernelStart = (KernelStart)Kernel;
    kernelStart();

end:
    for (;;)
        ;
}
