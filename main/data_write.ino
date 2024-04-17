/*
 * Portions of this code are derived from the original work of Newhaven Display International.
 * Original Copyright (c) 2019, Newhaven Display International.
 * Licensed under GNU General Public License.
 */

void data_write(unsigned char d) // Data Output Serial Interface
{
  unsigned int n;
  digitalWrite(CS, LOW);
  digitalWrite(RS, HIGH);
  for (n = 0; n < 8; n++)
  {
    if ((d & 0x80) == 0x80)
      digitalWrite(SI, HIGH);
    else
      digitalWrite(SI, LOW);
    while (0);
    d = (d << 1);
    digitalWrite(SC, LOW);
    while (0);
    digitalWrite(SC, HIGH);
    while (0);
    digitalWrite(SC, LOW);
  }
  digitalWrite(CS, HIGH);
}