/*
 * Copyright (c) 2012 Erik Bosman
 *
 * Permission  is  hereby  granted,  free  of  charge,  to  any  person
 * obtaining  a copy  of  this  software  and  associated documentation
 * files (the "Software"),  to deal in the Software without restriction,
 * including  without  limitation  the  rights  to  use,  copy,  modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the
 * Software,  and to permit persons to whom the Software is furnished to
 * do so, subject to the following conditions:
 *
 * The  above  copyright  notice  and this  permission  notice  shall be
 * included  in  all  copies  or  substantial portions  of the Software.
 *
 * THE SOFTWARE  IS  PROVIDED  "AS IS", WITHOUT WARRANTY  OF ANY KIND,
 * EXPRESS OR IMPLIED,  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY,  FITNESS  FOR  A  PARTICULAR  PURPOSE  AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT,  TORT OR OTHERWISE,  ARISING FROM, OUT OF OR IN
 * CONNECTION  WITH THE SOFTWARE  OR THE USE  OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * (http://opensource.org/licenses/mit-license.html)
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay_basic.h>
#include <stdint.h>

#include <avr/pgmspace.h>

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
	 0, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 9, 9, 10,
	11, 12, 13, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 28, 29, 30, 32,
	34, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 58, 60, 63, 65, 68, 70, 73,
	76, 79, 82, 85, 88, 91, 94, 97, 101, 104, 108, 111, 115, 119, 123, 126,
	130, 135, 139, 143, 147, 152, 156, 161, 166, 170, 175, 180, 185, 191, 196,
	201, 207, 212, 218, 224, 229, 235, 241, 247, 254,
};

const uint8_t frames[] =
{
	0xff, 0x00, 0x03, 0x84, 0x48, 0x30, 0x30, 0x48, 0x84, 0x03, 0x03, 0x84, 0x48, 0x30, 0x30,
	0x60, 0xc0, 0x81, 0x03, 0x06, 0x0c, 0x18, 0x03,
	0x00, 0xff, 0x00, 0x01, 0x00, 0x10, 0x38,
	0x7c, 0xff, 0x00, 0x02, 0x00, 0x20, 0x70,
	0x79, 0xff, 0x00, 0x80, 0x00, 0x08, 0x14,
	0x2a, 0xff, 0x00, 0xff, /* frames[0] */
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
	uint8_t phase, frame, led, pwm, i, f[4];

	f[0] = 0; f[3] = 254;

	for (;;)
	{
		for (frame=1; frame<sizeof(frames); frame++)
			for (phase=0; phase<sizeof(fade); phase++)
			{
				f[1] = fade[sizeof(fade)-1-phase];
				f[2] = fade[phase];

				for (led=0; led<8; led++)
				{
					i=0;
					if ( (1<<led) & frames[frame-1] ) i++;
					if ( (1<<led) & frames[frame] ) i+=2;

					pwm = f[i];

					if (pwm)
						led_on(led);

					for (i=0; i<pwm; i+=2);

					led_on(LED_OFF);

					for (i+=2; i>pwm; i+=2);
				}
			}
	}
}

