#line 1 "C:/Users/Usuario/Documents/GitHub/proyectosPDS/SenoDSPIC.c"



sbit LCD_RS at LATB2_bit;
sbit LCD_EN at LATB3_bit;
sbit LCD_D4 at LATB4_bit;
sbit LCD_D5 at LATB5_bit;
sbit LCD_D6 at LATB6_bit;
sbit LCD_D7 at LATB7_bit;

sbit LCD_RS_Direction at TRISB2_bit;
sbit LCD_EN_Direction at TRISB3_bit;
sbit LCD_D4_Direction at TRISB4_bit;
sbit LCD_D5_Direction at TRISB5_bit;
sbit LCD_D6_Direction at TRISB6_bit;
sbit LCD_D7_Direction at TRISB7_bit;


sbit loadPin at LATB13_bit;
sbit loadPinDir at TRISB13_bit;
sbit csPin at LATB12_bit;
sbit csPinDir at TRISB12_bit;





int adcRes;
int adcResFiltrada;

float frecuencia=60;
float frecuenciamuestreo=10000;
float frecuenciadigital;
int muestra;
int y;

short bandera = 0;









void config_timer(){
 T1CON = 0x8000;
 T1IE_bit = 1;
 T1IF_bit = 0;
 IPC0 = IPC0 | 0x1000;
 PR1 = 400;
}

void Timer1Interrupt() iv IVT_ADDR_T1INTERRUPT{
 T1IF_bit = 0;

 bandera = 1;
}





short indice = 0;
short indice2 = 0;

void main() {




 TRISA.F0 = 1;
 TRISB.F15 = 1;

 TRISB.F14 = 0;



 RPOR5 = 0x0708;

 loadPinDir = 0;
 csPinDir = 0;
 csPin = 1;
 loadPin = 1;


 SPI1_Init_Advanced(_SPI_MASTER, _SPI_16_BIT,_SPI_PRESCALE_SEC_1, _SPI_PRESCALE_PRI_1,
 _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH,
 _SPI_ACTIVE_2_IDLE);

 config_timer();

 TRISB.F15 = 0;
 LATB.F15 = 0;
 LATB.F14 =1;


 frecuenciadigital=frecuencia/frecuenciamuestreo;


 for(;;){
 if( bandera == 1){

 bandera = 0;

 y=floor((1*sin(2*3.1416*muestra*frecuenciadigital))*2048 + 2048);

 muestra++;

 if( indice == 69) {

 indice = 0;
 }




 csPin = 0;
 SPI1_Write(0x3000 | y);
 loadPin = 0;
 delay_us(2);
 loadPin = 1;
 csPin = 1;


 }

 }
}
