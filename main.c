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
int cont=0x01;
#INT_EXT
void ISR_RB0(){
   if( cont==2){
      cont= 0x01;
   }
   else{
      cont++; 
   }
}
void main (void){
   setup_oscillator(OSC_16MHZ);
   set_tris_b(0x01);
   set_tris_e(0x00);
   EXT_INT_EDGE(L_TO_H);
     
   enable_interrupts(INT_EXT);
   enable_interrupts(GLOBAL);
   while(1){
      output_e(cont);    
   } 
}   
