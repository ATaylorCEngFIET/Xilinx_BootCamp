#include "xparameters.h"
#include "xmbox.h"
#include "xstatus.h"
#include<string.h>
#include "xmutex.h"


#define MUTEX_DEVICE_ID		XPAR_MUTEX_0_IF_1_DEVICE_ID
#define MUTEX_NUM 		0


int toggle;//used to toggle the LED
#define LED_DELAY     1000000

int main()
{
	XMbox Mbox;
	XMbox_Config *ConfigPtr;
	XMutex Mutex;
	XMutex_Config *MTConfigPtr;
	int Status;
	u32 NumBytes;
	u32 Sent;
	char *hello = "MicroBlaze Starting Up  ";
	char *ledon = "LED On  ";
	char *ledoff = "LED Off ";


	ConfigPtr = XMbox_LookupConfig(XPAR_MBOX_0_DEVICE_ID );
	Status = XMbox_CfgInitialize(&Mbox, ConfigPtr, ConfigPtr->BaseAddress);

    MTConfigPtr = XMutex_LookupConfig(MUTEX_DEVICE_ID);
    XMutex_CfgInitialize(&Mutex, MTConfigPtr,	MTConfigPtr->BaseAddress);
	XMbox_WriteBlocking(&Mbox, (u32*)((u8*)hello), strlen(hello));

	while(1){
		XMbox_WriteBlocking(&Mbox, (u32*)((u8*)ledon), strlen(ledon));
		XMutex_Lock(&Mutex, MUTEX_NUM);
		usleep(LED_DELAY);
		XMutex_Unlock(&Mutex, MUTEX_NUM);
		XMbox_WriteBlocking(&Mbox, (u32*)((u8*)ledoff), strlen(ledoff));
		usleep(LED_DELAY);
	}
}
