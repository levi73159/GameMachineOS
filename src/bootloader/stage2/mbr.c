#include "mbr.h"
#include "memory.h"

typedef struct
{
    uint8_t attributes;    // 0x00 1       Drive attributes (bit 7 set = active or bootable)
    uint8_t chsStart[3];   // 0x01 3       CHS Address of partition start
    uint8_t partitionType; // 0x04 1       Partition Type
    uint8_t chsEnd[3];     // 0x05 3       CHS address of last partition sector
    uint32_t lbaStart;     // 0x08 4       LBA of partition start
    uint32_t size;         // 0x0C 4       Number of sectors in partition

} __attribute__((packed)) MBR_Entry;

void MBR_DetectPartition(Partition* partition, DISK* disk, void *partitionEntry)
{
    partition->disk = disk;
    if (disk->id < 0x80)
    {
        partition->partitionOffset = 0;
        partition->partitionSize = (uint32_t)disk->cylinders * (uint32_t)disk->heads * (uint32_t)disk->sectors;
        return;
    }

    MBR_Entry *entry = (MBR_Entry *)segOffsetToLinear(partitionEntry);

    printf("-attributes=0x%x\n", entry->attributes);
    printf("-lbaStart=0x%x\n", entry->lbaStart);
    printf("-size=0x%x\n", entry->size);
    printf("-partitionType=0x%x\n", entry->partitionType);
    printf("part-partitionOffset=0x%x\n", partition->partitionOffset);

    partition->partitionOffset = entry->lbaStart;
    partition->partitionSize = entry->size;
}

bool Partition_ReadSectors(Partition *partition, uint32_t lba, uint8_t sectors, void *dataOut)
{
    return DISK_ReadSectors(partition->disk, lba + partition->partitionOffset, sectors, dataOut);
}