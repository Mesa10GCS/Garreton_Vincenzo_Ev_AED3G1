#include "ds18b20.h"

#define PIN 5

void dq_output(){
 RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
 GPIOA->CRL&=~(0xF<<(PIN*4));
GPIOA->CRL&=~(0x5<<(PIN*4));
}

void dq_input(){
 RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
 GPIOA->CRL&=~(0xF<<(PIN*4));
GPIOA->CRL&=~(0x4<(PIN*4));
}

void dq_low(){

    GPIOA->BSRR|=(1<<(PIN +16));
}

uint8_t dq_read(){
    GPIOA->IDR&(1<<PIN);
    return GPIOA->IDR&(1<<PIN);
}

void ds18b20_init(){
    int t_pre=0;
    dq_output();
    GPIOA->BSRR|=(1<<(PIN));
    dq_low();
    delay_us(480);
    GPIOA->BSRR|=(1<<(PIN));
    dq_input();
    while(dq_read==0){
        delay_us(1);
        t_pre++;
    }
    if(t_pre<70){
    while(t_pre<240 && dq_read()==0){
        delay_us(1);
        t_pre++;
    }
}
}

uint8_t ds18b20_reset(){
    int presencia;
    dq_output();
    GPIOA->BSRR|=(1<<(PIN));
    dq_low();
    delay_us(480);
    dq_input();
    presencia=!dq_read();
    return presencia;
}

void ds18b20_write_bit(uint8_t bit){
    dq_output();
    if(bit==0){
        dq_low();
        delay_us(60);
        GPIOA->BSRR|=(1<<(PIN));
    }
    else if(bit==1){
        dq_low();
        GPIOA->BSRR|=(1<<(PIN));
        delay_us(2);
        dq_low();
    }
}

uint8_t ds18b20_read_bit(){
    int read;
    dq_output();
    dq_low();
    delay_us(1);
    GPIOA->BSRR|=(1<<(PIN));
    dq_input();
    read = dq_read();
    delay_us(15);
    return read;
}

void ds18b20_write_byte(uint8_t data){
    for(int i;i<8;i++){
        if((data>>i)&0x01) ds18b20_write_bit(1);
        else ds18b20_write_bit(0);
    }
}

uint8_t ds18b20_read_byte(){
    uint8_t byte;
    for(int i;i<8;i++){
    byte=(ds18b20_read_bit()<<i);
    }
    return byte;
}

void ds18b20_start_conversion(){
    if(ds18b20_reset()==0) return;
    else {
       ds18b20_write_byte(0xCC);
       ds18b20_write_byte(0x44);
    }
}

float ds18b20_get_temperature(){
    int temp;
    ds18b20_write_byte(0xBE);
    ds18b20_write_byte(0xCC);
    temp=ds18b20_read_byte();
    return temp=((ds18b20_read_byte())<<8);
}