/*
  Programa para executar os comandos da sinalizacao do BREletrico
  Placa Arduino Mega instalada em baixo do painel do veiculo

  A sinalizacao dos comandos no LCD sao:
  - setaDireitaOn();
  - setaDireitaOff();
  - setaEsquerdaOn();
  - setaEsquerdaOff();
  - farolBaixo();
  - farolAlto();
  - freioDeMao();
  - bateria();

  This example code is in the public domain.
  
  2018/09/15 - Transferido para Servidor Raspberry no carro
               Tela de inicializacao
               Ainda fala implementar o CAN e o Audio beep
  2020/11/22 - Nova placa com Arduino Nano - MCP2551             
               Testando funcionalidades 

*/

#define  lcd_RS 7    // D7
#define  lcd_RW 8    // D8
#define  lcd_E  9    // D9
// Falta implementar Reset no Shiftrefister
#define  beep   6    //  D6 

#include "U8glib.h"
#include "MsTimer2.h"

char versao[10]="22nov20";

U8GLIB_ST7920_128X64_4X u8g(lcd_E , lcd_RW, lcd_RS ); //13, 12, 11);  // Enable, RW, RS, RESE
// D7 = RS
// D8 = R/W
// D9 = E
// Falta implementar Reset no Shiftrefister

boolean PinFarolBaixa ; // = 34;   // fio preto
boolean PinFarolAlta  ; // = 32;    // fio marron 
boolean PinLanterna   ; //  = 30;     // fio amarela
boolean PinLuzDeFreio ; //= 28;   // fio laranja 
boolean PinLuzRe      ; // = 26;        // fio verde 
boolean PinSetaDireita; // = 24;  // fio azul  
boolean PinSetaEquerda; // = 22;  // fio vermelho

const int InLanterna = 53;
const int InFarolBaixo = 51;
const int InFarolAlto = 49;

const int InSetaEsquerda = 47;
const int InSetaDireita = 45;
const int InPiscaAlerta = 43; 
const int InFreioDeMao = 41;
const int InLuzDeFreio = 39;
const int InLuzRe = 37;

boolean BpiscaAlerta = 0;
boolean BsetaEsquerda = 0;
boolean BsetaDireita = 0;
boolean BfarolBaixo = 0;
boolean BfarolAlto = 0;
boolean Blanterna = 0;
boolean BfreioDeMao = 0;
boolean BluzDeFreio = 0;
boolean BluzRe = 0;

boolean pisca_on;


void LigaFarolBaixo() {
  boolean farol[][20] {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  // farol baixo
  int baixo = 2;
  int dX = 50;
  int dY = 40;
  for ( int x = 0; x < 13; x++)
    for ( int y = 0; y < 20; y++)
      if (farol[y][x] == 1)
        u8g.drawPixel(x + dX + 10, y + dY);
  u8g.drawLine(1 + dX, 1 + dY + baixo, 9 + dX, 1 + dY);
  u8g.drawLine(1 + dX, 5 + dY + baixo, 8 + dX, 5 + dY);
  u8g.drawLine(1 + dX, 10 + dY + baixo, 7 + dX, 10 + dY);
  u8g.drawLine(1 + dX, 15 + dY + baixo, 8 + dX, 15 + dY);
  u8g.drawLine(1 + dX, 19 + dY + baixo, 9 + dX, 19 + dY);
}

void LigaFarolAlto() {
  boolean farol[][20] {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  }; 
  // farol alto
  int dX = 50;
  int dY = 40;
  for ( int x = 0; x < 13; x++)
    for ( int y = 0; y < 20; y++)
      if (farol[y][x] == 1)
        u8g.drawPixel(x + dX + 10, y + dY);
  u8g.drawLine(1 + dX, 1 + dY, 9 + dX, 1 + dY);
  u8g.drawLine(1 + dX, 5 + dY, 8 + dX, 5 + dY);
  u8g.drawLine(1 + dX, 10 + dY, 7 + dX, 10 + dY);
  u8g.drawLine(1 + dX, 15 + dY, 8 + dX, 15 + dY);
  u8g.drawLine(1 + dX, 19 + dY, 9 + dX, 19 + dY);
}


void SetaDireitaOn()
{
  int dX = 112;
  int dY = 20;
  u8g.drawTriangle(0 + dX, 0 + dY, 0 + dX, 20 + dY, 15 + dX, 10 + dY);
}

void SetaDireitaOff()
{
  int dX = 113;
  int dY = 20;
  u8g.drawLine(0 + dX, 0 + dY, 0 + dX, 20 + dY);
  u8g.drawLine(0 + dX, 0 + dY, 15 + dX, 10 + dY);
  u8g.drawLine(15 + dX, 10 + dY, 0 + dX, 20 + dY);
}

void SetaEsquerdaOn()
{
  int dX = 0;
  int dY = 20;
  u8g.drawTriangle(15 + dX, 0 + dY, 15 + dX, 20 + dY, 0 + dX, 10 + dY); // seta esquerda
}

void SetaEsquerdaOff()
{
  int dX = 0;
  int dY = 20;
  u8g.drawLine(15 + dX, 0 + dY, 15 + dX, 20 + dY);
  u8g.drawLine(15 + dX, 0 + dY, 0 + dX, 10 + dY);
  u8g.drawLine(0 + dX, 10 + dY, 15 + dX, 20 + dY);
}

void BaseDeTempo(void)
{
 pisca_on =! pisca_on;
}

void beep_on(void)
{ 
  //digitalWrite(beep,HIGH);
  analogWrite(beep, 100);
}

void beep_off(void)
{ 
  digitalWrite(beep,LOW);
}

void setup() {
  pinMode(beep, OUTPUT);
  /* pinMode(PinSetaDireita, OUTPUT);
  pinMode(PinSetaEquerda, OUTPUT);
  pinMode(PinFarolBaixa, OUTPUT);
  pinMode(PinFarolAlta, OUTPUT);
  pinMode(PinLanterna, OUTPUT);
  pinMode(PinLuzDeFreio, OUTPUT);
  pinMode(PinLuzRe, OUTPUT);
  pinMode(InPiscaAlerta, INPUT); // pisca alerta
  pinMode(InSetaEsquerda, INPUT);// seta esquerda
  pinMode(InSetaDireita, INPUT); //seta direita 
  pinMode(InFarolBaixo, INPUT); // farol baixo
  pinMode(InFarolAlto, INPUT); // farol alto
  pinMode(InLanterna, INPUT); 
  pinMode(InFreioDeMao, INPUT);
  pinMode(InLuzRe,INPUT); */

 

  /* display grafico */
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }  
  MsTimer2::set(500, BaseDeTempo);
  MsTimer2::start();
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("BREletrica CAN Luzes ");
  beep_on();
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(20, 20, "BREletrico");      
    u8g.drawStr(30, 54, versao);
  } while (u8g.nextPage());
  delay(1000);
  beep_off();
  BfarolAlto = 1; 
}

void SequencialLuzes(void) {
  PinSetaDireita=1; delay(1000); PinSetaDireita=0; delay(1000);
  PinSetaEquerda=1; delay(1000); PinSetaEquerda=0; delay(1000);
  PinFarolBaixa=1;  delay(1000); PinFarolBaixa=0;  delay(1000);
  PinFarolAlta=1;   delay(1000); PinFarolAlta=0;   delay(1000);
  PinLanterna=1;    delay(1000); PinLanterna=0;    delay(1000);
  PinLuzDeFreio=1;  delay(1000); PinLuzDeFreio=0;  delay(1000);
  PinLuzRe=1;       delay(1000); PinLuzRe=0;       delay(1000);
}

void Leia_Chaves(void) {
  
   byte x; 
   /* BpiscaAlerta = digitalRead(InPiscaAlerta);
   BsetaEsquerda = digitalRead(InSetaEsquerda);
   BsetaDireita = digitalRead(InSetaDireita);
   BfarolBaixo = digitalRead(InFarolBaixo);
   BfarolAlto = digitalRead(InFarolAlto);
   Blanterna = digitalRead(InLanterna);
   BfreioDeMao = digitalRead(InFreioDeMao);
   BluzDeFreio = digitalRead(InLuzDeFreio);  
*/
    
   x= BpiscaAlerta*B10000000+BsetaEsquerda*B01000000+BsetaDireita*B00100000+BfarolBaixo*B00010000+BfarolAlto*B00001000+Blanterna*B00000100+BfreioDeMao*B00000010+BluzDeFreio*B00000001;
    
   Serial.print(x,BIN);
   Serial.println(" Entradas ");
 
  char inChar;
  if (Serial.available()) {
   inChar = (char)Serial.read();
   Serial.print(" Valor = ");Serial.print(inChar);
   switch (inChar){
    case '0':  BpiscaAlerta = 0; //digitalRead(InPiscaAlerta);
               BsetaEsquerda = 0; // digitalRead(InSetaEsquerda);
               BsetaDireita = 0; //digitalRead(InSetaDireita);
               BfarolBaixo = 0; // digitalRead(InFarolBaixo);
               BfarolAlto = 0; //digitalRead(InFarolAlto);
               Blanterna = 0;  //digitalRead(InLuzAlta);
               BfreioDeMao = 0; //digitalRead(InFreioDeMao);
               BluzDeFreio = 0; //digitalRead(InLuzDeFreio);
               break;
    case '1':  BpiscaAlerta = 1; break; // Blaterna
    case '2':  BfarolBaixo = 1; break; //setaEsquerda = 1; break;
    case '3':  BsetaDireita = 1; break;
    case '4':  BsetaEsquerda = 1; break; //farolBaixo = 1; break;
    case '5':  BfarolAlto = 1; break;
    case '6':  Blanterna = 1;  break;
    case '7':  BfreioDeMao = 1; break;
    case '8':  BluzDeFreio = 1; break;
    default: break;
    }
  } 

  //BpiscaAlerta = 1; //digitalRead(InPiscaAlerta);
  //BsetaEsquerda = 1; // digitalRead(InSetaEsquerda);
  //BsetaDireita = 1; //digitalRead(InSetaDireita);
  //BfarolBaixo = 1; // digitalRead(InFarolBaixo);
  //BfarolAlto = 1; //digitalRead(InFarolAlto);
  //Blanterna = 1;  //digitalRead(InLuzAlta);
  //BfreioDeMao = 1; //digitalRead(InFreioDeMao);
  //BluzDeFreio = 1; //digitalRead(InLuzDeFreio);*/
  
}

void draw() {
  // graphic commands to redraw the complete screen should be placed here  
  // comandos simultaneamento no LCD e na Sinalizacao  
  if (BfarolBaixo) {LigaFarolBaixo(); PinFarolBaixa=1; } else PinFarolBaixa=0;
  if (BfarolAlto) {LigaFarolAlto(); PinFarolAlta=1;  } else PinFarolAlta=0;  
  if (Blanterna) { PinLanterna=1; } else { PinLanterna=0; }
  if (BluzDeFreio) { PinLuzDeFreio=1; } else { PinLuzDeFreio=1;}
  if (BluzRe) { PinLuzRe=1;} else { PinLuzRe=0; }
  
  // comandos somente no LCD
  if (BfreioDeMao) {;}

  // comandos temporizados no LCD e na sinalizacao
  if (BpiscaAlerta) 
  { 
   if (pisca_on) 
   {
    SetaEsquerdaOn();  SetaDireitaOn(); PinSetaEquerda=1; PinSetaDireita=1; beep_on(); }
    else 
   { 
    SetaEsquerdaOff(); SetaDireitaOff();PinSetaEquerda=0; PinSetaDireita=0; beep_off(); }
  }
   else {SetaEsquerdaOff(); SetaDireitaOff();PinSetaEquerda=0; PinSetaDireita=0; beep_off();}
     
  if (BsetaEsquerda) 
  { 
    if (pisca_on) {SetaEsquerdaOn(); PinSetaEquerda=1; } else { SetaEsquerdaOff(); PinSetaEquerda=0;}
  } 
   else  { SetaEsquerdaOff(); PinSetaEquerda=0;}
  if (BsetaDireita) 
  { 
    if (pisca_on) {SetaDireitaOn();PinSetaDireita=1; } else { SetaDireitaOff(); PinSetaDireita=0;} 
  } else { SetaDireitaOff(); PinSetaDireita=0;}
}

void loop() {
  // SequencialLuzes();   //
  Leia_Chaves();
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );
}
