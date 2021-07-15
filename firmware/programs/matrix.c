#include <stdlib.h>
#include "herring.h"
#include "via.h"
#include "acia.h"
#include "utils.h"

#define COMMAND VIA1_PORTA
#define DATA VIA1_PORTB

#define BUBBLE_COUNT 5

void Delay(word w)
{
    word i = 0;
    for (i; i < w; i++)
    {
        asm("nop");
    }
}

void DrawPixel(byte x, byte y)
{
    byte x_send = 0b10000000 | (x & 0b00111111);

    poke(COMMAND, x_send);
    poke(DATA, y & 0b00011111);

    // toggle interupt pin
    poke(COMMAND, x_send | 0b01000000);
    Delay(1);
    poke(COMMAND, x_send & 0b10111111);
    Delay(1);
}

void ClearScreen()
{
    poke(DATA, 0x0);
    poke(COMMAND, 0x0);

    // toggle interupt pin
    poke(COMMAND, 0b01000000);
    Delay(1);
    poke(COMMAND, 0x0);
    Delay(1);
}

void DrawSprite(byte x, byte y, byte sprite[8][8])
{
    byte i = 0;
    byte j = 0;

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (sprite[j][i])
            {
                DrawPixel(x + i, y + j);
            }
        }
    }
}

void DrawBubble(byte x, byte y)
{
    DrawPixel(x, y - 1);
    DrawPixel(x, y + 1);
    DrawPixel(x - 1, y);
    DrawPixel(x + 1, y);
}

int main()
{
    byte x = 0;
    byte y = 0;
    byte i = 0;

    byte bubbles[BUBBLE_COUNT][2];

    byte fish_sprite[8][8] = {{0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 1, 1, 1, 0, 0},
                              {1, 0, 1, 0, 0, 0, 1, 0},
                              {0, 1, 0, 0, 0, 1, 0, 1},
                              {1, 0, 1, 0, 0, 0, 0, 1},
                              {0, 0, 0, 1, 1, 1, 1, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0}};

    srand(6161);

    ACIA_Init();

    poke(VIA1_DDRA, 0xFF);
    poke(VIA1_DDRB, 0xFF);
    poke(COMMAND, 0x00);
    poke(DATA, 0x00);

    ClearScreen();

    ACIA_WriteBuffer("Ready to play!");
    ACIA_NewLine();

    // Create random bubbles
    for (i = 0; i < BUBBLE_COUNT; i++)
    {
        bubbles[i][0] = (rand() % 57) + 3;
        bubbles[i][1] = rand() % 27 + 2;
    }

    while (true)
    {
        // User input

        if ((peek(ACIA1_STATUS) & ACIA_READY_RX))
        {
            char c = ACIA_Read();

            if (c == 'w')
            {
                y--;
            }
            else if (c == 's')
            {
                y++;
            }
            else if (c == 'a')
            {
                x--;
            }
            else if (c == 'd')
            {
                x++;
            }
        }

        // Update game state
        for (i = 0; i < BUBBLE_COUNT; i++)
        {
            if (bubbles[i][1] == 30)
            {
                bubbles[i][1] = 1;
            }
            else
            {
                bubbles[i][1]--;
            }
        }

        // Redraw screen
        ClearScreen();
        DrawSprite(x, y, fish_sprite);

        // Draw some bubbles
        for (i = 0; i < BUBBLE_COUNT; i++)
        {
            DrawBubble(bubbles[i][0], bubbles[i][1]);
        }

        Delay(200);
    }

    return 0;
}
