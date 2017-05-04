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
		char copy[256];
		ZeroMemory(copy, sizeof(char) * 256);
		strcpy(copy, string);

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
		return out;
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
}




