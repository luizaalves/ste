/*
 * Timer.cpp
 *
 *  Created on: 24 de mar de 2017
 *      Author: aluno
 */

#include "Timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

Timer::Timer(Hertz freq)
: _ticks(0), _timer_base(0), _us_per_tick(0),_n_timeout(0)
{
	static const unsigned int Timer_Top = 0xFF;

	static const Hertz Fmax_1024 = F_CPU / 1024;
	static const Hertz Fmax_256  = F_CPU / 256;
	static const Hertz Fmax_64   = F_CPU / 64;
	static const Hertz Fmax_8    = F_CPU / 8;
	static const Hertz Fmax_1    = F_CPU / 1;
	static const Microseconds Period_div[5] = {
			   (1*1000000) / F_CPU, //1
			   (8*1000000) / F_CPU, //8
			  (64*1000000) / F_CPU, //64
			 (256*1000000) / F_CPU, //256
			(1024*1000000) / F_CPU  //1024
	};

	TCCR0A = 0x00; // normal operation
	int div = 0;

	// lógica para selecionar divisor
	if(freq <= Fmax_1024)
	{
		TCCR0B = 0x05; // div 1024
		div = 1024;
	}
	else if(freq <= Fmax_256)
	{
		TCCR0B = 0x04; // div 256
		div = 256;
	}
	else if(freq <= Fmax_64)
	{
		TCCR0B = 0x03; // div 64
		div = 64;
	}
	else if(freq <= Fmax_8)
	{
		TCCR0B = 0x02; // div 8
		div = 8;
	}
	else if(freq <= Fmax_1)
	{
		TCCR0B = 0x01; // div 1
		div = 1;
	}
	else // #error "Frequency out of range (should be between 62Hz and 250kHz)."
		return;

	// calcular ciclos de timer
	Hertz ciclos = (F_CPU/div)/freq;
	if(ciclos > Timer_Top) return; // Frequência solicitada é muito alta!
	_timer_base = (Timer_Top + 1) - ciclos;

	//TODO: compute _us_per_tick
	_us_per_tick = ciclos * Period_div[(TCCR0B & 0x07) - 1];

	TCNT0  = _timer_base;
	TIMSK0 = 0x01; // liga int de ov
    
   
}

Milliseconds Timer::millis()
{
	return micros() / 1000;
}

Microseconds Timer::micros()
{
	return _ticks * _us_per_tick;
}

void Timer::delay(Milliseconds ms)
{
	udelay(ms*1000);
}

void Timer::udelay(Microseconds us)
{
	Microseconds start = micros();
	while((micros() - start) <= us);
}
//Alterado para Atividade-5
uint8_t Timer::addTimeout(uint32_t interval, CALLBACK_t callback){
    uint8_t id = 5;
    if(_n_timeout < 4){
        _timeout[_n_timeout].config(interval,callback);
        id = _n_timeout;
        _n_timeout++;
    }
    return id;
}
//---
void Timer::timeoutManager(){
    if(_n_timeout > 0){
        for(int i = 0; i < _n_timeout; i++){
            _timeout[i].checkTimeout();
        }
    }        
}

//Inserido na Atividade-5
void Timer::enable_timeout(uint8_t id_timeout){
    if(_n_timeout > id_timeout){
        self()->_timeout[id_timeout].enable(true);
    }
}

void Timer::reload_timeout(uint8_t id_timeout){
    if(_n_timeout > id_timeout){
        self()->_timeout[id_timeout].reload();
    }
}

void Timer::disable_timeout(uint8_t id_timeout){
    if(_n_timeout > id_timeout){
        self()->_timeout[id_timeout].enable(false);
    }
}

void Timer::set_intervalTimeout(uint32_t interv, uint8_t id_timeout){
    if(_n_timeout > id_timeout){
        self()->_timeout[id_timeout].set_interval(interv);
    }
}
//---

ISR(TIMER0_OVF_vect) { Timer::ovf_isr_handler(); }
void Timer::ovf_isr_handler() {
	TCNT0  = self()->_timer_base;
	self()->_ticks++;
    
    for(int i = 0;i < self()->_n_timeout ;i++){
        if(self()->_timeout[i].enabled()){          //Alterado para Atividade-5
            self()->_timeout[i].inc_counter();
        }
    }
}
