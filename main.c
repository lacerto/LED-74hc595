#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <wiringPi.h>
#include <wiringShift.h>

// Define pins connected to the 74HC595.
#define SD   0  // serial data input
#define STCP 1  // storage register clock pulse
#define SHCP 2  // shift register clock pulse

void
init_pins(int pin, ...);

void
send_pulse(int pin);

int
main(void)
{
    signed char exp = 0;
    char direction = 1;
    int delayms = 50;
    int delayinc = 25;

    wiringPiSetup();

    init_pins(SD, STCP, SHCP);

    do
    {
        shiftOut(SD, SHCP, LSBFIRST, (uint8_t) pow(2, exp));
        send_pulse(STCP);
        printf("Delay: %d ms\n", delayms);
        delay(delayms);
        exp += direction;
        if (exp == 8)
        {
            exp = 6;
            direction = -1;
        }
        if (exp < 0)
        {
            exp = 1;
            direction = 1;
        }
        delayms += delayinc;
    }
    while (delayms <= 1000);

    shiftOut(SD, SHCP, LSBFIRST, 0x00);
    send_pulse(STCP);

    return EXIT_SUCCESS;
}

void
init_pins(int pin, ...)
{
    va_list ap;
    int p;

    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    va_start(ap, pin);
    while ( (p = va_arg(ap, int)) != 0 )
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    va_end(ap);
}

void
send_pulse(int pin)
{
    digitalWrite(pin, 0);
    digitalWrite(pin, 1);
}
