

// Lcd module connections
sbit LCD_RS at LATB2_bit;                           // LATB escribe en la salida
sbit LCD_EN at LATB3_bit;
sbit LCD_D4 at LATB4_bit;
sbit LCD_D5 at LATB5_bit;
sbit LCD_D6 at LATB6_bit;
sbit LCD_D7 at LATB7_bit;

sbit LCD_RS_Direction at TRISB2_bit;                // TRISB  configuracion como  entrada o salida
sbit LCD_EN_Direction at TRISB3_bit;
sbit LCD_D4_Direction at TRISB4_bit;
sbit LCD_D5_Direction at TRISB5_bit;
sbit LCD_D6_Direction at TRISB6_bit;
sbit LCD_D7_Direction at TRISB7_bit;
// End Lcd module connections

sbit loadPin at LATB13_bit;                  // DAC load pin      pone un cero en LATB13
sbit loadPinDir at TRISB13_bit;              // DAC load pin
sbit csPin at LATB12_bit;                    // DAC CS pin
sbit csPinDir at TRISB12_bit;                // DAC CS pin

//char txt3[] = "Hola Mundo";
//char txt[10];                                                                              .
//char txt2[10];

int adcRes;
int adcResFiltrada;

float frecuencia=60;
float frecuenciamuestreo=10000;
float frecuenciadigital;
int muestra;
int y;

short bandera = 0;
            
//Timer1
//Prescaler 1:1; PR1 Preload = 400; Actual Interrupt Time = 100 us

//Place/Copy this part in declaration section
//Timer1
//Prescaler 1:1; PR1 Preload = 400; Actual Interrupt Time = 100 us

//Place/Copy this part in declaration section
void config_timer(){
  T1CON	 = 0x8000;
  T1IE_bit	 = 1;
  T1IF_bit	 = 0;
  IPC0		 = IPC0 | 0x1000;
  PR1		 = 400;
}

void Timer1Interrupt() iv IVT_ADDR_T1INTERRUPT{
  T1IF_bit	 = 0;
  //Enter your code here
   bandera = 1;
}





short indice = 0;
short indice2 = 0;

void main() {

  //Lcd_Init();
  //Lcd_Cmd(_LCD_CLEAR);               // Clear display
  //Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
  TRISA.F0 = 1;     //pin RA0 como entrada
  TRISB.F15 = 1;

  TRISB.F14 = 0;   //RB14 como salida

  //PPS_Mapping(10, _OUTPUT, _SCK1OUT);    // Sets pin 15 to be Input, and maps RX2/DT2 Input to it
  //PPS_Mapping(11, _OUTPUT, _SDO1);   // Sets pin 5 to be Output, and maps EUSART2 Asynchronous Transmit/Synchronous Clock Output to it
  RPOR5 = 0x0708;
  // DAC setup
  loadPinDir = 0;                           // LOAD pin
  csPinDir   = 0;                           // CS pin
  csPin    = 1;                             // Set CS to inactive
  loadPin  = 1;                             // Set LOAD to inactive

  // SPI setup
  SPI1_Init_Advanced(_SPI_MASTER, _SPI_16_BIT,_SPI_PRESCALE_SEC_1, _SPI_PRESCALE_PRI_1,
                     _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH,
                     _SPI_ACTIVE_2_IDLE);
  //ADC1_Init_Advanced(_ADC_12bit, _ADC_INTERNAL_REF);
  config_timer();

  TRISB.F15 = 0;
  LATB.F15 = 0;
  LATB.F14 =1;

  //Lcd_Out(1,1,txt3);                 // Write text in first row
  frecuenciadigital=frecuencia/frecuenciamuestreo;
  

  for(;;){
    if( bandera == 1){

      bandera = 0;

      y=floor((1*sin(2*3.1416*muestra*frecuenciadigital))*2048 + 2048);  // calculo seno digital de 12 bits  // calculo sino digital de 12 bits;  // calculo sino digital de 12 bits

       muestra++;

      if( indice == 69)    {

        indice = 0;
        }




      csPin = 0;                                // CS enable for DAC
      SPI1_Write(0x3000 | y);
      loadPin  = 0;                             // Load data in DAC
      delay_us(2);
      loadPin  = 1;
      csPin    = 1;                             // CS disable for DAC
      //WordToStr(adcResFiltrada, txt2);
      //Lcd_Out(2,1,txt2);
    }
    //Delay_ms(50);
  }
}