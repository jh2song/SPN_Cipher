#define _CRT_SECURE_NO_WARNINGS

#include "SPN.h"
#include <cstdio>
#include <sstream>
#include <iostream>
#include <fstream>

SPN::SPN()
{ 
	numOfBlock = 0; 
	Picture = 0;
	for (int i = 0; i < 1000; i++)
		for (int j = 0; j < 16; j++)
			text[i][j] = '\0';

	
}

void printLine()
{
	cout << "=======================================\n";
}

void SPN::TextGetAndShow()
{
	FILE *fptr;

	fptr = fopen("Text.bin", "rb");
	if (fptr == NULL)
	{
		printf("Cannot open PT\n");
		exit(1);
	}
	
	printLine();
	
	unsigned int length;

	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);

	unsigned char c;
	fread(&c, sizeof(char), 1, fptr);
	int count = 0;
	unsigned int i = 0;
	while (i != length)
	{
		if (count == 16)
		{
			numOfBlock++;
			count = 0;
		}
	
		text[numOfBlock][count++] = c;
		printf("%c", c);
		fread(&c, sizeof(char), 1, fptr);
		i++;
	}
	numOfBlock++;
	cout << "\n";
	printLine();
	fclose(fptr);
}

void SPN::getPicture()
{
	FILE *fptr;

	fptr = fopen("Picture.png", "rb");
	if (fptr == NULL)
	{
		printf("Cannot open Picture\n");
		exit(1);
	}
	
	char c;
	c = fgetc(fptr);
	Picture = c;
	c = fgetc(fptr);
	Picture = Picture * 256 + c;
	c = fgetc(fptr);
	Picture = Picture * 256 + c;
	c = fgetc(fptr);
	Picture = Picture * 256 + c;
	fclose(fptr);
}


//Key Generator
void SPN::KeyGet()
{
	// key : 64비트 8바이트
	cout << "키 범위 (-9,223,372,036,854,775,808 ~ 9,223,372,036,854,775,807)\n";
	cout << "키를 입력하세요 : ";
	cin >> key;
}
void SPN::KeyRightRotate(unsigned int n) { 
	key = (key >> n) | (key << (64 - n));
}
void SPN::KeyLeftRotate(unsigned int n) { 
	key = (key << n) | (key >> (64 - n));
}

//Enc
void SPN::Enc()
{
	KeyLeftRotate(1);
	for (unsigned int i = 0; i < numOfBlock; i++)
	{
		semiBlock_1 = semiBlock_2 = semiBlock_3 = semiBlock_4 = 0;
		merge64_1 = merge64_2 = 0;

		split32From128(text[i]);
		XOR32Picture();
		LeftShiftInEnc();
		Merge64From32();
		XOR64Key();


		Merge128From64(text[i]);
	}
}
void SPN::split32From128(unsigned char block[])
{
	for (int i = 0; i < 4; i++)
	{
		semiBlock_1 = semiBlock_1 << 8;
		semiBlock_1 += block[i];
		
		semiBlock_2 = semiBlock_2 << 8;
		semiBlock_2 += block[i + 4];
		
		semiBlock_3 = semiBlock_3 << 8;
		semiBlock_3 += block[i + 8];
		
		semiBlock_4 = semiBlock_4 << 8;
		semiBlock_4 += block[i + 12];
	}
}
void SPN::XOR32Picture()
{
	semiBlock_1 = semiBlock_1 ^ Picture;
	semiBlock_2 = semiBlock_2 ^ Picture;
	semiBlock_3 = semiBlock_3 ^ Picture;
	semiBlock_4 = semiBlock_4 ^ Picture;
}

void SPN::LeftShiftInEnc()
{
	int tmp = semiBlock_1;
	semiBlock_1 = semiBlock_2;
	semiBlock_2 = semiBlock_3;
	semiBlock_3 = semiBlock_4;
	semiBlock_4 = tmp;
}

void SPN::Merge64From32()
{
	merge64_1 = ((unsigned long long)semiBlock_1 << 32) + semiBlock_2;
	merge64_2 = ((unsigned long long)semiBlock_3 << 32) + semiBlock_4;
}
void SPN::XOR64Key()
{
	merge64_1 = merge64_1 ^ key;
	merge64_2 = merge64_2 ^ key;
}
void SPN::Merge128From64(unsigned char block[])
{
	for (int i = 7; i >= 0; i--)
	{
		block[i] = merge64_1 << (64-8) >> (64 - 8);
		merge64_1 = merge64_1 >> 8;
	}
	for (int i = 15; i >= 8; i--)
	{
		block[i] = merge64_2 << (64 - 8) >> (64 - 8);
		merge64_2 = merge64_2 >> 8;
	}
}

//Dec
void SPN::Dec(int round)
{
	if (round == 0)
		KeyRightRotate(60);
	else
		KeyRightRotate(1);

	for (int i = 0; i < (int)numOfBlock; i++)
	{
		semiBlock_1 = semiBlock_2 = semiBlock_3 = semiBlock_4 = 0;
		merge64_1 = merge64_2 = 0;

		split64From128(text[i]);

		XOR64Key();
		split32From64();
		RightShiftInDec();
		XOR32Picture();
		Merge128From32(text[i]);
	}
}
void SPN::split64From128(unsigned char block[])
{
	merge64_1 = merge64_2 = 0;
	for (int i = 0; i < 8; i++)
	{
		merge64_1 = merge64_1 << 8;
		merge64_1 = merge64_1 | block[i];

		merge64_2 = merge64_2 << 8;
		merge64_2 = merge64_2 | block[i + 8];
	}
}
void SPN::split32From64()
{
	semiBlock_2 = merge64_1 << 32 >> 32;
	semiBlock_1 = merge64_1 >> 32;

	semiBlock_4 = merge64_2 << 32 >> 32;
	semiBlock_3 = merge64_2 >> 32;
}

void SPN::RightShiftInDec()
{
	int tmp = semiBlock_4;
	semiBlock_4 = semiBlock_3;
	semiBlock_3 = semiBlock_2;
	semiBlock_2 = semiBlock_1;
	semiBlock_1 = tmp;
}

void SPN::Merge128From32(unsigned char block[])
{
	for (int i = 0; i < 4; i++)
	{
		block[i] = semiBlock_1 >> (32 - 8);
		semiBlock_1 = semiBlock_1 << 8;

		block[i + 4] = semiBlock_2 >> (32 - 8);
		semiBlock_2 = semiBlock_2 << 8;

		block[i + 8] = semiBlock_3 >> (32 - 8);
		semiBlock_3 = semiBlock_3 << 8;

		block[i + 12] = semiBlock_4 >> (32 - 8);
		semiBlock_4 = semiBlock_4 << 8;

	}
}

void SPN::PrintInConAndFile()
{
	FILE * fptr = fopen("Output.bin", "wb");

	printLine();
	if (fptr == NULL)
	{
		printf("Cannot Find File\n");
		exit(1);
	}
	
	unsigned char c;
	for (int i = 0; i < (int)numOfBlock; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			c = text[i][j];
			printf("%c", c);
			fwrite(&c, sizeof(char), 1, fptr);
		}
	}
	cout << endl;
	printLine();
	fclose(fptr);
}