#include <stdio.h>
#include <string.h>
#include <pigpio.h>
#include <unistd.h>

#include "microDotDisplay.h"

int controller1Handle=0;
int controller2Handle=0;
int controller3Handle=0;

unsigned char const fontLookup[][FONT_ROWS] = { 
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // 0x20, Space 
   {0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04},   // 0x21, ! 
   {0x09, 0x09, 0x12, 0x00, 0x00, 0x00, 0x00},   // 0x22, " 
   {0x0a, 0x0a, 0x1f, 0x0a, 0x1f, 0x0a, 0x0a},   // 0x23, # 
   {0x04, 0x0f, 0x14, 0x0e, 0x05, 0x1e, 0x04},   // 0x24, $ 
   {0x19, 0x19, 0x02, 0x04, 0x08, 0x13, 0x13},   // 0x25, % 
   {0x04, 0x0a, 0x0a, 0x0a, 0x15, 0x12, 0x0d},   // 0x26, & 
   {0x04, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00},   // 0x27, ' 
   {0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02},   // 0x28, ( 
   {0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08},   // 0x29, ) 
   {0x04, 0x15, 0x0e, 0x1f, 0x0e, 0x15, 0x04},   // 0x2a, * 
   {0x00, 0x04, 0x04, 0x1f, 0x04, 0x04, 0x00},   // 0x2b, + 
   {0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x08},   // 0x2c, , 
   {0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00},   // 0x2d, - 
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c},   // 0x2e, . 
   {0x01, 0x01, 0x02, 0x04, 0x08, 0x10, 0x10},   // 0x2f, / 
   {0x0e, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0e},   // 0x30, 0 
   {0x04, 0x0c, 0x04, 0x04, 0x04, 0x04, 0x0e},   // 0x31, 1 
   {0x0e, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1f},   // 0x32, 2 
   {0x0e, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0e},   // 0x33, 3 
   {0x02, 0x06, 0x0a, 0x12, 0x1f, 0x02, 0x02},   // 0x34, 4 
   {0x1f, 0x10, 0x1e, 0x01, 0x01, 0x11, 0x0e},   // 0x35, 5 
   {0x06, 0x08, 0x10, 0x1e, 0x11, 0x11, 0x0e},   // 0x36, 6 
   {0x1f, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08},   // 0x37, 7 
   {0x0e, 0x11, 0x11, 0x0e, 0x11, 0x11, 0x0e},   // 0x38, 8 
   {0x0e, 0x11, 0x11, 0x0f, 0x01, 0x02, 0x0c},   // 0x39, 9 
   {0x00, 0x0c, 0x0c, 0x00, 0x0c, 0x0c, 0x00},   // 0x3a, : 
   {0x00, 0x0c, 0x0c, 0x00, 0x0c, 0x04, 0x08},   // 0x3b, ; 
   {0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02},   // 0x3c, < 
   {0x00, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x00},   // 0x3d, = 
   {0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08},   // 0x3e, > 
   {0x0e, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04},   // 0x3f, ? 
   {0x0e, 0x11, 0x17, 0x15, 0x17, 0x10, 0x0f},   // 0x40, @ 
   {0x04, 0x0a, 0x11, 0x11, 0x1f, 0x11, 0x11},   // 0x41, A 
   {0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x1e},   // 0x42, B 
   {0x0e, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0e},   // 0x43, C 
   {0x1e, 0x09, 0x09, 0x09, 0x09, 0x09, 0x1e},   // 0x44, D 
   {0x1f, 0x10, 0x10, 0x1c, 0x10, 0x10, 0x1f},   // 0x45, E 
   {0x1f, 0x10, 0x10, 0x1f, 0x10, 0x10, 0x10},   // 0x46, F 
   {0x0e, 0x11, 0x10, 0x10, 0x13, 0x11, 0x0f},   // 0x37, G 
   {0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11},   // 0x48, H 
   {0x0e, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0e},   // 0x49, I 
   {0x1f, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0c},   // 0x4a, J 
   {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11},   // 0x4b, K 
   {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f},   // 0x4c, L 
   {0x11, 0x1b, 0x15, 0x11, 0x11, 0x11, 0x11},   // 0x4d, M 
   {0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11},   // 0x4e, N 
   {0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e},   // 0x4f, O 
   {0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10, 0x10},   // 0x50, P 
   {0x0e, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0d},   // 0x51, Q 
   {0x1e, 0x11, 0x11, 0x1e, 0x14, 0x12, 0x11},   // 0x52, R 
   {0x0e, 0x11, 0x10, 0x0e, 0x01, 0x11, 0x0e},   // 0x53, S 
   {0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04},   // 0x54, T 
   {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e},   // 0x55, U 
   {0x11, 0x11, 0x11, 0x11, 0x11, 0x0a, 0x04},   // 0x56, V 
   {0x11, 0x11, 0x11, 0x15, 0x15, 0x1b, 0x11},   // 0x57, W 
   {0x11, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x11},   // 0x58, X 
   {0x11, 0x11, 0x0a, 0x04, 0x04, 0x04, 0x04},   // 0x59, Y 
   {0x1f, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1f},   // 0x5a, Z 
   {0x0e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0e},   // 0x5b, [ 
   {0x10, 0x10, 0x08, 0x04, 0x02, 0x01, 0x01},   // 0x5c, backslash
   {0x0e, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0e},   // 0x5d, ] 
   {0x04, 0x0a, 0x11, 0x00, 0x00, 0x00, 0x00},   // 0x5e, ^ 
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f},   // 0x5f, _ 
   {0x04, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00},   // 0x60, ` 
   {0x00, 0x0e, 0x01, 0x0d, 0x13, 0x13, 0x0d},   // 0x61, a 
   {0x10, 0x10, 0x10, 0x1c, 0x12, 0x12, 0x1c},   // 0x62, b 
   {0x00, 0x00, 0x00, 0x0e, 0x10, 0x10, 0x0e},   // 0x63, c 
   {0x01, 0x01, 0x01, 0x07, 0x09, 0x09, 0x07},   // 0x64, d 
   {0x00, 0x00, 0x0e, 0x11, 0x1f, 0x10, 0x0f},   // 0x65, e 
   {0x06, 0x09, 0x08, 0x1c, 0x08, 0x08, 0x08},   // 0x66, f 
   {0x0e, 0x11, 0x13, 0x0d, 0x01, 0x01, 0x0e},   // 0x67, g 
   {0x10, 0x10, 0x10, 0x16, 0x19, 0x11, 0x11},   // 0x68, h 
   {0x00, 0x04, 0x00, 0x0c, 0x04, 0x04, 0x0e},   // 0x69, i 
   {0x02, 0x00, 0x06, 0x02, 0x02, 0x12, 0x0c},   // 0x6a, j 
   {0x10, 0x10, 0x12, 0x14, 0x18, 0x14, 0x12},   // 0x6b, k 
   {0x0c, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04},   // 0x6c, l 
   {0x00, 0x00, 0x0a, 0x15, 0x15, 0x11, 0x11},   // 0x6d, m 
   {0x00, 0x00, 0x16, 0x19, 0x11, 0x11, 0x11},   // 0x6e, n 
   {0x00, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x0e},   // 0x6f, o 
   {0x00, 0x1c, 0x12, 0x12, 0x1c, 0x10, 0x10},   // 0x70, p 
   {0x00, 0x07, 0x09, 0x09, 0x07, 0x01, 0x01},   // 0x71, q 
   {0x00, 0x00, 0x16, 0x19, 0x10, 0x10, 0x10},   // 0x72, r 
   {0x00, 0x00, 0x0f, 0x10, 0x0e, 0x01, 0x1e},   // 0x73, s 
   {0x08, 0x08, 0x1c, 0x08, 0x08, 0x09, 0x06},   // 0x74, t 
   {0x00, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0d},   // 0x75, u 
   {0x00, 0x00, 0x11, 0x11, 0x11, 0x0a, 0x04},   // 0x76, v 
   {0x00, 0x00, 0x11, 0x11, 0x15, 0x15, 0x0a},   // 0x77, w 
   {0x00, 0x00, 0x11, 0x0a, 0x04, 0x0a, 0x11},   // 0x78, x 
   {0x00, 0x11, 0x11, 0x0f, 0x01, 0x11, 0x0e},   // 0x79, y 
   {0x00, 0x00, 0x1f, 0x02, 0x04, 0x08, 0x1f},   // 0x7a, z 
   {0x06, 0x08, 0x08, 0x10, 0x08, 0x08, 0x06},   // 0x7b, { 
   {0x04, 0x04, 0x04, 0x00, 0x04, 0x04, 0x04},   // 0x7c, | 
   {0x0c, 0x02, 0x02, 0x01, 0x02, 0x02, 0x0c},   // 0x7d, } 
   {0x08, 0x15, 0x02, 0x00, 0x00, 0x00, 0x00},   // 0x7e, ~ 
   {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}    // 0x7f, DEL 
   };

char display[DIGITS][MATRIX_ROWS];

void initController(int handle)
{
	i2cWriteByteData(handle,CMD_RESET,0); 		// any value written to this register, resets registers to default, blanks display
	i2cWriteByteData(handle,CMD_CONFIG,CONFIG);	// setup for 8x8 matrix etc
	i2cWriteByteData(handle,CMD_LIGHTING,LIGHTING);	// set 35ma current for each row
}

void setBrightness(int handle,int value)
{
	i2cWriteByteData(handle,CMD_PWM,value);		// control brightness of display using PWM
}

void microDotDisplayInit()
{
	if (gpioInitialise() < 0)
	{
		return;
	}
	controller1Handle=i2cOpen(1,DIGIT_ADDR1,0);
	if (controller1Handle<0)
	{
		fprintf(stderr,"Unable to open controller 1\n");
		return;
	}
	initController(controller1Handle);

	controller2Handle=i2cOpen(1,DIGIT_ADDR2,0);
	if (controller2Handle<0)
	{
		fprintf(stderr,"Unable to open controller 2\n");
		return;
	}
	initController(controller2Handle);

	controller3Handle=i2cOpen(1,DIGIT_ADDR3,0);
	if (controller3Handle<0)
	{
		fprintf(stderr,"Unable to open controller 3\n");
		return;
	}
	initController(controller3Handle);

	setBrightness(controller1Handle,DIM);
	setBrightness(controller2Handle,DIM);
	setBrightness(controller3Handle,VERY_DIM);

	updateDisplay();
}

void clearDigit(int digit)
{
	int i;

	switch(digit)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			for(i=0;i<MATRIX_ROWS;i++)
			{
				display[digit][i]=0;
			}
			break;
	}
}

void clearDisplay()
{
	int i;

	for(i=0;i<DIGITS;i++)
	{
		clearDigit(i);
	}
}



void setPixel(int digit,int x, int y,int state)
{
	if ( (digit==1) || (digit==3) || (digit==5) )
	{
		if (state>0)
		{
			display[digit][y] |= 0b1 << x;
		}
		else
		{
			display[digit][y] &= ~(0b1 << x);
		}
	}
	else
	{
		if (state>0)
		{
			display[digit][x] |= 0b1 << y;
		}
		else
		{
			display[digit][x] &= ~(0b1 << y);
		}
	}
}

void setDigit(int digit,char c)
{
	int row,col,i;
	char val;

	for(row=0;row<FONT_ROWS;row++)
	{
		i=c-32; // font starts from space...
		val=fontLookup[i][row];
		val=val<<3;
		for(col=0;col<FONT_COLS;col++)
		{
			setPixel(digit,col,row, (val & 0b10000000) );
			val=val<<1;
		}
	}	
}

void setScrollDigit(int digit,char c,char oldC,int scroll)
{
	int row,col,i;
	char val;

	for(row=0;row<FONT_ROWS;row++)
	{
		if (row+scroll<FONT_ROWS)
		{
			i=oldC-32; // font starts from space...
			val=fontLookup[i][row+scroll];
		}
		else
		{
			i=c-32;
			val=fontLookup[i][row+scroll-FONT_ROWS];
		}
		val=val<<3; // we have a 5x7 font, matrix is 8x8... so lets shift a little to get to the edge of chararcter
		for(col=0;col<FONT_COLS;col++)
		{
			setPixel(digit,col,row, (val & 0b10000000) );
			val=val<<1;
		}
	}	
}

void displayString(char *string)
{
	int len,i;

	clearDisplay();
	len=strlen(string);
	for(i=0;i<len;i++)
	{
		setDigit(i,string[i]);
	}
	updateDisplay();
}

void scrollUpString(char *string)
{
	static char oldString[]="      ";
	int i,len,scroll;

	clearDisplay();
	len=strlen(string);
	for(scroll=1;scroll<=FONT_ROWS;scroll++)
	{
		for(i=0;i<DIGITS;i++)
		{
			if (i<len)
			{
				if (string[i]==oldString[i])
				{
					setDigit(i,string[i]);
				}
				else
				{
					setScrollDigit(i,string[i],oldString[i],scroll);
				}
			}
			else
			{
				setDigit(i,' ');
			}
		}
		updateDisplay();
	}
	for(i=0;i<DIGITS;i++)
	{
		if (i<len)
		{
			oldString[i]=string[i];	
		}
		else
		{
			oldString[i]=' ';
		}
	}
}
	

void setDecimalPoint(int digit,int value)
{
	if ( (digit==1) || (digit==3) || (digit==5) )
	{
		if (value)
		{
               		display[digit][6] |= 0b10000000;    
           	}
		else
		{
               		display[digit][6] &= 0b01111111;
		}
	}
	else
	{
           	if (value)
		{
               		display[digit][7] |= 0b01000000;
		}
           	else
		{
               		display[digit][7] &= 0b10111111;
		}
	}
}

void updateDisplay()
{
	int i;

	for (i=0;i<MATRIX_ROWS;i++)
	{
		i2cWriteByteData(controller1Handle,CMD_MATRIX2+i,display[0][i]);	
		i2cWriteByteData(controller1Handle,CMD_MATRIX1+i,display[1][i]);	
	}
	i2cWriteByteData(controller1Handle,CMD_UPDATE,0);
	for (i=0;i<MATRIX_ROWS;i++)
	{
		i2cWriteByteData(controller2Handle,CMD_MATRIX2+i,display[2][i]);	
		i2cWriteByteData(controller2Handle,CMD_MATRIX1+i,display[3][i]);	
	}
	i2cWriteByteData(controller2Handle,CMD_UPDATE,0);
	for (i=0;i<MATRIX_ROWS;i++)
	{
		i2cWriteByteData(controller3Handle,CMD_MATRIX2+i,display[4][i]);	
		i2cWriteByteData(controller3Handle,CMD_MATRIX1+i,display[5][i]);	
	}
	i2cWriteByteData(controller3Handle,CMD_UPDATE,0);
}

void microDotDisplayShutDown()
{
}
