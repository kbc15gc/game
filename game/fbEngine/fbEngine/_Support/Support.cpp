#include"fbstdafx.h"
#include "Support.h"

namespace Support
{
	int MakeHash(const char * s)
	{
		int hash = 0;
		while (*s != '\0')
		{
			//加算していく
			hash = hash * 37 + (*s);
			//次の文字に
			s++;
		}
		return hash;
	}

	void ToString(const int& dnum, char* s)
	{
		list<char> l;
		//結局コピーしちゃった
		int num = dnum;
		do
		{
			//文字コード変換
			char d = (num % 10) + '0';
			//登録
			l.push_back(d);
			//次の桁へ
			num /= 10;
		} while (num != 0);

		//逆から
		list<char>::reverse_iterator it = l.rbegin();
		short i = 0;
		while (it != l.rend())
		{
			//前から取り出す
			s[i] = (*it);
			it++;
			i++;
		}
		//終端文字
		s[i] = '\0';
	}

	void ToString(const int& dnum, wchar_t* s)
	{
		list<char> l;
		//結局コピーしちゃった
		int num = dnum;
		do
		{
			//文字コード変換
			char d = (num % 10) + '0';
			//登録
			l.push_back(d);
			//次の桁へ
			num /= 10;
		} while (num != 0);

		//逆から
		list<char>::reverse_iterator it = l.rbegin();
		short i = 0;
		while (it != l.rend())
		{
			//前から取り出す
			s[i] = (*it);
			it++;
			i++;
		}
		//終端文字
		s[i] = '\0';
	}

	void ToString(const float& fnum, wchar_t * s, const int& decimal)
	{
		//繰り上げ
		int num = (int)(fnum * pow(10, decimal));
		list<char> l;
		do
		{
			//文字コード変換
			char d = (num % 10) + '0';
			//登録
			l.push_back(d);
			//次の桁へ
			num /= 10;
		} while (num != 0);

		//逆から
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
			//前から取り出す
			s[i] = (*it);
			it++;
			i++;
		}
		//終端文字
		s[i] = '\0';
	}

	double StringToDouble(const char * string)
	{
		//符号チェック
		bool isminus = string[0] == '-';
		int sign = isminus ? -1 : 1;
		char copy[256];
		ZeroMemory(copy, sizeof(char) * 256);
		//1文字ずらす。
		strcpy(copy, string + (isminus));

		//整数部の文字列
		char* intS = strtok(copy, ".");
		//小数部分の文字列
		char decS[256];
		strcpy(decS, copy + (strlen(intS) + 1));
		

		//整数部分と小数部分
		double integer = 0,decimal = 0.0f;
		//長さを調べる
		int intL = strlen(intS), decL = strlen(decS);
		//整数部ループ
		for(short i = 0;i < intL;i++)
		{
			double val = *(intS + i) - '0';
			integer = (integer * 10) + val;
		}

		//小数部ループ
		for (short i = decL-1; i >= 0; i--)
		{
			double val = (*(decS + i) - '0') / 10.0f;
			decimal = (decimal / 10.0f) + val;
		}

		//整数部と小数部を加算する
		double out = integer + decimal;
		//四捨五入する
		Round(out, decL);
		return out * sign;
	}

	void Round(double & num, const int & decimal)
	{
		//べき乗計算
		double multi = 1.0f;
		FOR(i, decimal)
			multi *= 10.0f;

		int integer = (num * multi) + 0.5f;
		//flotaに戻す
		num = integer / multi;
	}
	void StrReplace(char * sorce, const char * find, const char * place)
	{
		while (true)
		{
			//文字列検索
			char* addres = strstr(sorce, find);

			if(addres)
			{
				const int slen = strlen(sorce);
				const int plen = strlen(place);
				//退避用バッファ
				char* buff = new char[slen];
				strcpy(buff, addres);
				//置換場所の次の場所
				char* next = buff + (plen + 1);
				//見つかった
				//置換
				memcpy(addres, place, sizeof(char) * plen);
				//詰める
				memcpy(addres + plen, next, sizeof(char) * strlen(addres + plen));
				SAFE_DELETE(buff);
				next = nullptr;
			}
			else
			{
				//見つからなかったので置換終了
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
			//数字の部分を取り出す。
			char* num = strtok(copy + offset, "/");
			//数字に変換する。
			float value = Support::StringToDouble(num);
			//値セット。
			memcpy((char*)pRet + (sizeof(float) * idx), &value, sizeof(float));
			//オフセット量を増やす。
			offset += strlen(num) + 1;
		}

		return pRet;
	}
}






