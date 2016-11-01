#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "microDotDisplay.h"

int main(int argc, char *argv)
{
	time_t rawtime;
	struct tm *info;
	int old_second=-99;
	char szTime[10];

	microDotDisplayInit();
	do
	{
		time(&rawtime);
		info=localtime(&rawtime);
		if (info->tm_sec!=old_second)
		{
			sprintf(szTime,"%2.2d%2.2d%2.2d",
			info->tm_hour,info->tm_min,info->tm_sec);
			scrollUpString(szTime);
			if ( info->tm_sec % 2)
			{
				setDecimalPoint(2,1);
				setDecimalPoint(4,1);
				updateDisplay();
			}
			old_second=info->tm_sec;
		}
		usleep(10000);
	}
	while(1);
	microDotDisplayShutDown();
}

/*
int main(int argc, char *argv)
{
	int x,y,digit;
	unsigned char c;

	microDotDisplayInit();

	displayString("Hello");
	updateDisplay();
	sleep(10);
	displayString("1.25");
	updateDisplay();
	sleep(10);

	for(digit=0;digit<DIGITS;digit++)
	{
		for (y=0;y<FONT_ROWS;y++)
		{
			for (x=0;x<FONT_COLS;x++)
			{
				setPixel(digit,x,y,1);
				updateDisplay();
			}
		}
	}
	for(digit=0;digit<DIGITS;digit++)
	{
		for (y=0;y<FONT_ROWS;y++)
		{
			for (x=0;x<FONT_COLS;x++)
			{
				setPixel(digit,x,y,0);
				updateDisplay();
			}
		}
	}
	clearDisplay();
	updateDisplay();
}
*/
