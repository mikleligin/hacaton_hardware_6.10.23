#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <ArduinoJson.h>
#include <WiFiClientSecureBearSSL.h>
const char* ssid = "iPhone monke";
const char* password = "bbbbbbbb"; 
const char* host = "www.ictis.ru"; // Адрес вашего сервера
const int httpsPort = 443; // HTTPS порт (стандартный порт 443)
const String url = "/67.html/7"; // Путь к ресурсу, который вы хотите запросить
const size_t bufferSize = 2*JSON_OBJECT_SIZE(5); // Здесь указывается ожидаемый размер буфера JSON
String line = "";
StaticJsonDocument<bufferSize> jsonDoc;
WiFiClient nga = WiFiClient();
BearSSL::WiFiClientSecure client;
int cntt = 0; 
void setup() {
  Serial.begin(9600); 
  StaticJsonDocument<200> doc;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Подключение к Wi-Fi...");
  }

  Serial.println("Подключено к Wi-Fi!");
  client.setInsecure();
}
int flag = 0;
String couples[8];
int indexx[8];
void loop()
{
  if (WiFi.status() == WL_CONNECTED && !flag) //Check WiFi connection status
  {
    
   

  if (!client.connect(host, 443)) {
    Serial.println(client.connect(host, 443));
    Serial.println("Ошибка подключения");
    return;
  }

  // Отправляем HTTP GET запрос
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Отправлен запрос");

  // Чтение и вывод ответа сервера
  while (client.connected() || client.available()) {
    if (client.available()) {

      line += client.readStringUntil('\n');
      line+='\n';
    } 
  }
  //Serial.println();
  client.stop();
  flag = 1;
 // Serial.println(line);
  //Serial.println(line.indexOf("current-day"));
  int index_data = line.indexOf("Пнд,09  октября");
  String temp_line;
  for(int i = index_data; i < line.indexOf("Втр,10") ; i++)
  {
    temp_line += line[i];
  }
  // Serial.println();
  // Serial.println();
  // Serial.println();
  //Serial.println();
  //Serial.println(temp_line);
  //Serial.println(temp_line);
  for(int i = 0; i < 7 ; i++)
  {
    indexx[i] = temp_line.indexOf("para-"+String(i+1))+8;
  }
  for(int i = 0; i < 7 ; i++)
  {
    
    int ind = temp_line.indexOf("<td class='para-"+String(i+1)+"'><span class="+"\"table-ext-icon\" "+ "title="+"\"Дистанционная пара\""+">📺")+103;
    //Serial.println(ind);
    if(ind-102>0)indexx[i] = ind;
  }
  for(int j = 0; j < 7 ; j++)
  {
    Serial.print('`');
    for(int i = indexx[j]; temp_line[i] !='<' ; i++)
    {
      Serial.print(temp_line[i]);
    }
    Serial.print('#');
  }
  delay(60000); // Подождите 60 секунд перед повторным запросом
}
}


