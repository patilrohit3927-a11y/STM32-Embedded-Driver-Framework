#include "oled.h"
#include "../i2c/i2c.h"

#define OLED_ADDRESS        0x3CU
#define OLED_WIDTH          128U
#define OLED_HEIGHT         64U
#define OLED_PAGES          8U

#define OLED_CONTROL_CMD    0x00U
#define OLED_CONTROL_DATA   0x40U

static uint8_t oled_buffer[OLED_WIDTH * OLED_PAGES];


/* ============================================================
 * Delay
 * ============================================================ */

static void OLED_Delay(volatile uint32_t count)
{
    while (count--)
    {
        __asm volatile ("nop");
    }
}


/* ============================================================
 * Send command
 * ============================================================ */

static void OLED_Command(uint8_t command)
{
    uint8_t packet[2];

    packet[0] = OLED_CONTROL_CMD;
    packet[1] = command;

    (void)I2C_Write(OLED_ADDRESS, packet, 2U);
}


/* ============================================================
 * OLED Initialization
 * ============================================================ */

void OLED_Init(void)
{
    OLED_Delay(1000000U);

    OLED_Command(0xAEU);       /* Display OFF */

    OLED_Command(0xD5U);
    OLED_Command(0x80U);       /* Display clock */

    OLED_Command(0xA8U);
    OLED_Command(0x3FU);       /* Multiplex ratio 64 */

    OLED_Command(0xD3U);
    OLED_Command(0x00U);       /* Display offset */

    OLED_Command(0x40U);       /* Start line */

    OLED_Command(0x8DU);
    OLED_Command(0x14U);       /* Charge pump */

    OLED_Command(0x20U);
    OLED_Command(0x00U);       /* Horizontal addressing mode */

    OLED_Command(0xA1U);       /* Segment remap */

    OLED_Command(0xC8U);       /* COM scan direction */

    OLED_Command(0xDAU);
    OLED_Command(0x12U);       /* COM pins */

    OLED_Command(0x81U);
    OLED_Command(0x7FU);       /* Contrast */

    OLED_Command(0xA4U);       /* Display follows RAM */

    OLED_Command(0xA6U);       /* Normal display */

    OLED_Command(0x2EU);       /* Deactivate scroll */

    OLED_Command(0xAFU);       /* Display ON */

    OLED_Clear();
}


/* ============================================================
 * Clear framebuffer
 * ============================================================ */

void OLED_Clear(void)
{
    uint16_t i;

    for (i = 0U; i < sizeof(oled_buffer); i++)
    {
        oled_buffer[i] = 0U;
    }
}


/* ============================================================
 * Set pixel
 * ============================================================ */

void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t state)
{
    uint16_t index;
    uint8_t mask;

    if (x >= OLED_WIDTH)
    {
        return;
    }

    if (y >= OLED_HEIGHT)
    {
        return;
    }

    index = ((uint16_t)(y / 8U) * OLED_WIDTH) + x;

    mask = (uint8_t)(1U << (y % 8U));

    if (state != 0U)
    {
        oled_buffer[index] |= mask;
    }
    else
    {
        oled_buffer[index] &= (uint8_t)~mask;
    }
}


/* ============================================================
 * Update OLED from framebuffer
 * ============================================================ */

void OLED_Update(void)
{
    uint8_t page;
    uint8_t column;
    uint8_t packet[17];

    for (page = 0U; page < OLED_PAGES; page++)
    {
        OLED_Command((uint8_t)(0xB0U | page));

        OLED_Command(0x00U);
        OLED_Command(0x10U);

        for (column = 0U; column < OLED_WIDTH; column += 16U)
        {
            uint8_t i;

            packet[0] = OLED_CONTROL_DATA;

            for (i = 0U; i < 16U; i++)
            {
                packet[i + 1U] =
                    oled_buffer[
                        ((uint16_t)page * OLED_WIDTH)
                        + column
                        + i
                    ];
            }

            (void)I2C_Write(OLED_ADDRESS, packet, 17U);
        }
    }
}


/* ============================================================
 * 5x7 FONT
 *
 * Each character uses 5 columns.
 *
 * Bit 0 = top pixel
 * Bit 6 = bottom pixel
 *
 * One extra column is added as spacing.
 * ============================================================ */


/* -------------------------
 * Digits
 * ------------------------- */

static const uint8_t font_0[5] =
{
    0x3EU, 0x51U, 0x49U, 0x45U, 0x3EU
};

static const uint8_t font_1[5] =
{
    0x00U, 0x42U, 0x7FU, 0x40U, 0x00U
};

static const uint8_t font_2[5] =
{
    0x42U, 0x61U, 0x51U, 0x49U, 0x46U
};

static const uint8_t font_3[5] =
{
    0x21U, 0x41U, 0x45U, 0x4BU, 0x31U
};

static const uint8_t font_4[5] =
{
    0x18U, 0x14U, 0x12U, 0x7FU, 0x10U
};

static const uint8_t font_5[5] =
{
    0x27U, 0x45U, 0x45U, 0x45U, 0x39U
};

static const uint8_t font_6[5] =
{
    0x3CU, 0x4AU, 0x49U, 0x49U, 0x30U
};

static const uint8_t font_7[5] =
{
    0x01U, 0x71U, 0x09U, 0x05U, 0x03U
};

static const uint8_t font_8[5] =
{
    0x36U, 0x49U, 0x49U, 0x49U, 0x36U
};

static const uint8_t font_9[5] =
{
    0x06U, 0x49U, 0x49U, 0x29U, 0x1EU
};


/* -------------------------
 * Uppercase letters
 * ------------------------- */

static const uint8_t font_A[5] =
{
    0x7EU, 0x11U, 0x11U, 0x11U, 0x7EU
};

static const uint8_t font_B[5] =
{
    0x7FU, 0x49U, 0x49U, 0x49U, 0x36U
};

static const uint8_t font_C[5] =
{
    0x3EU, 0x41U, 0x41U, 0x41U, 0x22U
};

static const uint8_t font_D[5] =
{
    0x7FU, 0x41U, 0x41U, 0x22U, 0x1CU
};

static const uint8_t font_E[5] =
{
    0x7FU, 0x49U, 0x49U, 0x49U, 0x41U
};

static const uint8_t font_F[5] =
{
    0x7FU, 0x09U, 0x09U, 0x09U, 0x01U
};

static const uint8_t font_G[5] =
{
    0x3EU, 0x41U, 0x49U, 0x49U, 0x7AU
};

static const uint8_t font_H[5] =
{
    0x7FU, 0x08U, 0x08U, 0x08U, 0x7FU
};

static const uint8_t font_I[5] =
{
    0x00U, 0x41U, 0x7FU, 0x41U, 0x00U
};

static const uint8_t font_J[5] =
{
    0x20U, 0x40U, 0x41U, 0x3FU, 0x01U
};

static const uint8_t font_K[5] =
{
    0x7FU, 0x08U, 0x14U, 0x22U, 0x41U
};

static const uint8_t font_L[5] =
{
    0x7FU, 0x40U, 0x40U, 0x40U, 0x40U
};

static const uint8_t font_M[5] =
{
    0x7FU, 0x02U, 0x0CU, 0x02U, 0x7FU
};

static const uint8_t font_N[5] =
{
    0x7FU, 0x04U, 0x08U, 0x10U, 0x7FU
};

static const uint8_t font_O[5] =
{
    0x3EU, 0x41U, 0x41U, 0x41U, 0x3EU
};

static const uint8_t font_P[5] =
{
    0x7FU, 0x09U, 0x09U, 0x09U, 0x06U
};

static const uint8_t font_Q[5] =
{
    0x3EU, 0x41U, 0x51U, 0x21U, 0x5EU
};

static const uint8_t font_R[5] =
{
    0x7FU, 0x09U, 0x19U, 0x29U, 0x46U
};

static const uint8_t font_S[5] =
{
    0x46U, 0x49U, 0x49U, 0x49U, 0x31U
};

static const uint8_t font_T[5] =
{
    0x01U, 0x01U, 0x7FU, 0x01U, 0x01U
};

static const uint8_t font_U[5] =
{
    0x3FU, 0x40U, 0x40U, 0x40U, 0x3FU
};

static const uint8_t font_V[5] =
{
    0x1FU, 0x20U, 0x40U, 0x20U, 0x1FU
};

static const uint8_t font_W[5] =
{
    0x3FU, 0x40U, 0x38U, 0x40U, 0x3FU
};

static const uint8_t font_X[5] =
{
    0x63U, 0x14U, 0x08U, 0x14U, 0x63U
};

static const uint8_t font_Y[5] =
{
    0x07U, 0x08U, 0x70U, 0x08U, 0x07U
};

static const uint8_t font_Z[5] =
{
    0x61U, 0x51U, 0x49U, 0x45U, 0x43U
};


/* -------------------------
 * Lowercase letters
 * ------------------------- */

static const uint8_t font_a[5] =
{
    0x20U, 0x54U, 0x54U, 0x54U, 0x78U
};

static const uint8_t font_b[5] =
{
    0x7FU, 0x48U, 0x44U, 0x44U, 0x38U
};

static const uint8_t font_c[5] =
{
    0x38U, 0x44U, 0x44U, 0x44U, 0x20U
};

static const uint8_t font_d[5] =
{
    0x38U, 0x44U, 0x44U, 0x48U, 0x7FU
};

static const uint8_t font_e[5] =
{
    0x38U, 0x54U, 0x54U, 0x54U, 0x18U
};

static const uint8_t font_f[5] =
{
    0x08U, 0x7EU, 0x09U, 0x01U, 0x02U
};

static const uint8_t font_g[5] =
{
    0x0CU, 0x52U, 0x52U, 0x52U, 0x3EU
};

static const uint8_t font_h[5] =
{
    0x7FU, 0x08U, 0x04U, 0x04U, 0x78U
};

static const uint8_t font_i[5] =
{
    0x00U, 0x44U, 0x7DU, 0x40U, 0x00U
};

static const uint8_t font_j[5] =
{
    0x20U, 0x40U, 0x44U, 0x3DU, 0x00U
};

static const uint8_t font_k[5] =
{
    0x7FU, 0x10U, 0x28U, 0x44U, 0x00U
};

static const uint8_t font_l[5] =
{
    0x00U, 0x41U, 0x7FU, 0x40U, 0x00U
};

static const uint8_t font_m[5] =
{
    0x7CU, 0x04U, 0x18U, 0x04U, 0x78U
};

static const uint8_t font_n[5] =
{
    0x7CU, 0x08U, 0x04U, 0x04U, 0x78U
};

static const uint8_t font_o[5] =
{
    0x38U, 0x44U, 0x44U, 0x44U, 0x38U
};

static const uint8_t font_p[5] =
{
    0x7CU, 0x14U, 0x14U, 0x14U, 0x08U
};

static const uint8_t font_q[5] =
{
    0x08U, 0x14U, 0x14U, 0x18U, 0x7CU
};

static const uint8_t font_r[5] =
{
    0x7CU, 0x08U, 0x04U, 0x04U, 0x08U
};

static const uint8_t font_s[5] =
{
    0x48U, 0x54U, 0x54U, 0x54U, 0x20U
};

static const uint8_t font_t[5] =
{
    0x04U, 0x3FU, 0x44U, 0x40U, 0x20U
};

static const uint8_t font_u[5] =
{
    0x3CU, 0x40U, 0x40U, 0x20U, 0x7CU
};

static const uint8_t font_v[5] =
{
    0x1CU, 0x20U, 0x40U, 0x20U, 0x1CU
};

static const uint8_t font_w[5] =
{
    0x3CU, 0x40U, 0x30U, 0x40U, 0x3CU
};

static const uint8_t font_x[5] =
{
    0x44U, 0x28U, 0x10U, 0x28U, 0x44U
};

static const uint8_t font_y[5] =
{
    0x0CU, 0x50U, 0x50U, 0x50U, 0x3CU
};

static const uint8_t font_z[5] =
{
    0x44U, 0x64U, 0x54U, 0x4CU, 0x44U
};


/* -------------------------
 * Punctuation
 * ------------------------- */

static const uint8_t font_space[5] =
{
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U
};

static const uint8_t font_colon[5] =
{
    0x00U, 0x36U, 0x36U, 0x00U, 0x00U
};

static const uint8_t font_dot[5] =
{
    0x00U, 0x60U, 0x60U, 0x00U, 0x00U
};

static const uint8_t font_dash[5] =
{
    0x08U, 0x08U, 0x08U, 0x08U, 0x08U
};

static const uint8_t font_percent[5] =
{
    0x63U, 0x13U, 0x08U, 0x64U, 0x63U
};

static const uint8_t font_slash[5] =
{
    0x20U, 0x10U, 0x08U, 0x04U, 0x02U
};


/* ============================================================
 * Get font column
 * ============================================================ */

static uint8_t OLED_GetFontColumn(char c, uint8_t column)
{
    if (column >= 5U)
    {
        return 0U;
    }

    switch (c)
    {
        /* Digits */
        case '0': return font_0[column];
        case '1': return font_1[column];
        case '2': return font_2[column];
        case '3': return font_3[column];
        case '4': return font_4[column];
        case '5': return font_5[column];
        case '6': return font_6[column];
        case '7': return font_7[column];
        case '8': return font_8[column];
        case '9': return font_9[column];

        /* Uppercase */
        case 'A': return font_A[column];
        case 'B': return font_B[column];
        case 'C': return font_C[column];
        case 'D': return font_D[column];
        case 'E': return font_E[column];
        case 'F': return font_F[column];
        case 'G': return font_G[column];
        case 'H': return font_H[column];
        case 'I': return font_I[column];
        case 'J': return font_J[column];
        case 'K': return font_K[column];
        case 'L': return font_L[column];
        case 'M': return font_M[column];
        case 'N': return font_N[column];
        case 'O': return font_O[column];
        case 'P': return font_P[column];
        case 'Q': return font_Q[column];
        case 'R': return font_R[column];
        case 'S': return font_S[column];
        case 'T': return font_T[column];
        case 'U': return font_U[column];
        case 'V': return font_V[column];
        case 'W': return font_W[column];
        case 'X': return font_X[column];
        case 'Y': return font_Y[column];
        case 'Z': return font_Z[column];

        /* Lowercase */
        case 'a': return font_a[column];
        case 'b': return font_b[column];
        case 'c': return font_c[column];
        case 'd': return font_d[column];
        case 'e': return font_e[column];
        case 'f': return font_f[column];
        case 'g': return font_g[column];
        case 'h': return font_h[column];
        case 'i': return font_i[column];
        case 'j': return font_j[column];
        case 'k': return font_k[column];
        case 'l': return font_l[column];
        case 'm': return font_m[column];
        case 'n': return font_n[column];
        case 'o': return font_o[column];
        case 'p': return font_p[column];
        case 'q': return font_q[column];
        case 'r': return font_r[column];
        case 's': return font_s[column];
        case 't': return font_t[column];
        case 'u': return font_u[column];
        case 'v': return font_v[column];
        case 'w': return font_w[column];
        case 'x': return font_x[column];
        case 'y': return font_y[column];
        case 'z': return font_z[column];

        /* Punctuation */
        case ' ': return font_space[column];
        case ':': return font_colon[column];
        case '.': return font_dot[column];
        case '-': return font_dash[column];
        case '%': return font_percent[column];
        case '/': return font_slash[column];

        default:
            return font_space[column];
    }
}


/* ============================================================
 * Write one character
 * ============================================================ */

void OLED_WriteChar(uint8_t x, uint8_t y, char c)
{
    uint8_t column;
    uint8_t row;
    uint8_t font_column;

    if (x >= OLED_WIDTH)
    {
        return;
    }

    if (y >= OLED_HEIGHT)
    {
        return;
    }

    for (column = 0U; column < 5U; column++)
    {
        font_column = OLED_GetFontColumn(c, column);

        for (row = 0U; row < 7U; row++)
        {
            if (font_column & (uint8_t)(1U << row))
            {
                OLED_SetPixel(
                    (uint8_t)(x + column),
                    (uint8_t)(y + row),
                    1U
                );
            }
            else
            {
                OLED_SetPixel(
                    (uint8_t)(x + column),
                    (uint8_t)(y + row),
                    0U
                );
            }
        }
    }

    /* Character spacing column */
    for (row = 0U; row < 7U; row++)
    {
        OLED_SetPixel(
            (uint8_t)(x + 5U),
            (uint8_t)(y + row),
            0U
        );
    }
}


/* ============================================================
 * Write string
 * ============================================================ */

void OLED_WriteString(uint8_t x, uint8_t y, const char *str)
{
    uint8_t position;

    if (str == 0)
    {
        return;
    }

    position = x;

    while (*str != '\0')
    {
        if ((uint16_t)position + 5U >= OLED_WIDTH)
        {
            break;
        }

        OLED_WriteChar(position, y, *str);

        position = (uint8_t)(position + 6U);

        str++;
    }
}