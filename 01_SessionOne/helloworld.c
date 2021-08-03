#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include "xgpiops.h"
#include "xbram.h"


#define mio_led0 20
#define mio_led1 19
#define mio_led2 18
#define mio_led3 17

#define word_1 0x4841434b
#define word_2 0x53544552
#define word_3 0x20554c54
#define word_4 0x52413936
#define word_5 0x20545554
#define word_6 0x4f524941
#define word_7 0x0000004c


XGpioPs Gpio;
XBram   Bram;
int main()
{

	int Status,i;
	XGpioPs_Config *GPIOConfigPtr;
	XBram_Config *BRAMConfigPtr;
	u32 secret_word[8];

    init_platform();

    BRAMConfigPtr = XBram_LookupConfig(XPAR_BRAM_0_DEVICE_ID);

	Status = XBram_CfgInitialize(&Bram, BRAMConfigPtr,BRAMConfigPtr->CtrlBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	for(i=0;i<8;i++){
		secret_word[i] = XBram_ReadReg(XPAR_BRAM_0_BASEADDR, i*4);
	}

    GPIOConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);

    Status = XGpioPs_CfgInitialize(&Gpio, GPIOConfigPtr, GPIOConfigPtr ->BaseAddr);

    if (Status != XST_SUCCESS) {

    return XST_FAILURE;

    }

    XGpioPs_SetDirectionPin(&Gpio, mio_led0, 1);
    XGpioPs_SetOutputEnablePin(&Gpio, mio_led0, 1);
    XGpioPs_SetDirectionPin(&Gpio, mio_led1, 1);
    XGpioPs_SetOutputEnablePin(&Gpio, mio_led1, 1);
    XGpioPs_SetDirectionPin(&Gpio, mio_led2, 1);
    XGpioPs_SetOutputEnablePin(&Gpio, mio_led2, 1);
    XGpioPs_SetDirectionPin(&Gpio, mio_led3, 1);
    XGpioPs_SetOutputEnablePin(&Gpio, mio_led3, 1);
    while(1){

        if( (secret_word[1]==word_1) && (secret_word[2]==word_2) && (secret_word[3]==word_3)
            && (secret_word[4]==word_4) && (secret_word[5]==word_5) && (secret_word[6]==word_6)
    		&& (secret_word[7]==word_7))
        {
			XGpioPs_WritePin(&Gpio, mio_led0, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led1, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led2, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led3, 0x1);
			usleep(1000000);
			XGpioPs_WritePin(&Gpio, mio_led0, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led1, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led2, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led3, 0x0);
			usleep(1000000);
        }
        else
        {
			XGpioPs_WritePin(&Gpio, mio_led0, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led1, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led2, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led3, 0x1);
			usleep(1000000);
			XGpioPs_WritePin(&Gpio, mio_led0, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led1, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led2, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led3, 0x0);
			usleep(1000000);
        }



    }

    cleanup_platform();
    return 0;
}
