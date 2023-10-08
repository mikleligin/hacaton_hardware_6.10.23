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
  tft.setTextColor(YELLOW);
  tft.println(utf8rus("Время 3:42"));
  tft.println();
  tft.println();
  tft.println();
  tft.println();
  tft.println();
  tft.setTextSize(4);
  tft.setTextColor(GREEN);
  tft.println("$chmod -er -s");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.println(utf8rus("       Загрузка..."));
  tft.setCursor(0, 223);
  tft.setTextSize(1);
}

bool evenflag = 1;

int myvar = 0;
int myvarh = 0;

int flag = 0;
int dot = 0;

char c;

String ParsedBySkrm = "";
int hflag = 0;
String half1 = "";
String half2 = "";
//tft.fillScreen(BLACK);


void loop(void) {

  String accel;
  if (esp8266.available()) {
    dot++;
    if(dot%350==0)
    {

      //tft.print(utf8rus("Математика"));
      // tft.write('к');
      // tft.write('с');
      // tft.write('т');
      String nigga = "";
      //nigga += 'т';
      //tft.print(nigga+'т');
      
    
        
        // for (int i = 192; i < 1000; i++)
        // {
        //   if(1)
        //   {
        //   tft.print(char(i));
        //   }
        // }
      // //tft.println();
      tft.print(".");
      Serial.println('.');
      
    }
    //Для теста ветки
    c = esp8266.read();
    if(c == '~')
    {
      flag = 1;
    }
    if(flag)
    {
      Serial.write(c); // Пересылать данные с ESP8266 в монитор порта Arduino
      ParsedBySkrm+=c;
    
    if(c == '#')
    {
      //Serial.write('\n');
      hflag = 1;
      if((c == '#') && (hflag))
      {
        hflag = 0;
        if(ParsedBySkrm.indexOf("~1")>-1 && ParsedBySkrm.indexOf("~2")>-1 && ParsedBySkrm.indexOf("~3")>-1 && ParsedBySkrm.indexOf("~4")>-1)// && ParsedBySkrm.indexOf("~5")>-1 )//&& ParsedBySkrm.indexOf("~6")>-1)
        {
          //String half1;
          tft.setTextSize(2);
          tft.fillScreen(BLACK);
          tft.setCursor(0, 0);
          tft.setTextColor(YELLOW);
          tft.println(utf8rus("Время 3:42"));
          tft.setTextColor(WHITE);
          for(int i = 0; i < ParsedBySkrm.length()/2;i++ )
          {

            if(ParsedBySkrm[i]=='#')
            {
              half1+='\n';
              half1+='\n';
              continue;
            }
            half1+=ParsedBySkrm[i];
          }
          // Serial.println(ParsedBySkrm);
          // tft.print(utf8rus(ParsedBySkrm));
          Serial.println(half1);
          tft.print(utf8rus(half1));
          for(int i =  ParsedBySkrm.length()/2; i < ParsedBySkrm.length();i++ )
          {

            if(ParsedBySkrm[i]=='#')
            {
              half2+='\n';
              half2+='\n';
              continue;
            }
            half2+=ParsedBySkrm[i];
          }
          tft.print(utf8rus(half2));
          ParsedBySkrm = "";
        }
      }
    }
    if(c == '\n')
    {
      flag = 0;
    }
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