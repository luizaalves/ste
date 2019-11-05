
#include <avr/io.h> //ctrl + clicar nele abre o .h; ctrl + f = atmega2560 > ctrl + clicar nele abre o .h
#include <util/delay.h>
#include "UART.h"
#include <avr/interrupt.h>
#include <stdlib.h>
void ADC_init();
//unsigned int adc_read();

UART uart(9600, UART::DATABITS_8, UART::NONE, UART::STOPBIT_1);
long sensorValue;
int main(int argc, char** argv) {   

    ADC_init();
    sei();
    ADCSRA |= (1<<ADSC);
    for(;;){
        //_delay_ms(1000);
        //sensorValue = adc_read();        
    }
    
    return 0;
}
void ADC_init(){
    ADMUX = (1<<REFS0);
    ADCSRA = (7<<ADPS0) | (1<<ADEN) | (1<<ADIE);
    
    
}
//unsigned int adc_read(){
//    //ADCSRA |= (1<<ADSC);
//    //while(ADCSRA & (1<<ADSC))
//        //;
//    //return ADC;
//}
ISR(ADC_vect){
    sensorValue = ADC;
    char x[sizeof(long)*8+1];
    ltoa(sensorValue,x,10);
    uart.puts(x);
    ADCSRA |= (1<<ADSC);
}