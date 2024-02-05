#include <stdint.h>
#include "stdio.h"
#include "x86.h"
#include "disk.h"
#include "fat.h"
#include "memdefs.h"
#include "memory.h"
#include "mbr.h"

uint8_t *KernelLoadBuffer = (uint8_t *)MEMORY_LOAD_KERNEL;
uint8_t *Kernel = (uint8_t *)MEMORY_KERNEL_ADDR;

typedef void (*KernelStart)();

void __attribute__((cdecl)) start(uint16_t bootDrive, void *partition)
{
    clear();

    printf("Booting Game Machine, partition=0x%x...\r\n", partition);
    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive))
    {
        printf("Disk init error\r\n");
        goto end;
    }
    printf("Disk init success\r\n");

    printf("Getting Partition...\r\n");

    Partition part;
    MBR_DetectPartition(&part, &disk, partition);

    printf("FAT Initializing...\r\n");
    if (!FAT_Initialize(&part))
    {
        printf("FAT init error\r\n");
        goto end;
    }
    printf("FAT init success\r\n");

    // load kernel
    FAT_File *fd = FAT_Open(&part, "/boot/kernel.bin");
    uint32_t read;
    uint8_t *kernelBuffer = Kernel;
    while ((read = FAT_Read(&part, fd, MEMORY_LOAD_SIZE, KernelLoadBuffer)))
    {
        memcpy(kernelBuffer, KernelLoadBuffer, read);
        kernelBuffer += read;
    }
    FAT_Close(fd);

    if (!fd)
    {
        printf("ERROR: Kernel not found!\r\n");
        goto end;
    }
    clear();
    printf("GameMachine Kernel version 1.1.0 Booted into memory...\r\nRunning Kernel...\r\n");

    // execute kernel
    KernelStart kernelStart = (KernelStart)Kernel;
    kernelStart();

end:
    for (;;)
        ;
}
