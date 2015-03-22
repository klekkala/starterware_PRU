#include <stdio.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include <poll.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdint.h>
 
#define PRUSS_SHAREDRAM_BASE     0x4a312000
#define NUM_RING_ENTRIES 20
 
struct ring_buffer {
        volatile uint8_t ring_head;
        volatile uint8_t ring_tail;
        struct __attribute__((__packed__)) {
                char str[10];
                uint8_t integers[4];
        } buffer[NUM_RING_ENTRIES];
};
 
 
static volatile struct ring_buffer *ring_buffer;
 
void check_for_rcin(void)
{
        while (ring_buffer->ring_head != ring_buffer->ring_tail) {
                printf("%s %u %u\n",ring_buffer->buffer[ring_buffer->ring_head].str, (unsigned int)ring_buffer->ring_head,
                                ring_buffer->buffer[ring_buffer->ring_head].integers[0]); // you can print upto 3 integers here
                ring_buffer->ring_head = (ring_buffer->ring_head + 1) % NUM_RING_ENTRIES;
        }
}
 
int main(){
 
        int mem_fd = open("/dev/mem", O_RDWR|O_SYNC);
        ring_buffer = (volatile struct ring_buffer*) mmap(0, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, PRUSS_SHAREDRAM_BASE);
        close(mem_fd);
        ring_buffer->ring_head = 0;
        while(1){
                check_for_rcin();
        }
        return 0;
}
