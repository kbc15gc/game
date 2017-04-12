#pragma once
#include <fstream>
#include <string>
#include <iostream>

//���낢��֗��Ȋ֐�����낤
namespace Support
{
	//�����񂩂�n�b�V���l�����߂�(�Ȉ�)
	extern int MakeHash(const char * s);
	//�����𕶎����
	extern void ToString(const int& dnum, char* s);
	//�����𕶎���(wchar)��
	extern void ToString(const int& dnum, wchar_t* s);
	//�����𕶎����
	extern void ToString(const float& fnum, wchar_t * s, const int& decimal = 1);
	//CSV�t�@�C����ǂݍ���(������)
	extern void ReadCSV(const char * filename);
}