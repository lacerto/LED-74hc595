/* Controls 8 LEDs using a 74HC595 shift register. */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <wiringPi.h>
#include <wiringShift.h>

// Define pins connected to the 74HC595 (wiringPi numbering).
#define SD   0  // serial data input
#define STCP 1  // storage register clock pulse
#define SHCP 2  // shift register clock pulse

/* Sets the pin(s) to output and the initial value to 0. */
void
init_pins(int pin, ...);

/* Sends a pulse to the pin. */
void
send_pulse(int pin);

int
main(void)
{
    signed char exp = 0;
    char direction = 1;
    int delayms = 50;
    int delayinc = 50;

    wiringPiSetup();

    init_pins(SD, STCP, SHCP);

    /* Turn on the LEDs one by one in Knight Rider fashion.
       Increase the delay gradually between each step. */
    do
    {
        // Shift out a byte to the 74HC595.
        shiftOut(SD, SHCP, LSBFIRST, (uint8_t) pow(2, exp));
        // Send a pulse on the storage register clock line
        // so that the byte shows up on the output.
        send_pulse(STCP);

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

    // Turn off all the LEDs.
    shiftOut(SD, SHCP, LSBFIRST, 0x00);
    send_pulse(STCP);

    return EXIT_SUCCESS;
}

/* Sets the pin(s) to output and the initial value to 0. */
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

/* Sends a pulse to the pin. */
void
send_pulse(int pin)
{
    digitalWrite(pin, 1);
    digitalWrite(pin, 0);
}
