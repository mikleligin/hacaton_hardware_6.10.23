#include "stm32f4xx.h"
#include "math.h"
#include "aboba.h"
const int Width = 128; // Глобальная переменная с шириной экрана
const int Height = 64; // Глобальная переменная с высотой экрана
char Buffer[Width * Height / 8]; // Буфер данных экрана
int cnt = 0;
int wght = 0;
int freq[30] = {200,200,400,200,200,385, 200, 200, 360, 200, 200, 345,200,200,305, 315,200,200,400,200,200,385, 200, 200, 360, 200, 200, 315,315};
int szz = 32;
void I2C_Write(int Address, char Reg, char* Data, int Size) // регистр и данные
{
 I2C2->CR1 |= I2C_CR1_START; // Занимаем линию связи для данных
 while (!(I2C2->SR1 & I2C_SR1_SB)) { } // Ждём занятия линии
 I2C2->DR = (Address<<1) & ~I2C_OAR1_ADD0; // Шлём адрес для передачи данных
 while (!(I2C2->SR1 & I2C_SR1_ADDR)) { } // Ждём успешной связи с устройством
 I2C2->SR2; // Читаем SR2 для его отчистки
 while (!(I2C2->SR1 & I2C_SR1_TXE)) { } // Ждем готовности передать байт
 I2C2->DR = Reg; // Передаём байт регистра
 while (Size--) // Цикл передачи байт
 {
 while (!(I2C2->SR1 & I2C_SR1_TXE)) { } // Ждем готовности передать байт
 I2C2->DR = *Data++; // Передаём байт данных
 }
 while (!(I2C2->SR1 & I2C_SR1_BTF)) { } // Ждём окончания передачи данных
 I2C2->CR1 |= I2C_CR1_STOP; // Освобождаем линию связи
 while (I2C2->CR1 & I2C_CR1_STOP) { } // Ждём освобождения линии
}


const int Addr = 0x3C; // Глобальная переменная с адресом устройства
void Command(char Value) { I2C_Write(Addr, 0x00, &Value, 1); } // Запись в 0x00 регистр

void SetAltFunc(GPIO_TypeDef* Port, int Channel, int AF)
{
  Port->MODER &= ~(3<<(2*Channel));
  Port->MODER |= 2<<(2*Channel); 
  if(Channel<8) 
  {
    Port->AFR[0] &= ~(15<<4*Channel); 
    Port->AFR[0] |= AF<<(4*Channel); 
  }
  else
  {
    Port->AFR[1] &= ~(15<<4*(Channel-8)); 
    Port->AFR[1] |= AF<<(4*(Channel-8)); 
  }
}

int AnalogRead(int N) // Функция принимает номер канала для преобразования
{
   ADC1->SQR3 = N; // Выбран полученный из аргумента канал
   for(int a=0; a<20; a++) { asm("NOP"); } // Ожидать больше 20 тактов
   ADC1->CR2 |= ADC_CR2_SWSTART; // Начать преобразование
   while(!(ADC1->SR & ADC_SR_EOC)) { asm("NOP"); } // Ждать установки бита конца операции
   return ADC1->DR; // Вернуть результат преобразования
}

void Clear() // Функция очистки буфера экрана
  {
    for(int a=0; a<sizeof(Buffer); a++) Buffer[a]=0;
  }
void DrawPixel(int X, int Y) // Функция установки пикселя
  {
     if ( X >= Width || Y >= Height ) return;
     Buffer[X + (Y / 8) * Width] |= 1 << (Y % 8);
  }
void UpdateScreen() // Функция прорисовки буфера экрана на дисплей
{
   for (char a = 0; a < 8; a++)
   {
     Command(0xB0 + a);
     Command(0x00);
     Command(0x10);
   I2C_Write(Addr, 0x40, &Buffer[Width * a], Width);
   }
}
void ShowImage(char image[][4097]){
  for(int i = 0; i < 12; i++){
    Clear(); // Очистим буфер экрана
    while(cnt != 4096) 
    {
      
      
      if(cnt%64 == 0){wght++;}
      if(image[i][cnt]!='0')
      {
        DrawPixel(32+cnt%64, wght%64);
      }
      
      cnt++;
      
      //if(cnt>=4096){cnt = 0;}
      //if(wght>=64){wght = 0;}
    }
      
/*for(int a=0; a<750000; a++) asm("NOP");
      TIM5->PSC = (84000000/100/freq[i])-1; // Установим частоту 200Гц
      for(int a=0; a<750000; a++) asm("NOP"); // Прослушаем её примерно 1 секунду
      szz++;
      if(szz>=1024){szz = 0;}
*/      
      UpdateScreen();
      if(cnt>=4096){cnt = 0;}
      if(wght>=64){wght = 0;}
  }
}

int main()
{
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
  
  GPIOC->MODER &= ~GPIO_MODER_MODER3;
  GPIOC->MODER |= GPIO_MODER_MODER3;
  ADC1->CR2 = ADC_CR2_ADON;
  ADC1->SQR3 = 13;
  for(int a=0; a<150; a++) { }
  //     
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; // Порт E задействован
  GPIOE->MODER &= ~GPIO_MODER_MODER7; // Сброс режима для PE7
  GPIOE->MODER |= GPIO_MODER_MODER7_0; // Установка режима на выход PE7
  GPIOE->BSRRL = 1<<7; // Установить значение HIGH (3.3V) для PE7

  RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; // I2C 3 задействован
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; // Порт H задействован
  GPIOH->OTYPER |= GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5; // Открытый сток для PH7 и PH8
  GPIOH->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4_0 | GPIO_OSPEEDER_OSPEEDR4_1; // Мах. скорость PH7 
  GPIOH->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_0 | GPIO_OSPEEDER_OSPEEDR5_1; // Мах. скорость PH8
  GPIOH->PUPDR |= GPIO_PUPDR_PUPDR4_0 | GPIO_PUPDR_PUPDR5_0; // Подтяжка 3.3V для PH7 и PH8
  SetAltFunc(GPIOH, 4, 4); // Установка альт. режима AF4 для SCL(PH7) (см. лаб. 2)
  SetAltFunc(GPIOH, 5, 4); // Установка альт. режима AF4 для SDA(PH8) (см. лаб. 2)
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; // Таймер 5 задействован (APB1 x2 = 84МГц)
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN; // Порт I задействован
  SetAltFunc(GPIOI, 0, 2); // Установка альт. Режима AF2 для TIM5 CH4 (PI0) (см. лаб. 2)
  
  TIM5->CR2 = TIM_CR2_MMS_0 | TIM_CR2_MMS_1; // Генерация пульса сравнения
  TIM5->ARR = 100 - 1; // Диапазон сравнения 100 (громкость)
  //TIM5->PSC = (84000000/100/1)-1; // Начальная частота 2000Гц
  TIM5->CCMR2 = TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE; // Выбран режим PWM 1
  TIM5->CCER = TIM_CCER_CC4E; // Выход на 4 канале активен
  TIM5->CR1 = TIM_CR1_CEN; // Таймер запущен
  TIM5->CCR4 = 1;

  
  I2C2->CR2 = (I2C_CR2_FREQ & 0x2A);
  I2C2->CCR = I2C_CCR_FS | (I2C_CCR_CCR & 0x006C);
  I2C2->TRISE = (I2C_TRISE_TRISE & 0x14);
  
  I2C2->CR1 = I2C_CR1_PE;
 
  while(I2C2->SR2 & I2C_SR2_BUSY) { }
  
  for(int a=0; a<10000000; a++) asm volatile("nop"); // Подождать готовности устройства
  Command(0xAE); // Display off
  Command(0x20); // Set Memory Addressing Mode 
  Command(0x10); // Page Addressing Mode (RESET)
  Command(0xB0); // Set Page Start Address for Page Addressing Mode,0-7
  Command(0xC8); // Set COM Output Scan Direction
  Command(0x00); //---set low column address
  Command(0x10); //---set high column address
  Command(0x40); //--set start line address
  Command(0x81); //--set contrast control register
  Command(0xFF); // Orientation
  Command(0xA1); //--set segment re-map (0 to 127)
  Command(0xA6); //--set normal display
  Command(0xA8); //--set multiplex ratio (1 to 64)
  Command(0x3F); //
  Command(0xA4); // Output follows RAM content
  Command(0xD3); //-set display offset
  Command(0x00); //-not offset
  Command(0xD5); //--set display clock divide ratio/oscillator frequency
  Command(0xF0); //--set divide ratio
  Command(0xD9); //--set pre-charge period
  Command(0x22); //
  Command(0xDA); //--set com pins hardware configuration
  Command(0x12); //
  Command(0xDB); //--set vcomh
  Command(0x20); // 0.77 * Vcc
  Command(0x8D); //--set DC-DC enable
  Command(0x14); //
  Command(0xAF); //--turn on SSD1309 panel
  
  
  
  

  while(1)
  {
      
    
    DrawPixel(0, 0); // Добавим левую верхнюю точку
    DrawPixel(127, 0); // Добавим правую верхнюю точку
    DrawPixel(127, 63); // Добавим правую нижнюю точку
    DrawPixel(0, 63); // Добавим левую нижнюю точку
    for(int i=0; i<30; i++) {
      
    for(int a=0; a<650000; a++) asm("NOP");
      TIM5->PSC = (84000000/100/freq[i])-1; // Установим частоту 200Гц
      for(int a=0; a<650000; a++) asm("NOP"); // Прослушаем её примерно 1 секунду
      szz++;
    }
    ShowImage(p1);
    free(p1);
    ShowImage(p2);
    free(p2);

  }
  return 0;
}
