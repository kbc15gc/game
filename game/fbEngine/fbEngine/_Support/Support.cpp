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

	void ReadCSV(char * filename)
	{
		std::ifstream ifs("Asset//Data//tets.csv");
		string buf;
		vector<vector<string>> Vec;
		int line = 0;
		//�ǂݍ���
		while (ifs && getline(ifs, buf))
		{
			string moji = "";
			vector<string> Vs;
			//�R���}��T��
			for (short idx = 0, i = 0; idx < (int)buf.length(); idx++)
			{
				if (buf[idx] == ',')
				{
					Vs.push_back(moji);
					i++;
					moji = "";
				}
				else
				{
					moji += buf[idx];
				}

			}
			//������ǉ�
			Vec.push_back(Vs);
			line++;
		}

		return;
	}
}




