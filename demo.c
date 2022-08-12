
#include <stdio.h>
#include <stdlib.h>
#include "AES.h"


void main()
{
    int ch;
	uint32_t count = 0;
	
	
	
	

	uint8_t Plain[16] = {0x54,0x4F,0x4E,0x20,
						 0x77,0x6E,0x69,0x54,
						 0x6F,0x65,0x6E,0x77,
	                     0x20,0x20,0x65,0x6F};
	uint8_t Key[16] = {0x54,0x73,0x20,0x67,
						 0x68,0x20,0x4B,0x20,
						 0x61,0x6D,0x75,0x46,
	                     0x74,0x79,0x6E,0x75};					 
						 
	uint8_t Trans[16]={0};
	
	
	Encryption_AES128( Plain, Key, Trans);
	
	for(int i =0;i<sizeof(Trans);i++)
		if(i%4)
			printf("\t%X ",Trans[i]);
		else
			printf("\n\t%X ",Trans[i]);
	
}
