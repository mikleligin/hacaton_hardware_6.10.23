#include <SoftwareSerial.h>
SoftwareSerial esp8266(10, 11); // RX, TX

#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
//#include <Adafruit_ILI9341.h>

#include "string.h"

String utf8rus();

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
void setup(void) {
  Serial.begin(9600);
  progmemPrintln(PSTR("TFT LCD test"));
#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  progmemPrintln(PSTR("Using Adafruit 2.8\" TFT Arduino Shield Pinout"));
#else
  progmemPrintln(PSTR("Using Adafruit 2.8\" TFT Breakout Board Pinout"));
#endif
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(3); // Need for the Mega, please changed for your choice or rotation initial
  progmemPrintln(PSTR("Benchmark                Time (microseconds)"));

  esp8266.begin(9600);
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.cp437(true);
  tft.setTextSize(2);
}

bool evenflag = 1;

int myvar = 0;
int myvarh = 0;

int flag = 0;
int dot = 0;

char c;

String ParsedBySkrm = "";
int hflag = 0;

//tft.fillScreen(BLACK);


void loop(void) {

  String accel;
  if (esp8266.available()) {
    dot++;
    if(dot%250==0)
    {
      //tft.print(utf8rus("Математика"));
      // tft.write('к');
      // tft.write('с');
      // tft.write('т');
      
      
    
        
        // for (int i = 192; i < 1000; i++)
        // {
        //   if(1)
        //   {
        //   tft.print(char(i));
        //   }
        // }
      // //tft.println();
      Serial.println('.');
      
    }
    //Для теста ветки
    c = esp8266.read();
    if(c == '~')
    {
      flag = 1;
    }
    if(c == '#')
    {
      Serial.write('\n');
    }
    if(flag)
    {
      if(hflag == 1)
      {

        //tft.setCursor(myvar, myvarh);
        //tft.print(utf8rus(String(char(replace_letter(toupper(c))))));
        //tft.print(String(c));
      }
      Serial.write(c); // Пересылать данные с ESP8266 в монитор порта Arduino
      ParsedBySkrm+=c;
    }
    if(c == '#')
    {
      Serial.write('\n');
      hflag = 1;
      if((c == '#') && (hflag))
      {
        hflag = 0;
        Serial.println(ParsedBySkrm);
        tft.print(utf8rus(ParsedBySkrm));
      }
    }
    if(c == '\n')
    {
      flag = 0;
    }
    
  }

}

void progmemPrint(const char *str) {
  char c;
  while(c = pgm_read_byte(str++)) Serial.print(c);
}

// Same as above, with trailing newline
void progmemPrintln(const char *str) {
  progmemPrint(str);
  Serial.println();
}


String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}