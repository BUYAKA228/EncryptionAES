#include "AES.h"




static void CommonRound(const uint8_t PlainText[], const uint8_t Key[], uint8_t TransText[]);
	static void SubBytes(const uint8_t PlainText[], uint8_t TransText[]);
	static void ShiftRows(uint8_t TransText[]);
	static void MixColumns(uint8_t TransText[]);
	static void AddRoundKey(uint8_t PlainText[],const uint8_t Key[]);







void Encryption_AES128(const uint8_t PlainText[],const uint8_t Key[], uint8_t TransText[])
{
	
	CommonRound(PlainText, Key, TransText);
}


static void CommonRound(const uint8_t PlainText[],const uint8_t Key[], uint8_t TransText[])
{
	SubBytes(PlainText, TransText);
	ShiftRows(TransText);
	MixColumns(TransText);
	AddRoundKey(TransText, Key);
}

static void SubBytes(const uint8_t PlainText[], uint8_t TransText[])
{
	uint8_t* Sbox_Generator(uint8_t* Buffer, uint16_t Polynomyal)
	{
		uint8_t t[256]   ={0};
		for (int i = 0, x = 1; i < 256; i++) {
			t[i] = x;
			x ^= (x<<1)^((x>>7)*Polynomyal);
		}
		Buffer[0] = 0x63;
		for (int i = 0; i < 255; i ++) 
		{
			int x = t[255 - i];
			x |= x << 8;
			x ^= (x >> 4) ^ (x >> 5) ^ (x >> 6) ^ (x >> 7);
			Buffer[t[i]] = (x ^ 0x63) & 0xFF;
		}
		return Buffer;
	}
	uint8_t SBox[256] = {0};
	Sbox_Generator(SBox, POLYNOMIAL_X8_X4_X3_X1_X0);
	
	for(int i = 0; i<16;i++)
		TransText[i] = SBox[PlainText[i]];
}

static void ShiftRows(uint8_t TransText[])
{
	void ShiftCount(uint8_t* Index, uint8_t CounterOfShift)
	{
		while(CounterOfShift--)
		{
			*(Index+1) ^= *Index	; *Index     ^= *(Index+1)  ; *(Index+1) ^= *Index;
			*(Index+2) ^= *(Index+1); *(Index+1) ^= *(Index+2)	; *(Index+2) ^= *(Index+1);
			*(Index+3) ^= *(Index+2); *(Index+2) ^= *(Index+3)  ; *(Index+3) ^= *(Index+2);
		}
	}
	
	ShiftCount(&TransText[0] , 0);
	ShiftCount(&TransText[4] , 1);
	ShiftCount(&TransText[8] , 2);
	ShiftCount(&TransText[12], 3);
}

static void MixColumns(uint8_t TransText[])
{
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

static void AddRoundKey(uint8_t PlainText[],const uint8_t Key[])
{
	for(int i = 0; i<16; i++)
		PlainText[i] = PlainText[i] ^ Key[i];
}








