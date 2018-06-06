#include <18F4620.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
int cont=0x01;
#INT_EXT
VOID ISR_RB0(){
      if( cont==2){
        cont= 0x01;
      }
      else
      {
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
