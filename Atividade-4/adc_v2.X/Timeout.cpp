/* 
 * File:   Timeout.cpp
 * Author: aluno
 * 
 * Created on 25 de Setembro de 2019, 08:10
 */

#include "Timeout.h"

Timeout::Timeout(){
    _counter=0;
    _interval=0;
    _enable = true;
    _event = false;
}
void Timeout::checkTimeout(){
    if(_event){
        callback();
        _event = false;
    }
}
void Timeout::callback(){
    (*_callback)();
}
void Timeout::inc_counter(){
    _counter++;
    if(_interval == _counter){
        _event = true;
        _counter = 0;
    }
}
void Timeout::config(uint32_t interval, CALLBACK_t callback){
    _interval = interval;
    _callback = callback;
    
}
