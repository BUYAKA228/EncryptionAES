#include "AES.h"
#include <stdio.h>

static void KeyExpansion(const uint8_t Key[], uint8_t ExpKey[] );
	static void KeyExpansionCore(uint8_t* In, uint8_t i);
		//static void SubBytes(uint8_t TransText[]);
		//	static uint8_t* Sbox_Generator(uint8_t* Buffer, uint16_t Polynomyal);
		//static void ShiftRows(uint8_t TransText[]);
		//	static void ShiftCount(uint8_t* Index, uint8_t CounterOfShift);


static void CommonRound(const uint8_t PlainText[], const uint8_t Key[], uint8_t TransText[]);
	static void SubBytes(uint8_t TransText[]);
		static uint8_t* Sbox_Generator(uint8_t* Buffer, uint16_t Polynomyal);
	static void ShiftRows(uint8_t TransText[]);
		static void ShiftCount(uint8_t* Index, uint8_t CounterOfShift);
	static void MixColumns(uint8_t TransText[]);
		static uint8_t GFmul_AES(uint8_t Byte, uint8_t multiplier);
	static void AddRoundKey(const uint8_t PlainText[],const uint8_t Key[], uint8_t TransText[]);


static void KeyExpansionCore(uint8_t* In, uint8_t i)
{
	uint8_t Rcon[256] = {
					0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 
					0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 
					0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
					0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
					0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
					0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
					0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
					0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
					0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
					0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
					0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
					0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
					0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
					0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
					0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
					0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
				};
	
	ShiftCount(In, 1);
	uint8_t Sbox[256]={0};
	Sbox_Generator(Sbox, POLYNOMIAL_X8_X4_X3_X1_X0);
	In[0] = Sbox[In[0]];
	In[1] = Sbox[In[1]];
	In[2] = Sbox[In[2]];
	In[3] = Sbox[In[3]];
	
	In[0] ^= Rcon[i];
	
}


void KeyExpansion(const uint8_t Key[], uint8_t ExpKey[] )
{
	for(int i = 0; i<16; i++)
		ExpKey[i] = Key[i];
	
	uint8_t bytesGenerated = 16;
	uint8_t CountOfRounds = 11;
	uint8_t RconIteration = 1;
	uint8_t temp[4];
	
	while(bytesGenerated <bytesGenerated*CountOfRounds)
	{
		for(int i =0; i<4;i++)
			temp[i] = ExpKey[i+bytesGenerated-4];
		if(bytesGenerated%16 == 0)
		{
			KeyExpansionCore(temp, RconIteration);
			RconIteration++;
		}
		for(int i = 0; i<4; i++)
		{
			ExpKey[bytesGenerated] = ExpKey[bytesGenerated-16]^temp[i];
			bytesGenerated++;
		}
	}
}



void Encryption_AES128(const uint8_t PlainText[],const uint8_t Key[], uint8_t TransText[])
{
	uint8_t ExpKey[176] = {0};
	
	KeyExpansion(Key, ExpKey);
	
	for(int i =0;i<sizeof(ExpKey);i++)
		if(i%4)
			printf("\t%X ",ExpKey[i]);
		else
			printf("\n\t%X ",ExpKey[i]);
	
	
	AddRoundKey(PlainText, Key, TransText);
	uint8_t IntermedianText[16] = {0};
	CommonRound(TransText, TransText, IntermedianText);
}


void CommonRound(const uint8_t PlainText[],const uint8_t Key[], uint8_t TransText[])
{
	uint8_t IntermedianText[16] = {0};
	for(int i =0;i<16;i++)
		IntermedianText[i]= PlainText[i];
	SubBytes(IntermedianText);
	ShiftRows(IntermedianText);
	MixColumns(IntermedianText);
	//AddRoundKey(IntermedianText, Key, TransText);
	
}

void SubBytes(uint8_t TransText[])
{
	uint8_t SBox[256] = {0};
	Sbox_Generator(SBox, POLYNOMIAL_X8_X4_X3_X1_X0);
	
	for(int i = 0; i<16;i++)
		TransText[i] = SBox[TransText[i]];
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

void AddRoundKey(const uint8_t PlainText[],const uint8_t Key[], uint8_t TransText[])
{
	for(int i = 0; i<16; i++)
		TransText[i] = PlainText[i]^Key[i];
}

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







