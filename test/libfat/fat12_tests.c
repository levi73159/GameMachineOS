#include "fat12_tests.h"
#include "../../src/testing/testing.h"
#include "../../src/libs/libfat/fat.h"
#include <assert.h>


TEST(libfat_fat12_smoke_test)
{
    DISK disk;
    FAT_Initialize(&disk);

    assert(disk.h)
}