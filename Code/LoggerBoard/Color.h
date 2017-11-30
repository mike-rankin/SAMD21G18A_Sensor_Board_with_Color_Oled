#ifndef COLOR_H
#define COLOR_H

inline uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t c;
  c = r >> 3;
  c <<= 6;
  c |= g >> 2;
  c <<= 5;
  c |= b >> 3;

  return c;
}

const uint16_t COLOR_TEXT = 0;
const uint16_t COLOR_BG_1 = Color565(0xf5, 0xf5, 0xf6);
const uint16_t COLOR_CHROME = Color565(0x5d, 0x99, 0xc6);
const uint16_t COLOR_HEADER = Color565(0x90, 0xca, 0xf9);
const uint16_t COLOR_LIGHT = Color565(0xc3, 0xfd, 0xff);
const uint16_t COLOR_RED = Color565(0xff, 0x00, 0x00);

#endif
