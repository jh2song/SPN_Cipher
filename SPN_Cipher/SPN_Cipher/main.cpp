#include <cstdio>
#include <iostream>
#include "SPN.h"
using namespace std;

int main()
{
	
	printf("S-Box�� ����� ���� ��ӵ� ������ ���� 32��Ʈ�� ������\n");
	printf("XOR�� SubBytes��Ų C++�� ������ ������ �˰���\n");
	printf("\n\n");
	printf("\t\t\t��ǻ�Ͱ��а� 20183016 ������\n");
	
	SPN spn;
	spn.TextGetAndShow();
	spn.getPicture();
	spn.KeyGet();
	printf("��ȣȭ(1) �Ͻðڽ��ϱ�? ��ȣȭ(2) �Ͻðڽ��ϱ�? >> ");
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
		printf("�߸��� �Է��Դϴ�.\n");
		exit(1);
	}
	spn.PrintInConAndFile();
}