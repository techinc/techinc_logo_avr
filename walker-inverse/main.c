#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay_basic.h>
#include <stdint.h>

#define PBB0 (1<<0)
#define PBB1 (1<<1)
#define PBB2 (1<<2)
#define PBB3 (1<<3)
#define PBB4 (1<<4)
#define PBB5 (1<<5)

#define ORANGE PBB4
#define BLUE   PBB3
#define GREEN  PBB2
#define PURPLE PBB1

#define LED_OFF (9)

const uint8_t grid_output[16] =
{
	(ORANGE|BLUE),     (BLUE|ORANGE), /* clockwise, starting north */
	(GREEN|BLUE),       (BLUE|GREEN),
	(GREEN|PURPLE),   (PURPLE|GREEN),
	(ORANGE|PURPLE), (PURPLE|ORANGE),
	0
};

const uint8_t grid_high[16] =
{
   (BLUE),	(ORANGE), /* clockwise, starting north */
    (BLUE),	(GREEN),
  (PURPLE),	(GREEN),
 (PURPLE),	(ORANGE),
	0
};

const uint8_t fade[] =
{
	0x01|0x30,
	0x03|0x20,
	0x02|0x60,
	0x06|0x40,
	0x04|0xc0,
	0x0c|0x80,
    0x08|0x81,
    0x18|0x01,
    0x10|0x03,
    0x30|0x02,
    0x20|0x06,
    0x60|0x04,
    0x40|0x0c,
    0xc0|0x08,
    0x80|0x18,
    0x81|0x10,
    0x01|0x30 /* fade[0] */
};

void led_on_ll(uint8_t output, uint8_t high)
{
	DDRB  = output; 
	PORTB = high;
}

void led_on(uint8_t which)
{
	uint8_t output, high;
	output = grid_output[which];
	high = grid_high[which];
	led_on_ll(output, high);
}

int main(void)
{
	uint8_t phase, led, pwm, i;

	i=0;
	for (phase=254 ;; phase-=2)
	{
		for (led=0; led<8; led++)
		{
			if ( (1<<led) & fade[i] )
				led_on(led);
			else
				led_on(LED_OFF);

			for ( pwm=0 ; pwm<phase ; pwm+=2 );

			if ( (1<<led) & fade[i+1] )
				led_on(led);
			else
				led_on(LED_OFF);

			for ( ; pwm ; pwm+=2 );
		}

		if (phase == 0)
			i++;

		if (i==sizeof(fade)-1)
			i=0;
	}
}

