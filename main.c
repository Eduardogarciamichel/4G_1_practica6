#include <18F4620.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
int cont=0x01;
#int_ext
void isr_rb0(){
      if( cont==2){
        cont= 0x01;
      }
      else
      {
      cont++; 
      }
}
int contadorTimer=1;
#int_timer0
void isr_timer0(){
    contadorTimer<<=1;
    if(contadorTimer>=8){
        contadorTimer=1;
    }
}
void main (void){
   setup_oscillator(OSC_16MHZ);
   set_tris_b(0x01);
   set_tris_e(0x00);
   set_tris_a(0x00);
   ext_int_edge(L_TO_H);
   set_timer0(0x00);
   setup_timer_0(RTCC_INTERNAL |RTCC_DIV_128 | T0_8_BIT); 
   enable_interrupts(INT_TIMER0);   
   enable_interrupts(INT_EXT);
   enable_interrupts(GLOBAL);
   while(1){
      output_e(cont);    
      output_a(contadorTimer);
   } 
}   
