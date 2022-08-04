#include "AES.h"

 
void SubBytes(const uint8_t PlainText[], uint8_t TransText[], const uint8_t Sbox[])
{
	for(int i = 0; i<16;i++)
		TransText[i] = Sbox[PlainText[i]];
}

void ShiftRows(uint8_t TransText[])
{
	ShiftCount(&TransText[0] , 0);
	ShiftCount(&TransText[4] , 1);
	ShiftCount(&TransText[8] , 2);
	ShiftCount(&TransText[12], 3);
}

void MixColumns(uint8_t TransText[])
{
	uint8_t SafeFixMatrix[4][4] =
	{{0x02,0x03,0x01,0x01},
	 {0x01,0x02,0x03,0x01},
	 {0x01,0x01,0x02,0x03},
	 {0x03,0x01,0x01,0x02}
	};
	uint8_t  Result[4][4] 	= {0};
	for(int rows = 0, indexPlain = 0; rows<4; rows++)
		for(int col = 0; col<4; col++,indexPlain++)
		{
			Result[col][rows]=TransText[indexPlain];
			TransText[indexPlain] = 0;
		}
	for(int col = 0, indexResult = 0; col<4; col++)
		for(int rows = 0; rows<4; rows++, indexResult++)
			for(int ElemOfXOR = 0; ElemOfXOR<4; ElemOfXOR++)
				TransText[indexResult]^=GFmul_AES(Result[rows][ElemOfXOR],SafeFixMatrix[col][ElemOfXOR]);
}


uint8_t GFmul_AES(uint8_t Byte, uint8_t multiplier)
{
	uint8_t result = 0;
	switch (multiplier)
	{
		case 0x01:
			result  ^= Byte;
			break;
		case 0x02:
			if(Byte<<1 > 0xFF)
				result  ^= (Byte<<1)^0x11b;
			else
				result  ^= (Byte<<1);
			break;
		case 0x03:
			if(Byte<<1 > 0xFF)
				result  ^= ((Byte<<1)^Byte)^0x11b;
			else
				result  ^= (Byte<<1)^Byte;
			break;
	}
	return result;
}

void ShiftCount(uint8_t* Index, uint8_t CounterOfShift)
{
	while(CounterOfShift--)
	{
		*(Index+1) ^= *Index	; *Index     ^= *(Index+1)  ; *(Index+1) ^= *Index;
		*(Index+2) ^= *(Index+1); *(Index+1) ^= *(Index+2)	; *(Index+2) ^= *(Index+1);
		*(Index+3) ^= *(Index+2); *(Index+2) ^= *(Index+3)  ; *(Index+3) ^= *(Index+2);
	}
}
