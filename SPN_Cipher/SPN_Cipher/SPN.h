#ifndef __SPN_H__
#define __SPN_H__
#include <cstdio>
using namespace std;

class SPN {
	unsigned int numOfBlock;

	unsigned char text[1000][16];
	unsigned long long key;
	unsigned int Picture;

	unsigned int semiBlock_1, semiBlock_2, semiBlock_3, semiBlock_4;
	unsigned long long merge64_1, merge64_2;
public:
	SPN();
	
	void TextGetAndShow();
	void getPicture();

	//Key Generator
	void KeyGet();
	void KeyRightRotate(unsigned int n);
	void KeyLeftRotate(unsigned int n);

	//Enc
	void Enc();
	void split32From128(unsigned char text[]);
	void XOR32Picture();
	void LeftShiftInEnc();
	void Merge64From32();
	void XOR64Key();
	void Merge128From64(unsigned char block[]);

	//Dec
	void Dec(int round);
	void split64From128(unsigned char block[]);
	// void XOR64bitsKey(); reuse
	void split32From64();
	void RightShiftInDec();
	void Merge128From32(unsigned char block[]);


	void PrintInConAndFile();
};

#endif