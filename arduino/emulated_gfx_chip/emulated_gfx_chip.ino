/*
 * Parallel interface controller for LED matrix displays, for use with 8-bit CPUs
 * 
 * Colin Maykish
 * June 5, 2021
 * 
 */

// NOTE: This will never keep up with a CPU running at full speed
// Options for increased speed:
// 1) Run this on a much faster microcontroller or an FPGA
// 2) Buffer the inputs and alert the CPU when the write is finished and just accept the slow speed

#include <RGBmatrixPanel.h>

// MCU interface pins
#define D0            14
#define D1            15
#define D2            16
#define D3            17
#define D4            18
#define D5            19
#define D6            20
#define D7            21
#define AD0           31  // TODO: this pin doesn't seem to do anything
#define AD1           33
#define AD2           35
#define AD3           37
#define AD4           39
#define AD5           41
#define AD6           43
#define AD7           45
#define AD8           47
#define AD9           49
#define AD10          51
#define RDY           2
#define CS1           4
#define CS2           5

// LED Panel controls
#define PANEL_CLK     11
#define PANEL_OE      9
#define PANEL_LAT     10
#define PANEL_A       A0
#define PANEL_B       A1
#define PANEL_C       A2
#define PANEL_D       A3

// LED array size
#define PANEL_WIDTH   64
#define PANEL_HEIGHT  32

// TODO: why doesn't double buffering work? out of memory?
RGBmatrixPanel matrix(PANEL_A, PANEL_B, PANEL_C, PANEL_D, PANEL_CLK, PANEL_LAT, PANEL_OE, false, 64);

uint8_t framebuffer[64][32] = {0};

uint16_t addr_in = 0;
uint8_t data_in = 0;

uint16_t last_addr_in = 0;
uint8_t last_data_in = 0;

void readBus()
{
  bool a0 = digitalRead(AD0);
  bool a1 = digitalRead(AD1);
  bool a2 = digitalRead(AD2);
  bool a3 = digitalRead(AD3);
  bool a4 = digitalRead(AD4);
  bool a5 = digitalRead(AD5);
  bool a6 = digitalRead(AD6);
  bool a7 = digitalRead(AD7);
  bool a8 = digitalRead(AD8);
  bool a9 = digitalRead(AD9);
  bool a10 = digitalRead(AD10);

  addr_in = a0;
  addr_in += (a1 << 1);
  addr_in += (a2 << 2);
  addr_in += (a3 << 3);
  addr_in += (a4 << 4);
  addr_in += (a5 << 5);
  addr_in += (a6 << 6);
  addr_in += (a7 << 7);
  addr_in += (a8 << 8);
  addr_in += (a9 << 9);
  addr_in += (a10 << 10);

  bool d0 = digitalRead(D0);
  bool d1 = digitalRead(D1);
  bool d2 = digitalRead(D2);
  bool d3 = digitalRead(D3);
  bool d4 = digitalRead(D4);
  bool d5 = digitalRead(D5);
  bool d6 = digitalRead(D6);
  bool d7 = digitalRead(D7);

  data_in = d0;
  data_in += d1 << 1;
  data_in += d2 << 2;
  data_in += d3 << 3;
  data_in += d4 << 4;
  data_in += d5 << 5;
  data_in += d6 << 6;
  data_in += d7 << 7;

}

void setup() {
  Serial.begin(115200);

  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);

  pinMode(AD0, INPUT);
  pinMode(AD1, INPUT);
  pinMode(AD2, INPUT);
  pinMode(AD3, INPUT);
  pinMode(AD4, INPUT);
  pinMode(AD5, INPUT);
  pinMode(AD6, INPUT);
  pinMode(AD7, INPUT);
  pinMode(AD8, INPUT);
  pinMode(AD9, INPUT);
  pinMode(AD10, INPUT);

  pinMode(RDY, OUTPUT);
  pinMode(CS1, INPUT);
  pinMode(CS2, INPUT);

  matrix.begin();
}

void loop() {
  if (digitalRead(CS1) && digitalRead(CS2))
  {
    last_addr_in = addr_in;
    last_data_in = data_in;
    
    // Chip select enabled
    readBus();

    if (last_addr_in != addr_in || last_data_in != data_in)
    {
      digitalWrite(RDY, LOW);
      
      bool highCommandBit = (data_in & 0b10000000) >> 7;
      bool lowCommandBit = (data_in & 0b01000000) >> 6;

      if (highCommandBit && lowCommandBit)
      {
        Serial.println("Clearing framebuffer");
        
        // Clear the framebuffer
        for (int i = 0; i < 64; i++)
        {
          for (int j = 0; j < 32; j++)
          {
            framebuffer[i][j] = 0;
          }
        }

        matrix.fillScreen(matrix.Color333(0, 0, 0));

        return;
      }
      
      uint8_t row = addr_in % 32;
      uint8_t column = addr_in / 32;
      
      Serial.print(addr_in, DEC);
      Serial.print(" | ");
      Serial.print("Row: ");
      Serial.print(row);
      Serial.print(", Col: ");
      Serial.print(column);
      Serial.print(" Color: ");
      Serial.print(data_in);
      Serial.println();

      // Store the pixel color from the input bus
      framebuffer[column][row] = data_in;

      // Redraw the framebuffer
      for (int i = 0; i < 64; i++)
      {
        for (int j = 0; j < 32; j++)
        {
          matrix.drawPixel(i, j,
            matrix.Color333(((framebuffer[i][j] & 0b110000) >> 4)* 2, 
                            ((framebuffer[i][j] & 0b1100) >> 2) * 2,
                            ((framebuffer[i][j] & 0b11))) * 2);
        }
      }

      digitalWrite(RDY, HIGH);
    }
  }
}
