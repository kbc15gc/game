#pragma once
#include <fstream>
#include <string>
#include <iostream>

//いろいろ便利な関数を作ろう
namespace Support
{
	//文字列からハッシュ値を求める(簡易)
	extern int MakeHash(const char * s);
	//整数を文字列に
	extern void ToString(const int& dnum, char* s);
	//整数を文字列(wchar)に
	extern void ToString(const int& dnum, wchar_t* s);
	//小数を文字列に
	extern void ToString(const float& fnum, wchar_t * s, const int& decimal = 1);
	//文字列を小数に
	extern double StringToDouble(const char* string);
	//小数を四捨五入する
	extern void Round(double& num, const int& decimal = 1);

	//変数の型を指定。
	enum DataTypeE
	{
		INT,	//整数
		FLOAT,	//浮動小数
		STRING	//文字
	};
	//メンバ変数の情報。
	struct DATARECORD
	{
		const char* name;	//変数名。
		DataTypeE type;		//メンバ変数の型。
		int offset;			//メンバ変数のアドレスが構造体の先頭アドレスからどれだけ移動しているか？
		int size;			//メンバ変数のサイズ。
	};

	//CSVファイルから情報を読み取り、テンプレートクラスに格納する。
	template<class T>
	extern bool LoadCSVData(const char* filepath, const Support::DATARECORD* datas, const int& datanum, vector<T>& output)
	{
		//ファイル入力用
		std::ifstream fin(filepath, std::ios::in);

		//ファイルが開けなかったらエラー
		if (fin.fail())
		{
			char error[256];
			strcat(error, filepath);
			strcat(error, "\nを開けませんでした。");
			MessageBoxA(0, error, "ファイル読み込みエラー", MB_ICONWARNING);
			return fin.fail();
		}

		//ファイルからデータを受け取るバッファ
		char line[256], copy[256];

		//スキップする処理(要改善)
		fin.getline(line, 256);

		//初期化してから、ファイルから一列読み込む
		ZeroMemory(line, sizeof(char) * 256);
		fin.getline(line, 256);
		//ファイルの終端までループ
		while (!fin.eof())
		{
			int idx = 0;		//何番目の要素か？
			int offset = 0;		//先頭からのoffset量
			memcpy(copy, line, sizeof(char) * 256);
			T tmp;
			//1行の終端までループ
			//範囲チェック
			while (*(line + offset) != '\0' && idx < datanum)
			{
				//","区切りで単語を抽出、","の部分には'\0'が埋め込まれるのでコピーを使う
				char* word = strtok(copy + offset, ",");

				//ファイルから読み取った値を入れる
				void* val = nullptr;
				int i = 0;
				float f = 0;
				//型にあった変換をする
				switch (datas[idx].type)
				{
				case Support::DataTypeE::INT:
					i = Support::StringToDouble(word);
					val = &i;
					break;
				case Support::DataTypeE::FLOAT:
					f = Support::StringToDouble(word);
					val = &f;
					break;
				case Support::DataTypeE::STRING:
					val = word;
					break;
				default:
					val = nullptr;
					break;
				}

				//変数の先頭アドレスを求める。char*型(1byte)にキャストすることで1づつずらすことができる。
				auto addres = ((char*)(&tmp)) + datas[idx].offset;
				//アドレスに値をコピー
				memcpy(addres, val, datas[idx].size);

				//単語の長さ+1(","分)をオフセット量に加算
				offset += strlen(word) + 1;
				idx++;
			}
			output.push_back(tmp);
			//初期化してから新しい行を読み込む
			ZeroMemory(line, sizeof(char) * 256);
			fin.getline(line, 256);
		}
		//読み込み終了
		return true;
	}

	//テンプレートクラスから情報を読み取り、CSVファイルに出力する。
	template<class T>
	extern bool OutputCSV(const char* filepath, const Support::DATARECORD* datas, const int& datanum, vector<T>& output)
	{
		//ファイル入力用
		std::ofstream fout(filepath, std::ios::out);

		//ファイルが開けなかったらエラー
		if (fout.fail())
		{
			char error[256];
			strcat(error, filepath);
			strcat(error, "\nを開けませんでした。");
			MessageBoxA(0, error, "ファイル読み込みエラー", MB_ICONWARNING);
			return fout.fail();
		}

		//最初の一行に変数名書き出し
		FOR(idx, datanum)
		{
			fout << datas[idx].name;
			if (idx != datanum - 1)
			{
				//","を追加
				fout << ",";
			}
		}
		fout << std::endl;

		//書き出すオブジェクトの分ループ
		for each (T out in output)
		{
			//書き出す変数の数ループ
			FOR(idx,datanum)
			{
				//出力する単語
				char* word = new char[256];
				ZeroMemory(word, sizeof(char) * 256);
				//変数の先頭アドレスを求める。char*型(1byte)にキャストすることで1づつずらすことができる。
				auto addres = ((char*)(&out)) + datas[idx].offset;

				int i = 0;
				float f = 0;
				//型にあった変換をする
				switch (datas[idx].type)
				{
				case DataTypeE::INT:
					memcpy(&i, addres, datas[idx].size);
					ToString(i, word);
					break;
				case DataTypeE::FLOAT:
					memcpy(&f, addres, datas[idx].size);
					//ToString(f, word);
					break;
				case DataTypeE::STRING:
					memcpy(word, addres, datas[idx].size);
					break;
				default:
					memcpy(word, addres, datas[idx].size);
					break;
				}

				//行の最後ではない
				if(idx != datanum -1)
				{
					//","を追加
					strcat(word, ",");
				}

				//改行
				fout << word;
				//メモリ解放
				SAFE_DELETE_ARRAY(word);
			}
			//改行
			fout << std::endl;
		}
		return true;
	}
}