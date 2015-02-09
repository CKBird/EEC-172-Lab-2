//Standard Includes
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/rom.h"
#include "glcdfont.c" 
//Extra Includes for OLED
#include "Adafruit_GFX.h"

//# of bites to send and rec
#define NUM_SSI_DATA            3

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

float p = 3.14159;
//#define WIDTH 128
//#define HEIGHT 128


void InitConsole(void)// Same as ConfigureUART()
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //UART Pin A
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    ROM_GPIOPinConfigure(GPIO_PA0_U0RX); //Pin MUX (In/Out)
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    UARTStdioConfig(0, 115200, 16000000);
}

void ConfigureSSI (void) {
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	//ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //Already done in InitConsole()

	//Configure muxs to enable special pin functions
	GPIOPinConfigure(GPIO_PA2_SSI0CLK);
	GPIOPinConfigure(GPIO_PA3_SSI0FSS);
	GPIOPinConfigure(GPIO_PA4_SSI0RX);
	GPIOPinConfigure(GPIO_PA5_SSI0TX);

	//Configure pins for SSI
	ROM_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2);

	//Set to master mode (SPI), 8 bit data
	ROM_SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8);

	//Enable SSI
	ROM_SSIEnable(SSI0_BASE);

}

void fillpixelbypixel(uint16_t color) 
{
  for (uint8_t x=0; x < width(); x++) 
  	{
    for (uint8_t y=0; y < height(); y++) 
    	{
		drawPixel(x, y, color);
		}
	}
  ROM_SysCtlDelay(SysCtlClockGet()/30); //delay(100);
}


void testlines(uint16_t color) {
   fillScreen(BLACK);
   for (uint16_t x=0; x < width()-1; x+=6) {
     drawLine(0, 0, x, height()-1, color);
   }
   for (uint16_t y=0; y < height()-1; y+=6) {
     drawLine(0, 0, width()-1, y, color);
   }
   
   fillScreen(BLACK);
   for (uint16_t x=0; x < width()-1; x+=6) {
     drawLine(width()-1, 0, x, height()-1, color);
   }
   for (uint16_t y=0; y < height()-1; y+=6) {
     drawLine(width()-1, 0, 0, y, color);
   }
   
   fillScreen(BLACK);
   for (uint16_t x=0; x < width()-1; x+=6) {
     drawLine(0, height()-1, x, 0, color);
   }
   for (uint16_t y=0; y < height()-1; y+=6) {
     drawLine(0, height()-1, width()-1, y, color);
   }

   fillScreen(BLACK);
   for (uint16_t x=0; x < width()-1; x+=6) {
     drawLine(width()-1, height()-1, x, 0, color);
   }
   for (uint16_t y=0; y < height()-1; y+=6) {
     drawLine(width()-1, height()-1, 0, y, color);
   }
   
}

void testdrawtext(char *text, uint16_t color) {
  setCursor(0,0);
  setTextColor(color);
  UARTprintf(text);

  for(int i = 0; i < 100; i++)
    drawChar(2,2,*text, YELLOW, BLACK, 1);

}

void testfastlines(uint16_t color1, uint16_t color2) {
   fillScreen(BLACK);
   for (uint16_t y=0; y < height()-1; y+=5) {
     drawFastHLine(0, y, width()-1, color1);
   }
   for (uint16_t x=0; x < width()-1; x+=5) {
     drawFastVLine(x, 0, height()-1, color2);
   }
}

void testdrawrects(uint16_t color) {
 fillScreen(BLACK);
 for (uint16_t x=0; x < height()-1; x+=6) {
   drawRect((width()-1)/2 -x/2, (height()-1)/2 -x/2 , x, x, color);
 }
}

void testfillrects(uint16_t color1, uint16_t color2) {
 fillScreen(BLACK);
 for (uint16_t x=height()-1; x > 6; x-=6) {
   fillRect((width()-1)/2 -x/2, (height()-1)/2 -x/2 , x, x, color1);
   drawRect((width()-1)/2 -x/2, (height()-1)/2 -x/2 , x, x, color2);
 }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=radius; x < width()-1; x+=radius*2) {
    for (uint8_t y=radius; y < height()-1; y+=radius*2) {
      fillCircle(x, y, radius, color);
    }
  }  
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=0; x < width()-1+radius; x+=radius*2) {
    for (uint8_t y=0; y < height()-1+radius; y+=radius*2) {
      drawCircle(x, y, radius, color);
    }
  }  
}

void testtriangles() {
  fillScreen(BLACK);
  int color = 0xF800;
  int t;
  int w = width()/2;
  int x = height();
  int y = 0;
  int z = width();
  for(t = 0 ; t <= 15; t+=1) {
    drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  fillScreen(BLACK);
  int color = 100;
  
  int x = 0;
  int y = 0;
  int w = width();
  int h = height();
  for(int i = 0 ; i <= 24; i++) {
    drawRoundRect(x, y, w, h, 5, color);
    x+=2;
    y+=3;
    w-=4;
    h-=6;
    color+=1100;
    UARTprintf("%d", i);
  }
}

//Own function for first two checkoffs


void tftPrintTest() {
  /*//Print out all characters in font[]
  for (int i=0; i<1275; i++) {
    drawChar(0, 5, font[i], WHITE, WHITE, 1);
  }*/
  
  //Print out Hello World!
  char* hw = "Hello world!";
  for (int i=0; i<strlen(hw); i++) {
    write(58);
    //drawChar(0, 5, hw[i], WHITE, WHITE, 1);
  }

  /*fillScreen(BLACK);
  setCursor(0, 5);
  setTextColor(RED);  
  setTextSize(1);
  UARTprintf("Hello World!");
  setTextColor(YELLOW);
  setTextSize(2);
  println("Hello World!");
  setTextColor(BLUE);
  setTextSize(3);
  print(1234.567);
  ROM_SysCtlDelay(SysCtlClockGet()/2); //delay(1500);
  setCursor(0, 5);
  fillScreen(BLACK);
  setTextColor(WHITE);
  setTextSize(0);
  println("Hello World!");
  setTextSize(1);
  setTextColor(GREEN);
  print(p, 6);
  println(" Want pi?");
  println(" ");
  println("845FED"); // print 8,675,309 out in HEX!
  println(" Print HEX!");
  println(" ");
  setTextColor(WHITE);
  println("Sketch has been");
  println("running for: ");
  setTextColor(MAGENTA);
  //print(millis() / 1000);
  setTextColor(WHITE);
  print(" seconds.");
*/}

void mediabuttons() {
 // play
  fillScreen(BLACK);
  fillRoundRect(25, 10, 78, 60, 8, WHITE);
  fillTriangle(42, 20, 42, 60, 90, 40, RED);
  ROM_SysCtlDelay(SysCtlClockGet()/6); //delay(500);
  // pause
  fillRoundRect(25, 90, 78, 60, 8, WHITE);
  fillRoundRect(39, 98, 20, 45, 5, GREEN);
  fillRoundRect(69, 98, 20, 45, 5, GREEN);
  ROM_SysCtlDelay(SysCtlClockGet()/6); //delay(500);
  // play color
  fillTriangle(42, 20, 42, 60, 90, 40, BLUE);
  ROM_SysCtlDelay(SysCtlClockGet()/60); //delay(50);
  // pause color
  fillRoundRect(39, 98, 20, 45, 5, RED);
  fillRoundRect(69, 98, 20, 45, 5, RED);
  // play color
  fillTriangle(42, 20, 42, 60, 90, 40, GREEN);
}

/**************************************************************************/
/*! 
    @brief  Renders a simple test pattern on the LCD
*/
/**************************************************************************/
void lcdTestPattern(void)
{
  uint32_t i,j;
  goTo(0, 0);
  
  for(i=0;i<128;i++)
  {
    for(j=0;j<128;j++)
    {
      if(i<16){
        writeData(RED>>8); writeData(RED);
      }
      else if(i<32) {
        writeData(YELLOW>>8);writeData(YELLOW);
      }
      else if(i<48){writeData(GREEN>>8);writeData(GREEN);}
      else if(i<64){writeData(CYAN>>8);writeData(CYAN);}
      else if(i<80){writeData(BLUE>>8);writeData(BLUE);}
      else if(i<96){writeData(MAGENTA>>8);writeData(MAGENTA);}
      else if(i<112){writeData(BLACK>>8);writeData(BLACK);}
      else {
        writeData(WHITE>>8);      
        writeData(WHITE);
       }
    }
  }
}

void setup(void) {
  UARTprintf("hello!\n");
  begin();

  UARTprintf("init\n");

  // You can optionally rotate the display by running the line below.
  // Note that a value of 0 means no rotation, 1 means 90 clockwise,
  // 2 means 180 degrees clockwise, and 3 means 270 degrees clockwise.
  //setRotation(1);
  // NOTE: The test pattern at the start will NOT be rotated!  The code
  // for rendering the test pattern talks directly to the display and
  // ignores any rotation.

  //uint16_t time = 111;
  fillRect(0, 0, 128, 128, BLACK);
  //time = millis() - time;
  
  //UARTprintf(time, %d);
  ROM_SysCtlDelay(SysCtlClockGet()/6); //delay(500);
  
  lcdTestPattern();
  ROM_SysCtlDelay(SysCtlClockGet()/6); //delay(500);
  
  invert(true);
  ROM_SysCtlDelay(SysCtlClockGet()/30); //delay(100);
  invert(false);
  ROM_SysCtlDelay(SysCtlClockGet()/30); //delay(100);

  UARTprintf("Test 0");

  fillScreen(BLACK);
  testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", WHITE);
  ROM_SysCtlDelay(SysCtlClockGet()/6); //delay(500);

  UARTprintf("Test 1");
  // tft print function!
  tftPrintTest();
  ROM_SysCtlDelay(SysCtlClockGet()/6); //delay(500);
  
  UARTprintf("Test 2");
  //a single pixel
  drawPixel(width()/2, height()/2, GREEN);
  ROM_SysCtlDelay(SysCtlClockGet()/6); //delay(500);
UARTprintf("Test 3");

  // line draw test
  testlines(YELLOW);
  ROM_SysCtlDelay(SysCtlClockGet()/6); //delay(500);    
 
 UARTprintf("Test 4");
  // optimized lines
  testfastlines(RED, BLUE);
  ROM_SysCtlDelay(SysCtlClockGet()/6); //delay(500);    

UARTprintf("Test 5");
  testdrawrects(GREEN);
  ROM_SysCtlDelay(SysCtlClockGet()/3); //delay(1000);

UARTprintf("Test 6");
  testfillrects(YELLOW, MAGENTA);
  ROM_SysCtlDelay(SysCtlClockGet()/3); //delay(1000);

UARTprintf("Test 7");
  fillScreen(BLACK);
  testfillcircles(10, BLUE);
  testdrawcircles(10, WHITE);
  ROM_SysCtlDelay(SysCtlClockGet()/3); //delay(1000);
   
   UARTprintf("Test 8");
  testroundrects();
  ROM_SysCtlDelay(SysCtlClockGet()/6); //delay(500);
  
  UARTprintf("Test 9");
  testtriangles();
  ROM_SysCtlDelay(SysCtlClockGet()/6); //delay(500);
  
  UARTprintf("Test 10");
  UARTprintf("done\n");
  ROM_SysCtlDelay(SysCtlClockGet()/3); //delay(1000);
}

void loop() {
}

int main (void) {
	uint32_t pui32DataTx[NUM_SSI_DATA];
	uint32_t pui32DataRx[NUM_SSI_DATA];
	uint32_t ui32Index;

	ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
 	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); 				//B
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);        	//pinMode(_rst, OUTPUT);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6);

	InitConsole();
	ConfigureSSI();
	begin();
	//float p = 3.1415926;
	while(1)
	{
		//if(ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_5))
			//begin();
    UARTprintf("Turning on now\n");
		setup();
	}
}
