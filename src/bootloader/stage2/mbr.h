#pragma once
#include <stdint.h>
#include "disk.h"

typedef struct
{
    DISK *disk;
    uint32_t partitionOffset;
    uint32_t partitionSize;

} Partition;

void MBR_DetectPartition(Partition *partition, DISK *disk, void *partitionEntry);
bool Partition_ReadSectors(Partition *disk, uint32_t lba, uint8_t sectors, void *lowerDataOut);