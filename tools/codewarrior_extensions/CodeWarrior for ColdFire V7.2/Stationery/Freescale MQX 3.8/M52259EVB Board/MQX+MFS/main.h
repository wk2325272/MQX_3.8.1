#ifndef __main_h_
#define __main_h_


#define MAIN_TASK     1
#define RAM_DISK_SIZE 0x2000

/* Uncomment following code to use whole external MRAM for ramdisk, if available.
   Not applicable when using MRAM target! */
/*
#define RAM_DISK_BASE BSP_EXTERNAL_MRAM_BASE 
#define RAM_DISK_SIZE BSP_EXTERNAL_MRAM_SIZE
*/


extern void Main_task (uint_32);
void Ram_disk_start(void);


#endif /* __main_h_ */

