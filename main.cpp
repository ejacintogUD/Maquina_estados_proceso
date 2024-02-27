/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"


BufferedSerial pc(USBTX, USBRX);
PwmOut motor(D9);
InterruptIn btn_start(BUTTON1);

// Blinking rate in milliseconds
#define BLINKING_RATE     300ms
#define base_tiempo       2000ms

Thread hilo_arranque;
Thread hilo_incrementar;
Thread hilo_mantener;
Thread hilo_decrementar;

Semaphore sema_arranque;
Semaphore sema_incre;
Semaphore sema_mant;
Semaphore sema_decre;

char men[25]; 

const int velocidades[4] = {30, 50, 70, 100};

void lanzar_arranque(void);
void arranque(void);
void incrementar (void);
void mantener(void);
void decrementar(void);


int main()
{
    // Initialise the digital pin LED1 as an output
    DigitalOut led(LED1);
    sprintf(men, "Inicio del sistema \n\r");
    pc.write(men,25);
    btn_start.rise(lanzar_arranque);
    
    hilo_arranque.start(arranque);
    hilo_incrementar.start(incrementar);
    hilo_mantener.start(mantener);
    hilo_decrementar.start(decrementar);
    
    while (true) {
        led = !led;
        ThisThread::sleep_for(BLINKING_RATE);
    }
}


void  lanzar_arranque(void)
{
    sema_arranque.release();
}

void arranque(void) 
{
const char men_arranque[25] = "Arranque del sistema\n\r";

    while(true)
    {
        sema_arranque.acquire();
        sprintf(men,men_arranque);
        pc.write(men,21);
        sema_incre.release();
    }
}

void incrementar(void) 
{
int i=0;
const char men_incrementar[25] = "Incrementar \t";

    while(true)
    {
        sema_incre.acquire();
        sprintf(men,men_incrementar);
        pc.write(men, 14);
        while(i<4)
        {
        motor = float(velocidades[i])/100;
        sprintf(men,"Velocidad %u porciento \n\r", velocidades[i]);
        pc.write(men, 25);
        i++;
        ThisThread::sleep_for(base_tiempo);
        }
        i=0;
        sema_mant.release();
    }
}

void mantener(void) 
{
const char men_mantener[25] = "Mantener \n\r";

    while(true)
    {
        sema_mant.acquire();
        sprintf(men,men_mantener);
        pc.write(men,12);
        ThisThread::sleep_for(2000ms);
        sema_decre.release();
    }
}

void decrementar(void) 
{
const char men_decrementar[25] = "Decrementar \n\r";

    while(true)
    {
        sema_decre.acquire();
        sprintf(men,men_decrementar);
        pc.write(men,14);
        ThisThread::sleep_for(2000ms);
        sema_incre.release();
    }
}

