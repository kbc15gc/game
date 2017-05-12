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

	void ToString(const int& dnum, char* s)
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

	void ToString(const int& dnum, wchar_t* s)
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

	void ToString(const float& fnum, wchar_t * s, const int& decimal)
	{
		//�J��グ
		int num = (int)(fnum * pow(10, decimal));
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

		//�t����
		list<char>::reverse_iterator it = l.rbegin();
		short i = 0;
		size_t size = l.size();
		while (it != l.rend())
		{
			if (size - i == decimal)
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
		int intL = strlen(intS), decL = strlen(decS);
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

	void Round(double & num, const int & decimal)
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
				const int slen = strlen(sorce);
				const int plen = strlen(place);
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
}




