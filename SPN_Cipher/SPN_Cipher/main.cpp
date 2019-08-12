#include <cstdio>
#include <iostream>
#include "SPN.h"
using namespace std;

int main()
{
	
	printf("S-Box를 대신해 사전 약속된 사진의 상위 32비트를 추출해\n");
	printf("XOR로 SubBytes시킨 C++로 개발한 독자적 알고리즘\n");
	printf("\n\n");
	printf("\t\t\t컴퓨터공학과 20183016 송지훈\n");
	
	SPN spn;
	spn.TextGetAndShow();
	spn.getPicture();
	spn.KeyGet();
	printf("암호화(1) 하시겠습니까? 복호화(2) 하시겠습니까? >> ");
	int swi;

	cin >> swi;
	switch (swi)
	{
	case 1:
		for (unsigned int i = 0; i < 4; i++)
		{
			spn.Enc();
		}
		break;
	case 2:
		for (unsigned int i = 0; i < 4; i++)
		{
			spn.Dec(i);
		}
		break;
	default:
		printf("잘못된 입력입니다.\n");
		exit(1);
	}
	spn.PrintInConAndFile();
}