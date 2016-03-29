/*---------------------------------------------------------------------------
 *
 *      arrows_bmp.c
 *
 *---------------------------------------------------------------------------*/

/* Declarations for this module */
#include "middleware/display/arrows_bmp.h"

// Up arrow
const unsigned char up_arrow[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFC, 0xF8,
                                   0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0C,
                                   0x0E, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                   0x0F, 0x0E, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x3F,
                                   0x1F, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00 };

// Up arrow hover
const unsigned char up_arrow_hover[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x03,
                                         0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                         0xF7, 0xF3, 0xF1, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0xF0, 0xF1, 0xF3, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                         0xFF, 0xFF, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0,
                                         0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

// Down arrow
const unsigned char down_arrow[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xE0, 0xE0, 0xE0,
                                     0xF0, 0xF8, 0xFC, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x30,
                                     0x70, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                     0xF0, 0x70, 0x30, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03,
                                     0x07, 0x0F, 0x1F, 0x3F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00 };

// Down arrow hover
const unsigned char down_arrow_hover[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x1F, 0x1F,
                                           0x1F, 0x0F, 0x07, 0x03, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                           0xEF, 0xCF, 0x8F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x0F, 0x8F, 0xCF, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                           0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC,
                                           0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

// Left arrow
const unsigned char left_arrow[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xC0, 0xC0,
                                     0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x40, 0x00, 0x00, 0x00, 0x18, 0x3C,
                                     0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                     0xFF, 0xC3, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03,
                                     0x07, 0x0F, 0x1F, 0x3F, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
                                     0x02, 0x00 };

// Left arrow hover
const unsigned char left_arrow_hover[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x3F,
                                           0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0xBF, 0xFF, 0xFF, 0xFF,
                                           0xE7, 0xC3, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x3C, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                           0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
                                           0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFD, 0xFF };

// Right arrow
const unsigned char right_arrow[] = { 0x00, 0x40, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC,
                                      0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x81, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                      0xFF, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x02, 0x03, 0x03,
                                      0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3F, 0x1F, 0x0F, 0x07, 0x03,
                                      0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Right arrow hover
const unsigned char right_arrow_hover[] = { 0xFF, 0xBF, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
                                            0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0x7E, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0xC3, 0xE7, 0xFF, 0xFF,
                                            0xFF, 0xFD, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
                                            0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
