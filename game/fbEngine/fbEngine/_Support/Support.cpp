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

	void ToString(const int dnum, char* s)
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

	void ToString(const int dnum, wchar_t* s)
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

	void ToString(const float fnum, wchar_t * s, const int decimal)
	{
		short i = 0;
		int _decimal = decimal;
		if (fnum < 0.0f) {
			// マイナスの値。
			s[0] = '-';
			i++;
			_decimal--;
		}
		else if (fabsf(fnum) <= 0.00001f) {
			// ほぼ0なので0.0fを入れて返却。
			wcscpy_s(s,wcslen(L"0.0") + 1, L"0.0");
			return;
		}

		//繰り上げ
		int num = (int)(fabsf(fnum) * pow(10, decimal));
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

		size_t size = l.size();
		if (size == decimal) {
			// 小数点以下しか積まれてない。

			// ※int型は小数点以下を切り捨てするので、1.0未満の値が引数に入れられると小数点以下しか配列に積まれなくなる。
			l.push_back(num + '0');
			size++;
		}

		//逆から
		list<char>::reverse_iterator it = l.rbegin();
		while (it != l.rend())
		{
			if (size - i == _decimal)
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
		//符号チェック
		bool isminus = (string[0] == '-');
		int sign = isminus ? -1 : 1;
		char copy[256];
		ZeroMemory(copy, sizeof(char) * 256);
		//1文字ずらす。
		strcpy(copy, string + (isminus));

		//整数部の文字列
		char* intS = strtok(copy, ".");

		//整数部分
		int integer = 0;
		//長さを調べる
		size_t intL = strlen(intS);
		//整数部ループ
		for (short i = 0; i < intL; i++)
		{
			int val = *(intS + i) - '0';
			integer = (integer * 10) + val;
		}

		return integer * sign;
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
		auto intL = strlen(intS), decL = strlen(decS);
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

	void Round(double& num, const int& decimal)
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
				const auto slen = strlen(sorce);
				const auto plen = strlen(place);
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






