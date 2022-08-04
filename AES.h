#include <stdint.h>




void SubBytes(const uint8_t PlainText[], uint8_t TransText[], const uint8_t Sbox[]);
void ShiftRows(uint8_t TransText[]);
void MixColumns(uint8_t TransText[]);

uint8_t GFmul_AES(uint8_t Byte, uint8_t multiplier);
void    ShiftCount(uint8_t* Index, uint8_t CounterOfShift);