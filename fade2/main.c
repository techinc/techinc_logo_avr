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

const uint8_t grid_output[] =
{
	(ORANGE|BLUE),     (BLUE|ORANGE), /* clockwise, starting north */
	(GREEN|BLUE),       (BLUE|GREEN),
	(GREEN|PURPLE),   (PURPLE|GREEN),
	(ORANGE|PURPLE), (PURPLE|ORANGE),
};

const uint8_t grid_high[] =
{
	(ORANGE),   (BLUE), /* clockwise, starting north */
	(GREEN),    (BLUE),
	(GREEN),  (PURPLE),
	(ORANGE), (PURPLE),
};

const uint8_t fade[] =
{
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8, 8, 9, 10, 11, 12, 14, 15, 16, 17, 19, 20, 22, 24, 25, 27, 29, 31, 33, 35, 37, 40, 42, 44, 47, 49, 52, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 91, 94, 97, 101, 104, 108, 111, 115, 118, 122, 125, 129, 133, 136, 140, 143, 147, 151, 154, 158, 161, 165, 168, 172, 175, 179, 182, 186, 189, 192, 195, 198, 202, 205, 208, 211, 213, 216, 219, 221, 224, 226, 229, 231, 233, 235, 237, 239, 241, 243, 244, 246, 247, 249, 250, 251, 252, 253, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 253, 253, 252, 251, 250, 249, 247, 246, 244, 243, 241, 239, 237, 235, 233, 231, 229, 226, 224, 221, 219, 216, 213, 211, 208, 205, 202, 198, 195, 192, 189, 186, 182, 179, 175, 172, 168, 165, 161, 158, 154, 151, 147, 143, 140, 136, 133, 129, 125, 122, 118, 115, 111, 108, 104, 101, 97, 94, 91, 87, 84, 81, 78, 75, 72, 69, 66, 63, 60, 57, 54, 52, 49, 47, 44, 42, 40, 37, 35, 33, 31, 29, 27, 25, 24, 22, 20, 19, 17, 16, 15, 14, 12, 11, 10, 9, 8, 8, 7, 6, 5, 5, 4, 4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
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

	for ( phase = 0 ;; phase++ )
	{
		for ( i=0, led=phase; i<8; i++)
		{
			led_on(i);
			for (pwm = 0; pwm < fade[led]; pwm++);
			led += 32;
		}
	}
}

