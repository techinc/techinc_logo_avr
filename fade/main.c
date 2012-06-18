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
	(ORANGE),   (BLUE), /* clockwise, starting north */
	(GREEN),    (BLUE),
	(GREEN),  (PURPLE),
	(ORANGE), (PURPLE),
	0
};

const uint8_t fade[] =
{
	0x00, 0xff, 0x00, 0x01, 0x00, 0x00, 0x10, 0x38,
	0x7c, 0xff, 0x00, 0x00, 0x02, 0x00, 0x20, 0x70,
	0x79, 0xff, 0x00, 0x00, 0x80, 0x00, 0x08, 0x14,
	0x2a, 0xff, 0x00, /* fade[0] */
};

int main(void)
{
	uint8_t phase = 255, lastphase, led, ledix=LED_OFF, pwm = 0, i=0, output, high;

	for (;; phase += 2)
	{
		for (led=0; led<8; led++)
		{
			for ( ; pwm>lastphase ; pwm+=2 )
			{
				if ( (1<<led) & fade[i+0] )
					ledix = ledix;
				else
					ledix = led;

				output = grid_output[ledix];
				high = grid_high[ledix];
			}

			DDRB  = output;
			PORTB = high;
			ledix = LED_OFF;

			pwm = 0;
			lastphase = phase;

			for ( ; pwm<phase ; pwm+=2 )
			{
				if ( (1<<led) & fade[i+1] )
					ledix = ledix;
				else
					ledix = led;

				output = grid_output[ledix];
				high = grid_high[ledix];
			}

			DDRB  = output;
			PORTB = high;
			ledix = LED_OFF;
		}

		if ( phase == 0 )
			i++;

		if (i==sizeof(fade)-1)
			i=0;
	}
}

