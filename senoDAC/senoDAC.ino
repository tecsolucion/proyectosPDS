#include <TimerOne.h>
#include <SPI.h>         // Remember this line!
#include <DAC_MCP49xx.h>
#define SS_PIN 10
 
DAC_MCP49xx dac(DAC_MCP49xx::MCP4921,SS_PIN);
 
 
int y;  // salida
int muestra=0;  // numero de muestra
float frecuencia=60;  // frecuencia de senal a generar
float frecuenciadigital;  // frecuencia digital
float frecuenciamuestreo=10000;// 1 K Hz
 
void ISR_timer(){
 
y=floor((1*sin(2*3.1416*muestra*frecuenciadigital))*2048 + 2048);  // calculo seno digital de 12 bits  // calculo sino digital de 12 bits;  // calculo sino digital de 12 bits

 muestra++;  // aumenta la muestra  

 dac.output(y); // manda la salida al DAC
 
  
}
 
void setup(){
  
  Serial.begin(9600);    // incializa comunicacions serial
  Timer1.initialize(100); //  Periodo de muestreo   // Dispara cada 1 ms =1000s  
  Timer1.attachInterrupt(ISR_timer);// Activa la interrupcion y la asocia a ISR_timer
 
 
  dac.setSPIDivider(SPI_CLOCK_DIV16);
  
  // Use "port writes", see the manual page. In short, if you use pin 10 for
  // SS (and pin 7 for LDAC, if used), this is much faster.
  // Also not strictly required (no setup() code is needed at all).
  dac.setPortWrite(true);
  
}
 
void loop(){
  
  frecuenciadigital=frecuencia/frecuenciamuestreo;
  
    
}
