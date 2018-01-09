unsigned int read16(int fd);
unsigned int read32(int fd);

unsigned long str4(char *s);
char *unstr4(unsigned long s);

#define BLOCKSZ (256 * 4)

int read_block(int fd, int block_no, unsigned char *buf);
int write_block(int fd, int block_no, unsigned char *buf);
