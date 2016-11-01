#define DIGITS 6

#define DIGIT_ADDR1 0x63
#define DIGIT_ADDR2 0x62
#define DIGIT_ADDR3 0x61

#define CONFIG		0b00011000 /* NORMAL MATRIX 1+2 NO AUDIO 8x8 */
#define LIGHTING	0b00001110

#define CMD_CONFIG	0x00
#define CMD_LIGHTING	0x0d
#define CMD_PWM		0x19
#define CMD_RESET	0xFF
#define CMD_MATRIX1	0x01
#define CMD_MATRIX2	0x0e
#define CMD_UPDATE	0x0c

#define MATRIX_ROWS	8
#define MATRIX_COLS	8

#define FONT_ROWS	7
#define FONT_COLS	5

#define OFF		0
#define VERY_DIM	2	
#define DIM		20
#define BRIGHT		127

void microDotDisplayInit();
void clearDigit(int);
void clearDisplay();
void setPixel(int,int,int,int);
void setDigit(int,char);
void setScrollDigit(int,char,char,int);
void displayString(char *);
void scrollUpString(char *);
void setDecimalPoint(int,int);
void updateDisplay();
void microDotDisplayShutDown();


