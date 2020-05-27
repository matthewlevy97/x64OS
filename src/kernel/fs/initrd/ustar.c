#include <fs/initrd/ustar.h>
#include <string.h>

static uint64_t oct2bin(unsigned char *str, int size);

/**
 * Note: inode_number is used as a pointer to the start of the file header
 */
uint64_t ustar_lookup(void *archive, const char *filename, uint64_t *inode_number) {
	unsigned char *ptr;
    uint64_t filesize;

    ptr = archive;

    while(!memcmp(ptr + USTAR_MAGIC_OFFSET, USTAR_MAGIC, USTAR_MAGIC_LEN)) {
        filesize = oct2bin(ptr + 0x7c, 11);

        if (!memcmp(ptr, filename, strlen(filename) + 1)) {
            *inode_number = (uint64_t)ptr - (uint64_t)archive;
            return filesize;
        }
        ptr += (((filesize + (USTAR_SECTOR_SIZE - 1)) / USTAR_SECTOR_SIZE) + 1) * USTAR_SECTOR_SIZE;
    }
    
    return 0;
}

uint64_t ustar_filesize(void *archive, uint64_t inode_number)
{
    unsigned char *ptr;

    ptr = (unsigned char*)archive + inode_number;
    return oct2bin(ptr + 0x7c, 11);
}

void *ustar_copy_data(void *archive, uint64_t inode_number, void *buf, uint64_t bytes, off_t offset)
{
    unsigned char *ptr;

    ptr = (unsigned char*)archive + inode_number + USTAR_SECTOR_SIZE + offset;
    return memcpy(buf, ptr, bytes);
}

static inline uint64_t oct2bin(unsigned char *str, int size) {
	uint64_t ret;

	ret = 0;
	while(size--) {
		ret *= 8;
		ret += *str - '0';
		str++;
	}

	return ret;
}