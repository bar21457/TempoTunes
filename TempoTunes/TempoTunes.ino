
#include <stdint.h>
#include <ili9341_SPI.h>

#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "font.h"
#include "lcd_registers.h"

//**********************************************************************************
// PROTOTIPOS DE FUNCIONES
//**********************************************************************************


void Timer0IntHandler();
void Timer1IntHandler();
void TimerInit();
int Ascii2Hex(int a);
void Tempo_SD(char doc[]);
void J1_SD(char doc[]);
void J2_SD(char doc[]);

//**********************************************************************************
// VARIABLES
//**********************************************************************************

// File de la SD
File myFile;

// Estado de las LEDs
uint8_t greenLedState = HIGH;
uint8_t redLedState = LOW;

// Variables para llevar control de las posiciones de las burbujas del juego
int posicion_YRojo1 = 10;
int posicion_YVerde1 = 10;
int posicion_YAzul1 = 10;
int posicion_YAmarillo1 = 10;
int posicion_YRojo2 = 10;
int posicion_YVerde2 = 10;
int posicion_YAzul2 = 10;
int posicion_YAmarillo2 = 10;
int posicion_YRojo3 = 10;
int posicion_YVerde3 = 10;
int posicion_YAzul3 = 10;
int posicion_YAmarillo3 = 10;
int posicion_YRojo4 = 10;
int posicion_YVerde4 = 10;
int posicion_YAzul4 = 10;
int posicion_YAmarillo4 = 10;
int posicion_YRojo12 = 10;
int posicion_YVerde12 = 10;
int posicion_YAzul12 = 10;
int posicion_YAmarillo12 = 10;
int posicion_YRojo22 = 10;
int posicion_YVerde22 = 10;
int posicion_YAzul22 = 10;
int posicion_YAmarillo22 = 10;
int posicion_YRojo32 = 10;
int posicion_YVerde32 = 10;
int posicion_YAzul32 = 10;
int posicion_YAmarillo32 = 10;
int posicion_YRojo42 = 10;
int posicion_YVerde42 = 10;
int posicion_YAzul42 = 10;
int posicion_YAmarillo42 = 10;

//Variables de las burbujas en pantalla del juego
int rojo1 = 0;
int verde1 = 0;
int amarillo1 = 0;
int azul1 = 0;
int rojo2 = 0;
int verde2 = 0;
int amarillo2 = 0;
int azul2 = 0;
int rojo3 = 0;
int verde3 = 0;
int amarillo3 = 0;
int azul3 = 0;
int rojo4 = 0;
int verde4 = 0;
int amarillo4 = 0;
int azul4 = 0;
int rojo12 = 0;
int verde12 = 0;
int amarillo12 = 0;
int azul12 = 0;
int rojo22 = 0;
int verde22 = 0;
int amarillo22 = 0;
int azul22 = 0;
int rojo32 = 0;
int verde32 = 0;
int amarillo32 = 0;
int azul32 = 0;
int rojo42 = 0;
int verde42 = 0;
int amarillo42 = 0;
int azul42 = 0;
int contadorRojo = 0;
int contadorVerde = 0;
int contadorAzul = 0;
int contadorAmarillo = 0;

// Variables de bitmpas provenientes de la memoria flash
extern uint8_t tempo[];
extern uint8_t tunes[];
extern uint8_t pressB1[];
extern uint8_t intrfz[];
extern uint8_t kirby_standing1[];
extern uint8_t kirby_standing2[];
extern uint8_t kirby_up1[];
extern uint8_t kirby_up2[];
extern uint8_t kirby_down1[];
extern uint8_t kirby_down2[];
extern uint8_t kirby_sides1[];
extern uint8_t kirby_sides2[];

// Variables varias
int win = 0;
int nivel1 [] = {2,1,3,4,4,2,3};
int nota = 0;
int kirby;
int flag = 1;
int puntosj1 = 0;
int puntosj2 = 0;
int kirby1 = 0;
int kirby2 = 0;

byte x = 1;
byte c;
byte z;

void setup() {

  Serial.begin(9600);                         //Inicia la comunicación serial a 9600 baudios
  Serial.println("Iniciando pantalla...");
  
  pinMode(PA_3, OUTPUT);
  pinMode(PB_6, OUTPUT);
  
  SPI.setModule(0);                           //Inicia la comunicación SPI con el módulo 0
  SD.begin(PB_6);                             //Inicia la comunicación con la SD con el puerto PB_6
  Wire.begin();                               //Inicia la comunicación I2C
  
  LCD_Init();                                 //Inicialización de la pantalla TFT
  LCD_Clear(0x00);                            //Limpiamos la pantalla
  
  Tempo_SD("tempo.txt");                      //Se imprime "Tempo" desde la SD
  
  delay(1000);
  
  LCD_Bitmap(90, 110, 200, 70, tunes);        //Se imprime "Tunes" desde la memoria flash
  delay(1000);
  
  LCD_Bitmap(120, 200, 80, 24, pressB1);      //Se imprime "Press B1" desde la memoria flash
  while (flag == 1){
  
  Wire.requestFrom(0x54, 2);                  //Pedimos que se presione el botón 1 del ESP32 para continuar
  while(Wire.available())     
  { 
    c = Wire.read();                          //Se recibe un byte de carácteres
  }
  Serial.println(c);
    if (c == 97){
      flag = 0;                               //Si se presionó el botón 1, inicia el juego
    }
  }
  
  Wire.begin();                               //Inicia la comunicación I2C
  delay(1000);
  
  LCD_Clear(0x00);                            //Limpiamos la pantalla
  LCD_Bitmap(0, 0, 320, 240, intrfz);         //Se imprime la interfaz en la pantalla
  
  delay(1000);
  
  TimerInit();                                //Iniciamos el Timer
  
}

void loop() {
  
}

//**********************************************************************************
// FUNCIONES
//**********************************************************************************

void TimerInit(){                             //Función para inizializar el Timer
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
  MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
  
  Serial.println("hola");
  float period_0 = 2; // 200ms
  float period_1 = 0.05 ; // 200ms
  
  MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, MAP_SysCtlClockGet()*period_0); //(80MHZ / MAP_SysCtlClockGet)
  MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, MAP_SysCtlClockGet()*period_1); //5 veces por seg

  TimerIntRegister(TIMER0_BASE, TIMER_A, &Timer0IntHandler);
  TimerIntRegister(TIMER1_BASE, TIMER_A, &Timer1IntHandler);
  
  MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  MAP_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  
  MAP_TimerEnable(TIMER0_BASE, TIMER_A);
  MAP_TimerEnable(TIMER1_BASE, TIMER_A);
}

void Timer0IntHandler() {                   //Función que lleva control de las burbujas en pantalla, los puntajes, mueve los Kirbys bailando y se indica el ganador a través del Timer0
  MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  if (nivel1[nota] == 1){
    contadorRojo++;
    rojo1 = 1;
    
    if (contadorRojo == 4){
      contadorRojo = 0;
    }
    if (contadorRojo == 1){
      rojo1 = 1;
      rojo12 = 1;
    }
    if (contadorRojo == 2){
      rojo2 = 1;
      rojo22 = 1;
    }
    if (contadorRojo == 3){
      rojo3 = 1;
      rojo32 = 1;
    }
  }
  if (nivel1[nota] == 2){
    contadorVerde++;
    verde1 = 1;
    if (contadorVerde == 4){
      contadorVerde = 0;
    }
    if (contadorVerde == 1){
      verde1 = 1;
      verde12 = 1;
    }
    if (contadorVerde == 2){
      verde2 = 1;
      verde22 = 1;
    }
    if (contadorVerde == 3){
      verde3 = 1;
      verde32 = 1;
    }
  }
  if (nivel1[nota] == 3){
    contadorAzul++;
    azul1 = 1;
    if (contadorAzul == 4){
      contadorAzul = 0;
    }
    if (contadorAzul == 1){
      azul1 = 1;
      azul12 = 1;
    }
    if (contadorAzul == 2){
      azul2 = 1;
      azul22 = 1;
    }
    if (contadorAzul == 3){
      azul3 = 1;
      azul32 = 1;
    }

  }
  if (nivel1[nota] == 4){
    contadorAmarillo++;
    amarillo1 = 1;
    if (contadorAmarillo == 4){
      contadorAmarillo = 0;
    }
    if (contadorAmarillo == 1){
      amarillo1 = 1;
      amarillo12 = 1;
    }
    if (contadorAmarillo == 2){
      amarillo2 = 1;
      amarillo22 = 1;
    }
    if (contadorAmarillo == 3){
      amarillo3 = 1;
      amarillo32 = 1;
    }

  }
  nota++;
  
  if (kirby == 0){
  LCD_Sprite(64, 175, 40, 40, kirby_standing1, 1, 1, 0, 0);
  LCD_Sprite(218, 175, 40, 40, kirby_standing2, 1, 1, 0, 0);
  }    
  if (kirby == 1){         
  LCD_Sprite(64, 175, 40, 40, kirby_up1, 1, 1, 0, 0);
  LCD_Sprite(218, 175, 40, 40, kirby_up2, 1, 1, 0, 0);
  }
  if (kirby == 2){
  LCD_Sprite(64, 175, 40, 40, kirby_sides1, 1, 1, 0, 0);
  LCD_Sprite(218, 175, 40, 40, kirby_sides2, 1, 1, 0, 0);
  }
  if (kirby == 3){
  LCD_Sprite(64, 175, 40, 40, kirby_sides1, 1, 1, 1, 0);
  LCD_Sprite(218, 175, 40, 40, kirby_sides2, 1, 1, 1, 0);
  }
  if (kirby == 4) {
  LCD_Sprite(64, 175, 40, 40, kirby_down1, 1, 1, 0, 0);
  LCD_Sprite(218, 175, 40, 40, kirby_down2, 1, 1, 0, 0);
  }
  kirby++;
  
  if (kirby == 5) {
    kirby = 0;
  }
    if (nota > 7 ){
    win = 1;
    FillRect(0,0,320,240,0x0000);
    if (puntosj1 > puntosj2){
      J1_SD("J1.txt");
      kirby1 = 1;
    }
    if (puntosj2 > puntosj1){
      J2_SD("J2.txt");
      kirby2 = 1;
    }
    while (win == 1){  
    
      if (kirby1 == 1){
        J1_SD("J1.txt");
          LCD_Sprite(64, 175, 40, 40, kirby_standing1, 1, 1, 0, 0);
          delay(200);               
          LCD_Sprite(64, 175, 40, 40, kirby_up1, 1, 1, 0, 0);
          delay(200);
          LCD_Sprite(64, 175, 40, 40, kirby_sides1, 1, 1, 0, 0);
          delay(200);
          LCD_Sprite(64, 175, 40, 40, kirby_sides1, 1, 1, 1, 0);
          delay(200);
          LCD_Sprite(64, 175, 40, 40, kirby_down1, 1, 1, 0, 0);
          delay(200);
      
      }
    
      if (kirby2 == 1){
        J2_SD("J2.txt");
          LCD_Sprite(218, 175, 40, 40, kirby_standing2, 1, 1, 0, 0);
          delay(200);               
          LCD_Sprite(218, 175, 40, 40, kirby_up2, 1, 1, 0, 0);
          delay(200);
          LCD_Sprite(218, 175, 40, 40, kirby_sides2, 1, 1, 0, 0);
          delay(200);
          LCD_Sprite(218, 175, 40, 40, kirby_sides2, 1, 1, 1, 0);
          delay(200);
          LCD_Sprite(218, 175, 40, 40, kirby_down2, 1, 1, 0, 0);
          delay(200);
      }
    }        
  }
}

void Timer1IntHandler() {                   //Función que lleva control de la posición de las burbujas en pantalla, se verifica si se presionó alguno de los botones del control, y si se deben otorgar puntos o no
  MAP_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  
  if (rojo1 == 1){
    LCD_Bitmap(20, posicion_YRojo1, 20, 20, anilloRojo);
    H_line( 20, posicion_YRojo1 - 1, 20, 0x74D9);
    posicion_YRojo1++;
  }
  
  if (rojo12 == 1){
    LCD_Bitmap(190, posicion_YRojo12, 20, 20, anilloRojo);
    H_line( 190, posicion_YRojo12 - 1, 20, 0x74D9);
    posicion_YRojo12++;
  }
  
  if (rojo2 == 1){
    LCD_Bitmap(20, posicion_YRojo2, 20, 20, anilloRojo);
    H_line( 20, posicion_YRojo2 - 1, 20, 0x74D9);
    posicion_YRojo2++;
  }
  
  if (rojo22 == 1){
    LCD_Bitmap(190, posicion_YRojo22, 20, 20, anilloRojo);
    H_line( 190, posicion_YRojo22 - 1, 20, 0x74D9);
    posicion_YRojo22++;
  }
  
  if (rojo3 == 1){
    LCD_Bitmap(20, posicion_YRojo3, 20, 20, anilloRojo);
    H_line( 20, posicion_YRojo3 - 1, 20, 0x74D9);
    posicion_YRojo3++;
  }
  
  if (rojo32 == 1){
    LCD_Bitmap(190, posicion_YRojo32, 20, 20, anilloRojo);
    H_line( 190, posicion_YRojo32 - 1, 20, 0x74D9);
    posicion_YRojo32++;
  }
  
  if (verde1 == 1){
    LCD_Bitmap(50, posicion_YVerde1, 20, 20, anilloVerde);
    H_line( 50, posicion_YVerde1 - 1, 20, 0x74D9);
    posicion_YVerde1++;
  }
  
  if (verde12 == 1){
    LCD_Bitmap(220, posicion_YVerde12, 20, 20, anilloVerde);
    H_line( 220, posicion_YVerde12 - 1, 20, 0x74D9);
    posicion_YVerde12++;
  }
  
  if (verde2 == 1){
    LCD_Bitmap(50, posicion_YVerde2, 20, 20, anilloVerde);
    H_line( 50, posicion_YVerde2 - 1, 20, 0x74D9);
    posicion_YVerde2++;
  }
  
  if (verde22 == 1){
    LCD_Bitmap(220, posicion_YVerde22, 20, 20, anilloVerde);
    H_line( 220, posicion_YVerde22 - 1, 20, 0x74D9);
    posicion_YVerde22++;
  }
  
  if (verde3 == 1){
    LCD_Bitmap(50, posicion_YVerde3, 20, 20, anilloVerde);
    H_line( 50, posicion_YVerde3 - 1, 20, 0x74D9);
    posicion_YVerde3++;
  }
  
  if (verde32 == 1){
    LCD_Bitmap(220, posicion_YVerde32, 20, 20, anilloVerde);
    H_line( 220, posicion_YVerde32 - 1, 20, 0x74D9);
    posicion_YVerde32++;
  }

  if (azul1 == 1){
    LCD_Bitmap(80, posicion_YAzul1, 20, 20, anilloAzul);
    H_line( 80, posicion_YAzul1 - 1, 20, 0x74D9);
    posicion_YAzul1++;
  }
  
  if (azul12 == 1){
    LCD_Bitmap(250, posicion_YAzul12, 20, 20, anilloAzul);
    H_line( 250, posicion_YAzul12 - 1, 20, 0x74D9);
    posicion_YAzul12++;
  }
  
  if (azul2 == 1){
    LCD_Bitmap(80, posicion_YAzul2, 20, 20, anilloAzul);
    H_line( 80, posicion_YAzul2 - 1, 20, 0x74D9);
    posicion_YAzul2++;
  }
  
  if (azul22 == 1){
    LCD_Bitmap(250, posicion_YAzul22, 20, 20, anilloAzul);
    H_line( 250, posicion_YAzul22 - 1, 20, 0x74D9);
    posicion_YAzul22++;
  }
  
  if (azul3 == 1){
    LCD_Bitmap(80, posicion_YAzul3, 20, 20, anilloAzul);
    H_line( 80, posicion_YAzul3 - 1, 20, 0x74D9);
    posicion_YAzul3++;
  }
  
  if (azul32 == 1){
    LCD_Bitmap(250, posicion_YAzul32, 20, 20, anilloAzul);
    H_line( 250, posicion_YAzul32 - 1, 20, 0x74D9);
    posicion_YAzul32++;
  }

  if (amarillo1 == 1){
    LCD_Bitmap(110, posicion_YAmarillo1, 20, 20, anilloAmarillo);
    H_line( 110, posicion_YAmarillo1 - 1, 20, 0x74D9);
    posicion_YAmarillo1++;
  }
  
  if (amarillo12 == 1){
    LCD_Bitmap(280, posicion_YAmarillo12, 20, 20, anilloAmarillo);
    H_line( 280, posicion_YAmarillo12 - 1, 20, 0x74D9);
    posicion_YAmarillo12++;
  }
  
  if (amarillo2 == 1){
    LCD_Bitmap(110, posicion_YAmarillo2, 20, 20, anilloAmarillo);
    H_line( 110, posicion_YAmarillo2 - 1, 20, 0x74D9);
    posicion_YAmarillo2++;
  }
  
  if (amarillo22 == 1){
    LCD_Bitmap(280, posicion_YAmarillo22, 20, 20, anilloAmarillo);
    H_line( 280, posicion_YAmarillo22 - 1, 20, 0x74D9);
    posicion_YAmarillo22++;  
  }
  
  if (amarillo3 == 1){
    LCD_Bitmap(110, posicion_YAmarillo3, 20, 20, anilloAmarillo);
    H_line( 110, posicion_YAmarillo3 - 1, 20, 0x74D9);
    posicion_YAmarillo3++;
  }
  
  if (amarillo32 == 1){
    LCD_Bitmap(280, posicion_YAmarillo32, 20, 20, anilloAmarillo);
    H_line( 280, posicion_YAmarillo32 - 1, 20, 0x74D9);
    posicion_YAmarillo32++;
  }

  if (posicion_YRojo1 == 140 or posicion_YRojo12 == 140){
    FillRect(190,posicion_YRojo12-1, 22,22, 0x74D9);
    FillRect(20,posicion_YRojo1-1, 22,22, 0x74D9);
    posicion_YRojo1 = 10;
    posicion_YRojo12 = 10;
    rojo1 = 0;
    rojo12 = 0;
  }
  
  if (posicion_YRojo2 == 140 or posicion_YRojo22 == 140){
    FillRect(190,posicion_YRojo22-1, 22,22, 0x74D9);
    FillRect(20,posicion_YRojo2-1, 22,22, 0x74D9);
    posicion_YRojo2 = 10;
    posicion_YRojo22 = 10;
    rojo2 = 0;
    rojo22 = 0;
  }
  
  if (posicion_YRojo3 == 140 or posicion_YRojo32 == 140){
    FillRect(190,posicion_YRojo32-1, 22,22, 0x74D9);
    FillRect(20,posicion_YRojo3-1, 22,22, 0x74D9);
    posicion_YRojo3 = 10;
    posicion_YRojo32 = 10;
    rojo3 = 0;
    rojo32 = 0;
  }

  if (posicion_YVerde1 == 140 or posicion_YVerde12 == 140){
    FillRect(220,posicion_YVerde12-1, 22,22, 0x74D9);
    FillRect(50,posicion_YVerde1-1, 22,22, 0x74D9);
    posicion_YVerde1 = 10;
    posicion_YVerde12 = 10;
    verde1 = 0;
    verde12 = 0;
  }
  
  if (posicion_YVerde2 == 140 or posicion_YVerde22 == 140){
    FillRect(220,posicion_YVerde22-1, 22,22, 0x74D9);
    FillRect(50,posicion_YVerde2-1, 22,22, 0x74D9);
    posicion_YVerde2 = 10;
    posicion_YVerde22 = 10;
    verde2 = 0;
    verde22 = 0;
  }
  
  if (posicion_YVerde3 == 140 or posicion_YVerde32 == 140){
    FillRect(220,posicion_YVerde32-1, 22,22, 0x74D9);
    FillRect(50,posicion_YVerde3-1, 22,22, 0x74D9);
    posicion_YVerde3 = 10;
    posicion_YVerde32 = 10;
    verde3 = 0;
    verde32 = 0;
  }

  if (posicion_YAzul1 == 140 or posicion_YAzul12 == 140){
    FillRect(250,posicion_YAzul12-1, 22,22, 0x74D9);
    FillRect(80,posicion_YAzul1-1, 22,22, 0x74D9);
    posicion_YAzul1 = 10;
    posicion_YAzul12 = 10;
    azul1 = 0;
    azul12 = 0;
  }
  
  if (posicion_YAzul2 == 140 or posicion_YAzul22 == 140){
    FillRect(250,posicion_YAzul22-1, 22,22, 0x74D9);
    FillRect(80,posicion_YAzul2-1, 22,22, 0x74D9);
    posicion_YAzul2 = 10;
    posicion_YAzul22 = 10;
    azul2 = 0;
    azul22 = 0;
  }
  
  if (posicion_YAzul3 == 140 or posicion_YAzul32 == 140){
    FillRect(250,posicion_YAzul32-1, 22,22, 0x74D9);
    FillRect(80,posicion_YAzul3-1, 22,22, 0x74D9);
    posicion_YAzul3 = 10;
    posicion_YAzul32 = 10;
    azul3 = 0;
    azul32 = 0;
  }

  if (posicion_YAmarillo1 == 140 or posicion_YAmarillo12 == 140){
    FillRect(280,posicion_YAmarillo12-1, 22,22, 0x74D9);
    FillRect(110,posicion_YAmarillo1-1, 22,22, 0x74D9);
    posicion_YAmarillo1 = 10;
    posicion_YAmarillo12 = 10;
    amarillo1 = 0;
    amarillo12 = 0;
  }
  
  if (posicion_YAmarillo2 == 140 or posicion_YAmarillo22 == 140){
    FillRect(280,posicion_YAmarillo22-1, 22,22, 0x74D9);
    FillRect(110,posicion_YAmarillo2-1, 22,22, 0x74D9);
    posicion_YAmarillo2 = 10;
    posicion_YAmarillo22 = 10;
    amarillo2 = 0;
    amarillo22 = 0;
  }
  
  if (posicion_YAmarillo3 == 140 or posicion_YAmarillo32 == 140){
    FillRect(280,posicion_YAmarillo32-1, 22,22, 0x74D9);
    FillRect(110,posicion_YAmarillo3-1, 22,22, 0x74D9);
    posicion_YAmarillo3 = 10;
    posicion_YAmarillo32 = 10;
    amarillo3 = 0;
    amarillo32 = 0;
  }
  
  Wire.begin();
  Wire.requestFrom(0x54, 2);    // request 6 bytes from slave device #2

  while(Wire.available())    // slave may send less than requested
  { 
    c = Wire.read(); // receive a byte as character
    //Serial.print(c);         // print the character
    if (c == 97){
 
      if (posicion_YRojo1 > 101 and posicion_YRojo1 < 130){
        FillRect(20,posicion_YRojo1-1, 22,22, 0x74D9);
        posicion_YRojo1 = 10;
        rojo1 = 0;
        puntosj1 = puntosj1 + 10;
      }
      
      if (posicion_YRojo2 > 101 and posicion_YRojo2 < 130){
        FillRect(20,posicion_YRojo2-1, 22,22, 0x74D9);
        posicion_YRojo2 = 10;
        rojo2 = 0;
        puntosj1 = puntosj1 + 10;
      }
      
      if (posicion_YRojo3 > 101 and posicion_YRojo3 < 130){
        FillRect(20,posicion_YRojo3-1, 22,22, 0x74D9);
        posicion_YRojo3 = 10;
        rojo3 = 0;
        puntosj1 = puntosj1 + 10;
      }
      Serial.println("hola");         // print the character
    }
    
    if (c == 98){
      
      if (posicion_YVerde1 > 101 and posicion_YVerde1 < 130){
        FillRect(50,posicion_YVerde1-1, 22,22, 0x74D9);
        posicion_YVerde1 = 10;
        verde1 = 0;
        puntosj1 = puntosj1 + 10;
      }
      if (posicion_YVerde2 > 101 and posicion_YVerde2 < 130){
        FillRect(50,posicion_YVerde2-1, 22,22, 0x74D9);
        posicion_YVerde2 = 10;
        verde2 = 0;
        puntosj1 = puntosj1 + 10;
      }
      if (posicion_YVerde3 > 101 and posicion_YVerde3 < 130){
        FillRect(50,posicion_YVerde3-1, 22,22, 0x74D9);
        posicion_YVerde3 = 10;
        verde3 = 0;
        puntosj1 = puntosj1 + 10;
      }
      Serial.println("hola");         // print the character
    }
    
    if (c == 99){
      if (posicion_YAzul1 > 101 and posicion_YAzul1 < 130){
        FillRect(80,posicion_YAzul1-1, 22,22, 0x74D9);
        posicion_YAzul1 = 10;
        azul1 = 0;
        puntosj1 = puntosj1 + 10;
      }
      if (posicion_YAzul2 > 101 and posicion_YAzul2 < 130){
        FillRect(80,posicion_YAzul2-1, 22,22, 0x74D9);
        posicion_YAzul2 = 10;
        azul2 = 0;
        puntosj1 = puntosj1 + 10;
      }
      if (posicion_YAzul3 > 101 and posicion_YAzul3 < 130){
        FillRect(80,posicion_YAzul3-1, 22,22, 0x74D9);
        posicion_YAzul3 = 10;
        azul3 = 0;
        puntosj1 = puntosj1 + 10;
      }
      Serial.println("hola");         // print the character
    }
    
    if (c == 100){
      if (posicion_YAmarillo1 > 101 and posicion_YAmarillo1 < 130){
        FillRect(110,posicion_YAmarillo1-1, 22,22, 0x74D9);
        posicion_YAmarillo1 = 10;
        amarillo1 = 0;
        puntosj1 = puntosj1 + 10;
      }
      if (posicion_YAmarillo2 > 101 and posicion_YAmarillo2 < 130){
        FillRect(110,posicion_YAmarillo2-1, 22,22, 0x74D9);
        posicion_YAmarillo2 = 10;
        amarillo2 = 0;
        puntosj1 = puntosj1 + 10;
      }
      if (posicion_YAmarillo3 > 101 and posicion_YAmarillo3 < 130){
        FillRect(110,posicion_YAmarillo3-1, 22,22, 0x74D9);
        posicion_YAmarillo3 = 10;
        amarillo3 = 0;
        puntosj1 = puntosj1 + 10;
      }
      Serial.println("hola");         // print the character
    }
  }

  Wire.requestFrom(0x55, 2);    // request 6 bytes from slave device #2

  while(Wire.available())    // slave may send less than requested
  { 
    z = Wire.read(); // receive a byte as character
    //Serial.print(z);         // print the character
    if (z == 101){
      if (posicion_YRojo12 > 101 and posicion_YRojo12 < 130){
        FillRect(190,posicion_YRojo12-1, 22,22, 0x74D9);
        posicion_YRojo12 = 10;
        rojo12 = 0;
        puntosj2 = puntosj2 + 10;
      }
      if (posicion_YRojo22 > 101 and posicion_YRojo22 < 130){
        FillRect(190,posicion_YRojo22-1, 22,22, 0x74D9);
        posicion_YRojo22 = 10;
        rojo22 = 0;
        puntosj2 = puntosj2 + 10;
      }
      if (posicion_YRojo32 > 101 and posicion_YRojo32 < 130){
        FillRect(190,posicion_YRojo32-1, 22,22, 0x74D9);
        posicion_YRojo32 = 10;
        rojo32 = 0;
        puntosj2 = puntosj2 + 10;
      }
      Serial.println("hola");         // print the character
    }
    
  if (z == 102){
    if (posicion_YVerde12 > 101 and posicion_YVerde12 < 130){
      FillRect(220,posicion_YVerde12-1, 22,22, 0x74D9);
      posicion_YVerde12 = 10;
      verde12 = 0;
      puntosj2 = puntosj2 + 10;
    }
    if (posicion_YVerde22 > 101 and posicion_YVerde22 < 130){
      FillRect(220,posicion_YVerde22-1, 22,22, 0x74D9);
      posicion_YVerde22 = 10;
      verde22 = 0;
      puntosj2 = puntosj2 + 10;
    }
    if (posicion_YVerde32 > 101 and posicion_YVerde32 < 130){
      FillRect(220,posicion_YVerde32-1, 22,22, 0x74D9);
      posicion_YVerde32 = 10;
      verde32 = 0;
      puntosj2 = puntosj2 + 10;
    }
      Serial.println("hola");         // print the character
    }
    
    if (z == 103){
      if (posicion_YAzul12 > 101 and posicion_YAzul12 < 130){
        FillRect(250,posicion_YAzul12-1, 22,22, 0x74D9);
        posicion_YAzul12 = 10;
        azul12 = 0;
        puntosj2 = puntosj2 + 10;
      }
      if (posicion_YAzul22 > 101 or posicion_YAzul22 < 130){
        FillRect(250,posicion_YAzul22-1, 22,22, 0x74D9);
        posicion_YAzul22 = 10;
        azul22 = 0;
        puntosj2 = puntosj2 + 10;
      }
      if (posicion_YAzul32 > 101 and posicion_YAzul32 < 130){
        FillRect(250,posicion_YAzul32-1, 22,22, 0x74D9);
        posicion_YAzul32 = 10;
        azul32 = 0;
        puntosj2 = puntosj2 + 10;
      }
      Serial.println("hola");         // print the character
    }
    
    if (z == 104){
      if (posicion_YAmarillo12 > 101 and posicion_YAmarillo12 < 130){
        FillRect(280,posicion_YAmarillo12-1, 22,22, 0x74D9);
        posicion_YAmarillo12 = 10;
        amarillo12 = 0;
        puntosj2 = puntosj2 + 10;
      }
      if (posicion_YAmarillo22 > 101 and posicion_YAmarillo22 < 130){
        FillRect(280,posicion_YAmarillo22-1, 22,22, 0x74D9);
        posicion_YAmarillo22 = 10;
        amarillo22 = 0;
        puntosj2 = puntosj2 + 10;
      }
      if (posicion_YAmarillo32 > 101 and posicion_YAmarillo32 < 130){
        FillRect(280,posicion_YAmarillo32-1, 22,22, 0x74D9);
        posicion_YAmarillo32 = 10;
        amarillo32 = 0;
        puntosj2 = puntosj2 + 10;
      }
      Serial.println("hola");         // print the character
    }
  }
}

int Ascii2Hex(int a){;                                //Función para convertir los carácteres ASCII provenientes de la SD a valores hexadecimales para utilizarlos en un bitmap
  switch (a) {
    case(48):
      return 0;
    case(49):
      return 1;
    case(50):
      return 2;
    case(51):
      return 3;
    case(52):
      return 4;
    case(53):
      return 5;
    case(54):
      return 6;
    case(55):
      return 7;
    case(56):
      return 8;
    case(57):
      return 9;
    case(97):
      return 10;
    case(98):
      return 11;
    case(99):
      return 12;
    case(100):
      return 13;
    case(101):
      return 14;
    case(102):
      return 15;
  }
}

void Tempo_SD(char doc[]){                          //Función para mapear los carácteres del archivo "tempo.txt" a un bitmap 
  myFile = SD.open(doc, FILE_READ);
  int hex1 = 0;
  int val1 = 0;
  int val2 = 0;
  int mapear = 0;
  int vertical = 30;
  unsigned char maps[440];

  if (myFile) {
    while (myFile.available() ) {                   //Mientras el archivo aún no se haya leído por completo, seguir leyendo el archivo
      mapear = 0;
      while (mapear < 440) {                        //Repetir el proceso hasta que se hayan impreso todas las líneas del bitmap
        hex1 = myFile.read();
        if (hex1 == 120) {
          val1 = myFile.read();
          val2 = myFile.read();
          val1 = Ascii2Hex(val1);
          val2 = Ascii2Hex(val2);
          maps[mapear] = val1 * 16 + val2;
          mapear++;
        }
      }
      LCD_Bitmap(30, vertical, 220, 1, maps);       //Imprimir línea por línea el bitmap proveniente de la SD
      vertical++;
    }
    myFile.close();
  }
  else {
    Serial.println("No se pudo abrir la imagen, prueba de nuevo.");
    myFile.close();
  }
}

void J1_SD(char doc[]){                            //Función para mapear los carácteres del archivo "J1.txt" a un bitmap 
  myFile = SD.open(doc, FILE_READ);
  int hex1 = 0;
  int val1 = 0;
  int val2 = 0;
  int mapear = 0;
  int vertical = 50;
  unsigned char maps[200];

  if (myFile) {
    while (myFile.available() ) {                   //Mientras el archivo aún no se haya leído por completo, seguir leyendo el archivo
      mapear = 0;
      while (mapear < 200) {                        //Repetir el proceso hasta que se hayan impreso todas las líneas del bitmap
        hex1 = myFile.read();
        if (hex1 == 120) {
          val1 = myFile.read();
          val2 = myFile.read();
          val1 = Ascii2Hex(val1);
          val2 = Ascii2Hex(val2);
          maps[mapear] = val1 * 16 + val2;
          mapear++;
        }
      }
      LCD_Bitmap(110, vertical, 100, 1, maps);      //Imprimir línea por línea el bitmap proveniente de la SD
      vertical++;
    }
    myFile.close();
  }
  else {
    Serial.println("No se pudo abrir la imagen, prueba de nuevo.");
    myFile.close();
  }
}

void J2_SD(char doc[]){                            //Función para mapear los carácteres del archivo "J2.txt" a un bitmap 
  myFile = SD.open(doc, FILE_READ);
  int hex1 = 0;
  int val1 = 0;
  int val2 = 0;
  int mapear = 0;
  int vertical = 50;
  unsigned char maps[200];

  if (myFile) {
    while (myFile.available() ) {                   //Mientras el archivo aún no se haya leído por completo, seguir leyendo el archivo
      mapear = 0;
      while (mapear < 200) {                        //Repetir el proceso hasta que se hayan impreso todas las líneas del bitmap
        hex1 = myFile.read();
        if (hex1 == 120) {
          val1 = myFile.read();
          val2 = myFile.read();
          val1 = Ascii2Hex(val1);
          val2 = Ascii2Hex(val2);
          maps[mapear] = val1 * 16 + val2;
          mapear++;
        }
      }
      LCD_Bitmap(110, vertical, 100, 1, maps);      //Imprimir línea por línea el bitmap proveniente de la SD
      vertical++;
    }
    myFile.close();
  }
  else {
    Serial.println("No se pudo abrir la imagen, prueba de nuevo.");
    myFile.close();
  }
}
