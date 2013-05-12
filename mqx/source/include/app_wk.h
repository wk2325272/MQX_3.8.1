void uart_isr();
extern uchar read_buffer[7];  // 触摸屏按下时接收数据
extern void LCDTouchSel(void); // 触摸屏按键判断程序

extern unsigned int  DataSize; // DMA 的 major count 的次数，相当于一次DMA服务接收数据的个数

extern uchar BufRxchar[6175]; // DMA接收数据的缓存区
extern void DMA_RecData_OK(); //DMA 数据接收完成后的处理函数
extern uchar SPI_Send; // WK @130406 --> SPI是否向下发送数据的标志
extern uchar SysDataSend[56]; // K60写给DSP的数据

extern void DisTimeFlg(); //wk @130510 --> KRTC.C
//#define _UART_DBUG_  // wk --> 测试用：是否需要调试信息输出
//#define _SPIDMA_DBUG_

