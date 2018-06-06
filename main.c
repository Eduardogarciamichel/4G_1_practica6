#include <18F4620.h>
#include <stdio.h>
#include <stdlib.h>
#use fast_io(a)
#use fast_io(d)
#use fast_io(b)
#use fast_io(e)
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#BIT dataDht = 0xF82.4             //pin de dato de entrada del DHT11 portc
#BIT dht_io = 0xF94.4  //bit 0 del tris b para ponerlo como entrada o salida

int temperatura, temperaturaDecimal, humidity, humidityDecimal, checksum ;
short timeOut;
int cambio=0x01;
int contador=0x01;
int display[10] = {63,6,91,79,102,109,125,7,127,103};
unsigned int dato[9] ={0} ;

void inicioDht11();
short respuesta();
unsigned int readData();
void mostrarDatos();
void temperaturaHumedad();
void configuracionesIniciales();
void habilitarInterrupciones();
int comprobacionDelChecksum();

#INT_EXT
void ISR_RB0(){
   if( contador==2){
      contador= 0x01;
   }
   else{
      contador++; 
   }
}
void main (void){
   configuracionesIniciales();
   habilitarInterrupciones();
   int sumaDatos = 0x00;
   
   while(1){
      inicioDht11();
      if(respuesta()){                    
         humidity = readData();          
         humidityDecimal = readData();                 
         temperatura = readData();                   
         temperaturaDecimal = readData();                     
         checksum = readData(); 
         sumaDatos = comprobacionDelChecksum();
         if(checksum==(sumaDatos)){
            temperaturaHumedad();
         }
      }   
   } 
}

void inicioDht11(){
   dht_io = 0;  //configuracion del pin C4 como salida
   dataDht = 0;       //se encia un 0 al sensor
   delay_ms(18);
   dataDht = 1;
   delay_us(30);
   dht_io = 1;  //configuracion de pin C4 como entrada
}
short respuesta(){
   delay_us(40);
   if(!dataDht){                     // Read and test if connection pin is low
      delay_us(80);
      if(dataDht){                    // Read and test if connection pin is high
         delay_us(50);
         return 1;}
   }
}
unsigned int readData(){
   unsigned int i, k, data = 0;     // k is used to count 1 bit reading duration
   if(timeOut)
      break;
   for(i = 0; i < 8; i++){
      k = 0;
      while(!dataDht){                          // Wait until pin goes high
         k++;
         if (k > 100) {
            timeOut = 1; 
            break;
         }
         delay_us(1);
      }
      delay_us(30);
      if(!dataDht)
         bit_clear(data, (7 - i));               // Clear bit (7 - i)
      else{
         bit_set(data, (7 - i));                 // Set bit (7 - i)
         while(dataDht){                         // Wait until pin goes low
            k++;
            if (k > 100) {timeOut = 1; break;}
               delay_us(1);}
      }
   }
   return data;
}

void temperaturaHumedad(){
   switch(cambio){
      case 1:
         output_e(cambio);
         dato[1]=(int)temperatura/10;
         dato[2]=(int)temperatura%10;
         dato[4]=(int)temperaturaDecimal/10;
         dato[8]=(int)temperaturaDecimal%10;
      break;
      case 2:
         output_e(cambio);
         dato[1]=(int)humidity/10;
         dato[2]=(int)humidity%10;
         dato[4]=(int)humidityDecimal/10;
         dato[8]=(int)humidityDecimal%10;
      break;
   }
}
void mostrarDatos(){
   if(contador==2){
         output_a(contador);
         output_d(display[dato[contador]]+128);
      }
      else{
         output_a(contador);
         output_d(display[dato[contador]]);
      } 
}
void configuracionesIniciales(){
   setup_oscillator(OSC_16MHZ);
   set_tris_d(0x00);
   set_tris_a(0x00);
   set_tris_e(0x00); 
   set_tris_b(0xFF); 
   ext_int_edge(L_TO_H);
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_128 | RTCC_8_BIT);
   set_timer0(85);
}
void habilitarInterrupciones(){
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_EXT);
   enable_interrupts(GLOBAL);
}
int comprobacionDelChecksum(){
   return temperatura+temperaturaDecimal+humidity+humidityDecimal;
}
