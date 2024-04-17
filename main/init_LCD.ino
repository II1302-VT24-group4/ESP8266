/*
 * Portions of this code are derived from the original work of Newhaven Display International.
 * Original Copyright (c) 2019, Newhaven Display International.
 * Licensed under GNU General Public License.
 */

void init_LCD()
{
  comm_write(0xA0); // ADC select
  comm_write(0xAE); // Display OFF
  comm_write(0xC8); // COM direction scan
  comm_write(0xA2); // LCD bias set
  comm_write(0x2F); // Power Control set
  comm_write(0x26); // Resistor Ratio Set
  comm_write(0x81); // Electronic Volume Command (set contrast) Double Byte: 1 of 2
  comm_write(0x11); // Electronic Volume value (contrast value) Double Byte: 2 of 2
  comm_write(0xAF); // Display ON
}