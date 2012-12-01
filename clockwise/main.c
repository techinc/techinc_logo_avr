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

const uint8_t grid_output[16] =
{
	(ORANGE|BLUE),     (BLUE|ORANGE), /* clockwise, starting north */
	(GREEN|BLUE),       (BLUE|GREEN),
	(GREEN|PURPLE),   (PURPLE|GREEN),
	(ORANGE|PURPLE), (PURPLE|ORANGE),

	(ORANGE|BLUE),     (BLUE|ORANGE), /* repeated to prevent modulo */
	(GREEN|BLUE),       (BLUE|GREEN),  /* if statement in inner loop */
	(GREEN|PURPLE),   (PURPLE|GREEN),
	(ORANGE|PURPLE), (PURPLE|ORANGE),
};

const uint8_t grid_high[16] =
{
	(ORANGE),   (BLUE), /* clockwise, starting north */
	(GREEN),    (BLUE),
	(GREEN),  (PURPLE),
	(ORANGE), (PURPLE),

	(ORANGE),   (BLUE), /* repeated to prevent modulo */
	(GREEN),    (BLUE),  /* if statement in inner loop */
	(GREEN),  (PURPLE),
	(ORANGE), (PURPLE),
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
	__builtin_avr_delay_cycles(96);
}

int main(void)
{
	uint8_t led, i, j, k;

	for (;;)
		for (i=0 ; i<8; i++)
			for (j=0; j<4 ; j++)
				for (led=0; led<7; led++)
					for (k=0; k<(1<<led); k++)
						led_on(led+i);
}

