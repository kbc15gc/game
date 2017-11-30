#include"fbstdafx.h"
#include "Support.h"

namespace Support
{
	int MakeHash(const char * s)
	{
		int hash = 0;
		while (*s != '\0')
		{
			//���Z���Ă���
			hash = hash * 37 + (*s);
			//���̕�����
			s++;
		}
		return hash;
	}

	void ToString(const int dnum, char* s)
	{
		list<char> l;
		//���ǃR�s�[���������
		int num = dnum;
		do
		{
			//�����R�[�h�ϊ�
			char d = (num % 10) + '0';
			//�o�^
			l.push_back(d);
			//���̌���
			num /= 10;
		} while (num != 0);

		//�t����
		list<char>::reverse_iterator it = l.rbegin();
		short i = 0;
		while (it != l.rend())
		{
			//�O������o��
			s[i] = (*it);
			it++;
			i++;
		}
		//�I�[����
		s[i] = '\0';
	}

	void ToString(const int dnum, wchar_t* s)
	{
		list<char> l;
		//���ǃR�s�[���������
		int num = dnum;
		do
		{
			//�����R�[�h�ϊ�
			char d = (num % 10) + '0';
			//�o�^
			l.push_back(d);
			//���̌���
			num /= 10;
		} while (num != 0);

		//�t����
		list<char>::reverse_iterator it = l.rbegin();
		short i = 0;
		while (it != l.rend())
		{
			//�O������o��
			s[i] = (*it);
			it++;
			i++;
		}
		//�I�[����
		s[i] = '\0';
	}

	void ToString(const float fnum, wchar_t * s, const int decimal)
	{
		short i = 0;
		int _decimal = decimal;
		if (fnum < 0.0f) {
			// �}�C�i�X�̒l�B
			s[0] = '-';
			i++;
			_decimal--;
		}
		else if (fabsf(fnum) <= 0.00001f) {
			// �ق�0�Ȃ̂�0.0f�����ĕԋp�B
			wcscpy_s(s,wcslen(L"0.0") + 1, L"0.0");
			return;
		}

		//�J��グ
		int num = (int)(fabsf(fnum) * pow(10, decimal));
		list<char> l;
		do
		{
			//�����R�[�h�ϊ�
			char d = (num % 10) + '0';
			//�o�^
			l.push_back(d);
			//���̌���
			num /= 10;
		} while (num != 0);

		size_t size = l.size();
		if (size == decimal) {
			// �����_�ȉ������ς܂�ĂȂ��B

			// ��int�^�͏����_�ȉ���؂�̂Ă���̂ŁA1.0�����̒l�������ɓ������Ə����_�ȉ������z��ɐς܂�Ȃ��Ȃ�B
			l.push_back(num + '0');
			size++;
		}

		//�t����
		list<char>::reverse_iterator it = l.rbegin();
		while (it != l.rend())
		{
			if (size - i == _decimal)
			{
				s[i] = '.';
				i++;
			}
			//�O������o��
			s[i] = (*it);
			it++;
			i++;
		}

		//�I�[����
		s[i] = '\0';
	}

	void ToString(const Vector4& vec4, wchar_t* s, const int decimal) {
		wchar_t x[10];
		ToString(vec4.x, x, decimal);
		wcscpy_s(s,wcslen(x) + 1,x);
		wcscat_s(s, wcslen(s) + wcslen(L", ") + 1, L", ");
		wchar_t y[10];
		ToString(vec4.y, y, decimal);
		wcscat_s(s, wcslen(s) + wcslen(y) + 1, y);
		wcscat_s(s, wcslen(s) + wcslen(L", ") + 1, L", ");
		wchar_t z[10];
		ToString(vec4.z, z, decimal);
		wcscat_s(s, wcslen(s) + wcslen(z) + 1, z);
		wcscat_s(s, wcslen(s) + wcslen(L", ") + 1, L", ");
		wchar_t w[10];
		ToString(vec4.w, w, decimal);
		wcscat_s(s, wcslen(s) + wcslen(w) + 1, w);
	}

	int StringToInt(const char * string)
	{
		//�����`�F�b�N
		bool isminus = (string[0] == '-');
		int sign = isminus ? -1 : 1;
		char copy[256];
		ZeroMemory(copy, sizeof(char) * 256);
		//1�������炷�B
		strcpy(copy, string + (isminus));

		//�������̕�����
		char* intS = strtok(copy, ".");

		//��������
		int integer = 0;
		//�����𒲂ׂ�
		size_t intL = strlen(intS);
		//���������[�v
		for (short i = 0; i < intL; i++)
		{
			int val = *(intS + i) - '0';
			integer = (integer * 10) + val;
		}

		return integer * sign;
	}

	double StringToDouble(const char * string)
	{
		//�����`�F�b�N
		bool isminus = string[0] == '-';
		int sign = isminus ? -1 : 1;
		char copy[256];
		ZeroMemory(copy, sizeof(char) * 256);
		//1�������炷�B
		strcpy(copy, string + (isminus));

		//�������̕�����
		char* intS = strtok(copy, ".");
		//���������̕�����
		char decS[256];
		strcpy(decS, copy + (strlen(intS) + 1));
		

		//���������Ə�������
		double integer = 0,decimal = 0.0f;
		//�����𒲂ׂ�
		auto intL = strlen(intS), decL = strlen(decS);
		//���������[�v
		for(short i = 0;i < intL;i++)
		{
			double val = *(intS + i) - '0';
			integer = (integer * 10) + val;
		}

		//���������[�v
		for (short i = decL-1; i >= 0; i--)
		{
			double val = (*(decS + i) - '0') / 10.0f;
			decimal = (decimal / 10.0f) + val;
		}

		//�������Ə����������Z����
		double out = integer + decimal;
		//�l�̌ܓ�����
		Round(out, decL);
		return out * sign;
	}

	void Round(double& num, const int& decimal)
	{
		//�ׂ���v�Z
		double multi = 1.0f;
		FOR(i, decimal)
			multi *= 10.0f;

		int integer = (num * multi) + 0.5f;
		//flota�ɖ߂�
		num = integer / multi;
	}
	void StrReplace(char * sorce, const char * find, const char * place)
	{
		while (true)
		{
			//�����񌟍�
			char* addres = strstr(sorce, find);

			if(addres)
			{
				const auto slen = strlen(sorce);
				const auto plen = strlen(place);
				//�ޔ�p�o�b�t�@
				char* buff = new char[slen];
				strcpy(buff, addres);
				//�u���ꏊ�̎��̏ꏊ
				char* next = buff + (plen + 1);
				//��������
				//�u��
				memcpy(addres, place, sizeof(char) * plen);
				//�l�߂�
				memcpy(addres + plen, next, sizeof(char) * strlen(addres + plen));
				SAFE_DELETE(buff);
				next = nullptr;
			}
			else
			{
				//������Ȃ������̂Œu���I��
				return;
			}
		}
	}

	void* ConvertFloatArrayFromString(char* word, void* pRet,const int dataNum) {
		int offset = 0;
		char copy[256];
		strcpy(copy, word);
		for (int idx = 0; idx < dataNum; idx++)
		{
			//�����̕��������o���B
			char* num = strtok(copy + offset, "/");
			//�����ɕϊ�����B
			float value = Support::StringToDouble(num);
			//�l�Z�b�g�B
			memcpy((char*)pRet + (sizeof(float) * idx), &value, sizeof(float));
			//�I�t�Z�b�g�ʂ𑝂₷�B
			offset += strlen(num) + 1;
		}

		return pRet;
	}
}






