#ifndef USIM_MISC_H
#define USIM_MISC_H

extern unsigned int read16(int fd);
extern unsigned int read32(int fd);

extern unsigned long str4(char *s);
extern char *unstr4(unsigned long s);

#define BLOCKSZ (256 * 4)

extern int read_block(int fd, int block_no, unsigned char *buf);
extern int write_block(int fd, int block_no, unsigned char *buf);

#endif
