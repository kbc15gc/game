#pragma once
#include <fstream>
#include <string>
#include <iostream>

//いろいろ便利な関数を作ろう
namespace Support
{
	//文字列からハッシュ値を求める(簡易)
	extern int MakeHash(const char* s);
	//整数を文字列に
	extern void ToString(const int dnum, char* s);
	//整数を文字列(wchar)に
	extern void ToString(const int dnum, wchar_t* s);
	//小数を文字列に
	extern void ToString(const float fnum, wchar_t* s, const int decimal = 1);
	// Vectorを文字列に
	// 引数：	変換したい値。
	//			変換後の文字列へのポインタ。
	//			小数点第何位まで変換するか。
	extern void ToString(const Vector4& vec4, wchar_t* s, const int decimal = 1);
	//文字列を整数にする。
	extern int StringToInt(const char* string);
	//文字列を小数に
	extern double StringToDouble(const char* string);
	// 文字列をVectorやQuaternionやFloat配列に
	extern void* ConvertFloatArrayFromString(char* word, void* pRet,const int dataNum);

	//小数を四捨五入する
	extern void Round(double& num, const int& decimal = 1);
	//文字列中の指定文字列を別の文字列に置換
	//変換を行う文字列 in.out
	//置き換えたい文字列
	//置き換える文字列
	extern void StrReplace(char* sorce, const char* find, const char* place);
}

namespace Support {

	//変数の型を指定。
	enum DataTypeE
	{
		INT,	//整数
		FLOAT,	//浮動小数
		VECTOR2,	// Vector2。
		VECTOR3,//Vector3
		VECTOR4,// Vector4。
		QUATERNION = VECTOR4,//Quaternion。
		STRING,	//文字
		ARRAY = BIT(8),	//配列。
		INT_ARRAY = INT | ARRAY,				//整数配列。
		FLOAT_ARRAY = FLOAT | ARRAY,			//浮動小数配列。
		VECTOR3_ARRAY = VECTOR3 | ARRAY,		//Vector3配列。
		VECTOR4_ARRAY = VECTOR4 | ARRAY,		//Vector4配列。
		QUATERNION_ARRAY = QUATERNION | ARRAY	//Quaternion配列。
	};
	//メンバ変数の情報。
	struct DATARECORD
	{
		const char* name;	//変数名。
		DataTypeE type;		//メンバ変数の型。
		int offset;			//メンバ変数のアドレスが構造体の先頭アドレスからどれだけ移動しているか？
		int size;			//メンバ変数のサイズ。
	};
}

namespace
{
	//文字列を数字に変換し、受け取ったアドレスに設定。
	//[out] 結果を格納するアドレス。
	//[in] 変換する文字列。
	//[in] 型を指定する列挙型。
	//[in] コピーするデータのサイズ。
	void SetValue(char* addres, char* word, Support::DataTypeE type, const int size)
	{
		//配列ではない
		if((type & Support::DataTypeE::ARRAY) == 0)
		{
			if (type == Support::DataTypeE::INT)
			{
				int val = Support::StringToInt(word);
				memcpy(addres, &val, size);
			}
			else if (type == Support::DataTypeE::FLOAT)
			{
				float val = Support::StringToDouble(word);
				memcpy(addres, &val, size);
			}
			else if (type == Support::DataTypeE::VECTOR2)
			{
				Vector2 v2 = Vector2(0.0f, 0.0f);

				Support::ConvertFloatArrayFromString(word, &v2, 2);
				memcpy(addres, &v2, size);
			}
			else if (type == Support::DataTypeE::VECTOR3)
			{
				Vector3 v3 = Vector3::zero;

				Support::ConvertFloatArrayFromString(word, &v3, 3);

				memcpy(addres, &v3, size);
			}
			else if (type == Support::DataTypeE::VECTOR4)
			{
				Vector4 v4 = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

				Support::ConvertFloatArrayFromString(word, &v4, 4);

				memcpy(addres, &v4, size);
			}
			else if (type == Support::DataTypeE::STRING)
			{
				memcpy(addres, word, size);
			}
		}
		//配列。
		else
		{
			size_t offset = 0;
			char copy[512];
			//初期化。
			ZeroMemory(copy, sizeof(copy));
			//[]を外す。
			auto len = strlen(word) - 2;
			strncpy(copy, word + 1, len);

			if (type == Support::DataTypeE::INT_ARRAY) 
			{
				const int max = size / sizeof(int);
				int Array[999];	// とりあえず多めに取っておく。									
				ZeroMemory(Array, sizeof(Array));//初期化。

				for (int idx = 0;  idx < max, *(copy + offset) != '\0'; idx++) {
					//数字の部分を取り出す。
					char* num = strtok(copy + offset, ",");
					//数字に変換する。
					Array[idx] = Support::StringToInt(num);
					//オフセット量を増やす。
					offset += strlen(num) + 1;
				}
				memcpy(addres, &Array, size);
			}
			else if (type == Support::DataTypeE::FLOAT_ARRAY)
			{
				const int max = size / sizeof(float);
				float Array[999];	// とりあえず多めに取っておく。									
				ZeroMemory(Array, sizeof(Array));//初期化。

				for (int idx = 0; idx < max, *(copy + offset) != '\0'; idx++) {
					//配列の一要素を取り出す。
					char* num = strtok(copy + offset, ",");
					//数字に変換する。
					Array[idx] = Support::StringToDouble(num);
					//オフセット量を増やす。
					offset += strlen(num) + 1;
				}
				memcpy(addres, &Array, size);
			}
			else if (type == Support::DataTypeE::VECTOR3_ARRAY)
			{
				const int max = size / sizeof(Vector3);
				Vector3 Array[999];	// とりあえず多めに取っておく。									
				ZeroMemory(Array, sizeof(Array));//初期化。

				for (int idx = 0; idx < max, *(copy + offset) != '\0'; idx++) {
					//配列の一要素を取り出す。
					char* num = strtok(copy + offset, ",");
					Vector3 v3 = Vector3::zero;
					//文字列を値に変換。
					Support::ConvertFloatArrayFromString(num, &v3, 3);
					Array[idx] = v3;

					//オフセット量を増やす。
					offset += strlen(num) + 1;
				}
				memcpy(addres, &Array, size);
			}
			else if (type == Support::DataTypeE::VECTOR4_ARRAY)
			{
				const int max = size / sizeof(Vector4);
				Vector4 Array[999];	// とりあえず多めに取っておく。									
				ZeroMemory(Array, sizeof(Array));//初期化。

				for (int idx = 0; idx < max, *(copy + offset) != '\0'; idx++) {
					//配列の一要素を取り出す。
					char* num = strtok(copy + offset, ",");
					Vector4 v4(0, 0, 0, 0);
					//文字列を値に変換。
					Support::ConvertFloatArrayFromString(num, &v4, 4);
					Array[idx] = v4;

					//オフセット量を増やす。
					offset += strlen(num) + 1;
				}
				memcpy(addres, &Array, size);
			}
		}
	}
}

namespace Support {
	//CSVファイルから情報を読み取り、構造体に格納する。
	// テンプレート引数：	この関数内で配列に格納する時に、この引数を用いてnewする。
	//						引数4に渡す配列の中身(ユニークポインタ)のテンプレート引数(デフォルトはテンプレート引数1)。
	// 引数：				読み込み先のファイル名。
	//						読み込むデータのレコード(形式)。
	//						1レコードを構成するデータの数。
	//						読み込んだデータを格納する配列。
	// ※テンプレート引数2はポリモーフィズムを活用したクラスや構造体のデータを読み込む際に使用する。
	template<class T1,class T2 = T1>
	extern bool LoadCSVData(const char* filepath, const Support::DATARECORD* datas, const int& datanum, vector<unique_ptr<T2>>& output)
	{
		//ファイル入力用
		std::ifstream fin(filepath, std::ios::in);

		//ファイルが開けなかったらエラー
		if (fin.fail())
		{
			char error[256];
			sprintf(error, "ファイル名：%s\nを開けませんでした。", filepath);
			MessageBoxA(0, error, "ファイル読み込みエラー", MB_ICONWARNING);
			return fin.fail();
		}

		const int MAX_SIZE = 1028;
		//ファイルからデータを受け取るバッファ
		char line[MAX_SIZE], copy[MAX_SIZE];

		//スキップする処理(要改善)
		fin.getline(line, MAX_SIZE);

		while (true)
		{
			//初期化してから新しい行を読み込む
			ZeroMemory(line, sizeof(line));
			fin.getline(line, MAX_SIZE);

			//ファイルの終端になったなら抜ける。
			if (fin.eof())
				break;

			//csvの"\n"が"//n"と認識される問題(根本的な解決ではないかも)。
			//\\nを\nに置き換える処理。
			StrReplace(line, "\\n", "\n");

			int idx = 0;		//何番目の要素か？
			int offset = 0;		//先頭からのoffset量
			memcpy(copy, line, sizeof(line));
			//アドレス確保
			T2* tmp = new T1();
			
			while (*(line + offset) != '\0' &&	//1行の終端までループ
				idx < datanum)					//範囲チェック
			{
				auto type = (DataTypeE)datas[idx].type;
				char* word;
				if ((type & DataTypeE::ARRAY) > 0)
				{
					//']'まで取得。
					word = strtok(copy + offset, "]");
					strcat(word, "]");
				}
				else
				{
					//","区切りで単語を抽出
					//","の部分には'\0'が埋め込まれるのでコピーを使う
					word = strtok(copy + offset, ",");
				}
				

				//変数の先頭アドレスを求める。
				//char*型(1byte)にキャストすることで1づつずらすことができる。
				auto addres = (char*)tmp + datas[idx].offset;
				//アドレスに値を設定。
				SetValue(addres, word, type, datas[idx].size);

				//単語の長さ+1(","分)をオフセット量に加算
				offset += strlen(word) + 1;
				idx++;
			}
			//要素追加
			output.push_back(unique_ptr<T2>(tmp));
		}
		//読み込み終了
		return true;
	}

	//テンプレートクラスから情報を読み取り、CSVファイルに出力する。
	template<class T>
	extern bool OutputCSV(const char* filepath, const Support::DATARECORD* datas, const int& datanum, const vector<unique_ptr<T>>& output) {
		vector<T*> work;
		for (int idx = 0; idx < static_cast<int>(output.size()); idx++) {
			work.push_back(output[idx].get());
		}
		return OutputCSV(filepath, datas, datanum, work);
	}

	//テンプレートクラスから情報を読み取り、CSVファイルに出力する。
	template<class T>
	extern bool OutputCSV(const char* filepath, const Support::DATARECORD* datas, const int& datanum,const vector<T*>& output)
	{
		//ファイル入力用
		std::ofstream fout(filepath, std::ios::out);

		//ファイルが開けなかったらエラー
		if (fout.fail())
		{
			char error[256];
			strcpy(error, filepath);
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
		for each (T* out in output)
		{
			//書き出す変数の数ループ
			FOR(idx,datanum)
			{
				//出力する単語
				char* word = new char[256];
				ZeroMemory(word, sizeof(char) * 256);
				//変数の先頭アドレスを求める。
				//char*型(1byte)にキャストすることで1づつずらすことができる。
				auto addres = (char*)out + datas[idx].offset;

				int i = 0;
				float f = 0;
				//型にあった変換をする
				switch (datas[idx].type)
				{
				case DataTypeE::INT:
					memcpy(&i, addres, datas[idx].size);
					sprintf(word, "%d", i);
					//ToString(i, word);
					break;
				case DataTypeE::INT_ARRAY:	// 配列に対応?。
					strcat(word, "[");
					FOR(num, datas[idx].size / sizeof(int)){
						memcpy(&i, addres, sizeof(int));
						sprintf(word, "%d", i);
						//ToString(i, word);
						//配列の最後ではない
						if (num < (datas[idx].size / sizeof(int)) - 1)
						{
							//","を追加
							strcat(word, ",");
						}
						addres += sizeof(int);	// int一要素分ずらす。
					}
					strcat(word, "]");
					break;
				case DataTypeE::FLOAT:
					memcpy(&f, addres, datas[idx].size);
					sprintf(word, "%f", f);
					//ToString(f, word);
					break;
				case DataTypeE::VECTOR2:
				case DataTypeE::VECTOR3:
				case DataTypeE::VECTOR4:
					FOR(num, datas[idx].size / sizeof(float)){
						memcpy(&f, addres, sizeof(float));
						sprintf(word, "%f", f);
						//Vectorの最後ではない
						if (num < (datas[idx].size / sizeof(float)) - 1)
						{
							//"/"を追加
							strcat(word, "/");
						}
						addres += sizeof(float);	// float一要素分ずらす。
					}
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