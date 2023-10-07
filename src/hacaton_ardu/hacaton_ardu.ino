#include <SoftwareSerial.h>
SoftwareSerial esp8266(10, 11); // RX, TX
void setup() {
  Serial.begin(9600); // Настройте скорость обмена данными для монитора порта Arduino
  esp8266.begin(9600); // Настройте скорость обмена данными для ESP8266
}
int flag = 0;
int 
void loop() {
  String accel;
  if (esp8266.available()) {
    char c = esp8266.read();
    if(c == '!')
    {
      flag = 1;
      continue;
    }
    if(c == '#')
    {
      Serial.write('\n');
      continue;
    }
    if(c != '\n' && flag)
    {
      Serial.write(c); // Пересылать данные с ESP8266 в монитор порта Arduino
    }
    if(c == '#')
    {
      Serial.write('\n');
    }
    if(c == '\n')
    {
      flag = 0;
    }
    
  }
  if (Serial.available()) {
    char c = Serial.read();
    esp8266.write(c); // Пересылать данные с монитора порта Arduino в ESP8266
  }
}
