#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xmbox.h"
#include "xstatus.h"
#include "xmutex.h"


#define MSGSIZ  1024
#define MUTEX_DEVICE_ID		XPAR_MUTEX_0_IF_0_DEVICE_ID
#define MUTEX_NUM 		0
#define TEST_BUFFER_SIZE 16
char RecvMsg[MSGSIZ] __attribute__ ((aligned(4)));

char *hello = "MicroBlaze Starting Up  ";
char *ledon = "LED On  ";
char *ledoff = "LED Off ";

int main()
{
	XMbox Mbox;
	XMbox_Config *ConfigPtr;
	XMutex Mutex;
	XMutex_Config *MTConfigPtr;
	XStatus Status;


    init_platform();

    ConfigPtr = XMbox_LookupConfig(XPAR_MBOX_0_DEVICE_ID );
    XMbox_CfgInitialize(&Mbox, ConfigPtr, ConfigPtr->BaseAddress);

    MTConfigPtr = XMutex_LookupConfig(MUTEX_DEVICE_ID);
    XMutex_CfgInitialize(&Mutex, MTConfigPtr,	MTConfigPtr->BaseAddress);


    printf("Xilinx Training MERG / SERG \n\r");

    XMbox_ReadBlocking(&Mbox, (u32*)(RecvMsg), strlen(hello));
    printf ("Rcvd the message --> \r\n\r\n\t--[%s]--\r\n\r\n", RecvMsg);

    memset(RecvMsg, 0, MSGSIZ);

    while(1){
    	XMbox_ReadBlocking(&Mbox, (u32*)(RecvMsg), strlen(ledon));
    	Status = XMutex_IsLocked(&Mutex, MUTEX_NUM);
    	if (Status != XST_SUCCESS){
    		printf("Mutex Locked\n\r");
    	}else{
    		printf("Mutex Unlocked\n\r");
    	}
    	printf ("Rcvd the message --> \r\n\r\n\t--[%s]--\r\n\r\n", RecvMsg);
    }


    cleanup_platform();
    return 0;
}
