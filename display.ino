/*****************************************************************************
 *
/ Program for writing to NHD-C12864A1Z display Series with the ST7565P Controller.
/ This code is written for the Arduino Uno R3 using Serial Interface
/
/ Newhaven Display invests time and resources providing this open source code,
/ Please support Newhaven Display by purchasing products from Newhaven Display!

* Copyright (c) 2019, Newhaven Display International
*
* This code is provided as an example only and without any warranty by Newhaven Display.
* Newhaven Display accepts no responsibility for any issues resulting from its use.
* The developer of the final application incorporating any parts of this
* sample code is responsible for ensuring its safe and correct operation
* and for any consequences resulting from its use.
* See the GNU General Public License for more details.
*
* Use Vertical Orientation when converting BMP to hex code to display 
* custom image using LCD assistant.
*
*****************************************************************************/

/****************************************************
 *               Hex Table for NHD Pic               
 *****************************************************/

unsigned char NHD[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x1E, 0x1E, 0x3F, 0x3F, 0x1E, 0x04, 0x0C, 0x8C, 0x4C,
    0x4C, 0x2C, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x4C, 0x00, 0xFC, 0x1C, 0x38, 0xC0, 0xFC, 0xFC, 0x00,
    0x00, 0x00, 0xF8, 0xFC, 0x24, 0x24, 0x24, 0x24, 0x00, 0x00, 0x00, 0x1C, 0xF8, 0x80, 0xE0, 0x3C,
    0x3C, 0xE0, 0x80, 0xE0, 0x3C, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0x20, 0x20, 0x20, 0xFC, 0x00, 0x00,
    0x00, 0x00, 0xE0, 0xFC, 0xCC, 0xFC, 0xE0, 0x80, 0x00, 0x00, 0x04, 0x3C, 0xF0, 0x80, 0xC0, 0x78,
    0x1C, 0x04, 0x00, 0x60, 0xF8, 0xAC, 0x24, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x1C, 0x38,
    0xE0, 0x80, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0x40,
    0x86, 0x8F, 0x9F, 0x9F, 0x9F, 0x83, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0xC2, 0x63, 0x39, 0x0C,
    0x04, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x00, 0xFB, 0x18, 0x18, 0x18, 0x3B, 0xF3, 0x00,
    0x00, 0x00, 0xF9, 0xF9, 0x03, 0x03, 0x03, 0x73, 0x78, 0x58, 0x58, 0xD8, 0xD9, 0x83, 0x03, 0x00,
    0x00, 0xFB, 0xFB, 0x99, 0x98, 0xD8, 0xF8, 0x20, 0x03, 0x03, 0xF8, 0xF8, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x03, 0x81, 0xE0, 0xB8, 0xB8, 0xE1, 0x83, 0x02, 0x00, 0x00, 0x18, 0x30, 0xC3, 0xE3, 0x38,
    0x18, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0x06, 0x06, 0x03, 0x03, 0x00,
    0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 0x02, 0x06, 0x06, 0x06, 0x06, 0x03, 0x03, 0x00, 0x00,
    0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x06, 0x06, 0x00,
    0x00, 0x04, 0x07, 0x01, 0x01, 0x01, 0x01, 0x07, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x03, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/****************************************************      
 *****************************************************/

/*    Pinout Table    
  Display:                          NodeMCU:
  SCL (Serial Clock)----------------D5 (GPIO14)
  SI (Serial Data input)------------D7 (GPIO13)
  A0 (Register Select)--------------D1 (GPIO5)
  /RESET (Reset signal)-------------D2 (GPIO4)
  /CS (Chip Select Signal)----------D8 (GPIO15)
  VDD (Supply Voltage +3.0V)--------3V3
  VSS (Ground)----------------------GND
  LED- (Backlight Cathode)----------GND
  LED+ (Backlight Anode)------------3V3
*/

#define RES D1 // /RES (Reset signal) connected to D2 (GPIO4)
#define CS D8 // /CS (Chip Select Signal) connected to D8 (GPIO15)
#define RS D1 // RS (Register Select) signal connected to D1 (GPIO5)
#define SC D5  // SCL (serial mode) signal connected to D5 (GPIO14)
#define SI D7 // SDI (serial mode) signal connected to D7 (GPIO13)

/****************************************************
 *                 Function Commands                 
 *****************************************************/

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

void comm_write(unsigned char d) // Command Output Serial Interface
{
  unsigned int n;
  digitalWrite(CS, LOW);
  digitalWrite(RS, LOW);
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

void DispPic(unsigned char *lcd_string)
{
  unsigned int i, j;
  unsigned char page = 0xB0;
  comm_write(0xAE); // Display OFF
  comm_write(0x40); // Display start address + 0x40
  for (i = 0; i < 8; i++)
  {                   // 64 pixel display / 8 pixels per page = 8 pages
    comm_write(page); // send page address
    comm_write(0x10); // column address upper 4 bits + 0x10
    comm_write(0x00); // column address lower 4 bits + 0x00
    for (j = 0; j < 128; j++)
    {                          // 128 columns wide
      data_write(*lcd_string); // send picture data
      lcd_string++;
    }
    page++; // after 128 columns, go to next page
  }
  comm_write(0xAF);
}

void ClearLCD(unsigned char *lcd_string)
{
  unsigned int i, j;
  unsigned char page = 0xB0;
  comm_write(0xAE); // Display OFF
  comm_write(0x40); // Display start address + 0x40
  for (i = 0; i < 8; i++)
  {                   // 64 pixel display / 8 pixels per page = 8 pages
    comm_write(page); // send page address
    comm_write(0x10); // column address upper 4 bits + 0x10
    comm_write(0x00); // column address lower 4 bits + 0x00
    for (j = 0; j < 128; j++)
    {                   // 128 columns wide
      data_write(0x00); // write clear pixels
      lcd_string++;
    }
    page++; // after 128 columns, go to next page
  }
  comm_write(0xAF);
}

/****************************************************
 *           Initialization For controller           
 *****************************************************/

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

/*****************************************************
 *           Setup Function, to run once              
 *****************************************************/

void setup()
{
  //DDRD = 0xFF;          // configure PORTD as output
  pinMode(RES, OUTPUT); // configure RES as output
  pinMode(CS, OUTPUT);  // configure CS as output
  pinMode(RS, OUTPUT);  // configure RS as output
  pinMode(SC, OUTPUT);  // configure SC as output
  pinMode(SI, OUTPUT);  // configure SI as output
  digitalWrite(RES, LOW);
  delay(100);
  digitalWrite(RES, HIGH);
  delay(100);
}

/*****************************************************
 *           Loop Function, to run repeatedly         
 *****************************************************/

void loop()
{
  delay(10);
  init_LCD();
  while (1)
  {
    ClearLCD(NHD); // clear LCD
    delay(1000);
    DispPic(NHD); // Show Image
    delay(3000);
    comm_write(0xA5); // Turn all points ON
    delay(1000);
    comm_write(0xA4); // Revert to Normal Display
    delay(1000);
  }
}