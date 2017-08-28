#pragma once
#include <fstream>
#include <string>
#include <iostream>

//���낢��֗��Ȋ֐�����낤
namespace Support
{
	//�����񂩂�n�b�V���l�����߂�(�Ȉ�)
	extern int MakeHash(const char* s);
	//�����𕶎����
	extern void ToString(const int dnum, char* s);
	//�����𕶎���(wchar)��
	extern void ToString(const int dnum, wchar_t* s);
	//�����𕶎����
	extern void ToString(const float fnum, wchar_t* s, const int decimal = 1);
	// Vector�𕶎����
	// �����F	�ϊ��������l�B
	//			�ϊ���̕�����ւ̃|�C���^�B
	//			�����_�扽�ʂ܂ŕϊ����邩�B
	extern void ToString(const Vector4& vec4, wchar_t* s, const int decimal = 1);
	//������𐮐��ɂ���B
	extern int StringToInt(const char* string);
	//�������������
	extern double StringToDouble(const char* string);
	// �������Vector��Quaternion��Float�z���
	extern void* ConvertFloatArrayFromString(char* word, void* pRet,const int dataNum);

	//�������l�̌ܓ�����
	extern void Round(double& num, const int& decimal = 1);
	//�����񒆂̎w�蕶�����ʂ̕�����ɒu��
	//�ϊ����s�������� in.out
	//�u����������������
	//�u�������镶����
	extern void StrReplace(char* sorce, const char* find, const char* place);
}

namespace Support {

	//�ϐ��̌^���w��B
	enum DataTypeE
	{
		INT,	//����
		INTARRAY,	// �����z��B
		FLOAT,	//��������
		VECTOR2,	// Vector2�B
		VECTOR3,//Vector3
		VECTOR4,// Vector4�B
		QUATERNION,//Quaternion�B
		STRING,	//����
	};
	//�����o�ϐ��̏��B
	struct DATARECORD
	{
		const char* name;	//�ϐ����B
		DataTypeE type;		//�����o�ϐ��̌^�B
		int offset;			//�����o�ϐ��̃A�h���X���\���̂̐擪�A�h���X����ǂꂾ���ړ����Ă��邩�H
		int size;			//�����o�ϐ��̃T�C�Y�B
	};
}

namespace
{
	//��������󂯎���Ēl�ɕϊ����A�A�h���X��Ԃ��B
	//�����̓N�r��B
	void* ConvertValueFromString(char* word, Support::DataTypeE type, const int size)
	{
		if (type == Support::DataTypeE::INT)
		{
			int val = Support::StringToInt(word);
			return &val;
		}
		else if (type == Support::DataTypeE::INTARRAY) {
			int offset = 0;
			char copy[256];
			strcpy(copy, word);
			const int max = size / sizeof(int);
			int Array[999];	// �Ƃ肠�������߂Ɏ���Ă����B
			ZeroMemory(Array, sizeof(Array));

			for (int idx = 0; idx < max; idx++) {
				//�����̕��������o���B
				char* num = strtok(copy + offset, "/");
				//�����ɕϊ�����B
				Array[idx] = Support::StringToInt(num);
				//�I�t�Z�b�g�ʂ𑝂₷�B
				offset += strlen(num) + 1;
			}

			return Array;
		}
		else if (type == Support::DataTypeE::FLOAT)
		{
			float val = Support::StringToDouble(word);
			return &val;
		}
		else if (type == Support::DataTypeE::VECTOR2)
		{
			Vector2 v2 = Vector2(0.0f, 0.0f);

			Support::ConvertFloatArrayFromString(word, &v2, 2);

			//int offset = 0;
			//char copy[256];
			//strcpy(copy, word);
			//FOR(i, 3)
			//{
			//	//�����̕��������o���B
			//	char* num = strtok(copy + offset, "/");
			//	//�����ɕϊ�����B
			//	float value = Support::StringToDouble(num);
			//	//�l�Z�b�g�B
			//	memcpy((char*)&v3 + (sizeof(float) * i), &value, sizeof(float));
			//	//�I�t�Z�b�g�ʂ𑝂₷�B
			//	offset += strlen(num) + 1;
			//}
			return &v2;
		}
		else if (type == Support::DataTypeE::VECTOR3)
		{
			Vector3 v3 = Vector3::zero;

			Support::ConvertFloatArrayFromString(word, &v3, 3);

			//int offset = 0;
			//char copy[256];
			//strcpy(copy, word);
			//FOR(i, 3)
			//{
			//	//�����̕��������o���B
			//	char* num = strtok(copy + offset, "/");
			//	//�����ɕϊ�����B
			//	float value = Support::StringToDouble(num);
			//	//�l�Z�b�g�B
			//	memcpy((char*)&v3 + (sizeof(float) * i), &value, sizeof(float));
			//	//�I�t�Z�b�g�ʂ𑝂₷�B
			//	offset += strlen(num) + 1;
			//}
			return &v3;
		}
		else if (type == Support::DataTypeE::VECTOR4)
		{
			Vector4 v4 = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

			Support::ConvertFloatArrayFromString(word, &v4, 4);

			//int offset = 0;
			//char copy[256];
			//strcpy(copy, word);
			//FOR(i, 3)
			//{
			//	//�����̕��������o���B
			//	char* num = strtok(copy + offset, "/");
			//	//�����ɕϊ�����B
			//	float value = Support::StringToDouble(num);
			//	//�l�Z�b�g�B
			//	memcpy((char*)&v3 + (sizeof(float) * i), &value, sizeof(float));
			//	//�I�t�Z�b�g�ʂ𑝂₷�B
			//	offset += strlen(num) + 1;
			//}
			return &v4;
		}
		else if (type == Support::DataTypeE::QUATERNION)
		{
			Quaternion quat = Quaternion::Identity;

			Support::ConvertFloatArrayFromString(word, &quat, 4);

			//int offset = 0;
			//char copy[256];
			//strcpy(copy, word);
			//FOR(i, 4)
			//{
			//	//�����̕��������o���B
			//	char* num = strtok(copy + offset, "/");
			//	//�����ɕϊ�����B
			//	float value = Support::StringToDouble(num);
			//	//�l�Z�b�g�B
			//	memcpy((char*)&quat + (sizeof(float) * i), &value, sizeof(float));
			//	//�I�t�Z�b�g�ʂ𑝂₷�B
			//	offset += strlen(num) + 1;
			//}
			return &quat;
		}
		else if (type == Support::DataTypeE::STRING)
		{
			return word;
		}

		return nullptr;
	}

	//������𐔎��ɕϊ����A�󂯎�����A�h���X�ɐݒ�B
	void SetValue(char* addres, char* word, Support::DataTypeE type, const int size)
	{
		if (type == Support::DataTypeE::INT)
		{
			int val = Support::StringToInt(word);
			memcpy(addres, &val, size);
		}
		else if (type == Support::DataTypeE::INTARRAY) {
			int offset = 0;
			char copy[256];
			strcpy(copy, word);
			const int max = size / sizeof(int);
			int Array[999];	// �Ƃ肠�������߂Ɏ���Ă����B
			ZeroMemory(Array, sizeof(Array));

			for (int idx = 0; idx < max; idx++) {
				//�����̕��������o���B
				char* num = strtok(copy + offset, "/");
				//�����ɕϊ�����B
				Array[idx] = Support::StringToInt(num);
				//�I�t�Z�b�g�ʂ𑝂₷�B
				offset += strlen(num) + 1;
			}

			memcpy(addres, &Array, size);
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
		else if (type == Support::DataTypeE::QUATERNION)
		{
			Quaternion quat = Quaternion::Identity;

			Support::ConvertFloatArrayFromString(word, &quat, 4);

			memcpy(addres, &quat, size);
		}
		else if (type == Support::DataTypeE::STRING)
		{
			memcpy(addres, &word, size);
		}
	}
}

namespace Support {
	//CSV�t�@�C���������ǂݎ��A�\���̂Ɋi�[����B
	//�����z�֐������������\���̂��g���ƃ�������j�󂷂�\������B
	template<class T>
	extern bool LoadCSVData(const char* filepath, const Support::DATARECORD* datas, const int& datanum, vector<unique_ptr<T>>& output)
	{
		//�t�@�C�����͗p
		std::ifstream fin(filepath, std::ios::in);

		//�t�@�C�����J���Ȃ�������G���[
		if (fin.fail())
		{
			char error[256];
			sprintf(error, "�t�@�C�����F%s\n���J���܂���ł����B", filepath);
			MessageBoxA(0, error, "�t�@�C���ǂݍ��݃G���[", MB_ICONWARNING);
			return fin.fail();
		}

		//�t�@�C������f�[�^���󂯎��o�b�t�@
		char line[512], copy[512];

		//�X�L�b�v���鏈��(�v���P)
		fin.getline(line, 512);

		while (true)
		{
			//���������Ă���V�����s��ǂݍ���
			ZeroMemory(line, sizeof(char) * 512);
			fin.getline(line, 512);

			//�t�@�C���̏I�[�ɂȂ����Ȃ甲����B
			if (fin.eof())
				break;

			//csv��"\n"��"//n"�ƔF���������(���{�I�ȉ����ł͂Ȃ�����)�B
			StrReplace(line, "\\n", "\n");

			int idx = 0;		//���Ԗڂ̗v�f���H
			int offset = 0;		//�擪�����offset��
			memcpy(copy, line, sizeof(char) * 512);
			//�A�h���X�m��
			T* tmp = new T();
			
			while (*(line + offset) != '\0' &&	//1�s�̏I�[�܂Ń��[�v
				idx < datanum)					//�͈̓`�F�b�N
			{
				//","��؂�ŒP��𒊏o
				//","�̕����ɂ�'\0'�����ߍ��܂��̂ŃR�s�[���g��
				char* word = strtok(copy + offset, ",");

				//�ϐ��̐擪�A�h���X�����߂�B
				//char*�^(1byte)�ɃL���X�g���邱�Ƃ�1�Â��炷���Ƃ��ł���B
				auto addres = (char*)tmp + datas[idx].offset;
				//�A�h���X�ɒl��ݒ�B
				SetValue(addres, word, (DataTypeE)datas[idx].type, datas[idx].size);

				//�P��̒���+1(","��)���I�t�Z�b�g�ʂɉ��Z
				offset += strlen(word) + 1;
				idx++;
			}
			//�v�f�ǉ�
			output.push_back(unique_ptr<T>(tmp));
		}
		//�ǂݍ��ݏI��
		return true;
	}

	//�e���v���[�g�N���X�������ǂݎ��ACSV�t�@�C���ɏo�͂���B
	template<class T>
	extern bool OutputCSV(const char* filepath, const Support::DATARECORD* datas, const int& datanum, const vector<unique_ptr<T>>& output) {
		vector<T*> work;
		for (int idx = 0; idx < output.size(); idx++) {
			work.push_back(output[idx].get());
		}
		return OutputCSV(filepath, datas, datanum, work);
	}

	//�e���v���[�g�N���X�������ǂݎ��ACSV�t�@�C���ɏo�͂���B
	template<class T>
	extern bool OutputCSV(const char* filepath, const Support::DATARECORD* datas, const int& datanum,const vector<T*>& output)
	{
		//�t�@�C�����͗p
		std::ofstream fout(filepath, std::ios::out);

		//�t�@�C�����J���Ȃ�������G���[
		if (fout.fail())
		{
			char error[256];
			strcat(error, filepath);
			strcat(error, "\n���J���܂���ł����B");
			MessageBoxA(0, error, "�t�@�C���ǂݍ��݃G���[", MB_ICONWARNING);
			return fout.fail();
		}

		//�ŏ��̈�s�ɕϐ��������o��
		FOR(idx, datanum)
		{
			fout << datas[idx].name;
			if (idx != datanum - 1)
			{
				//","��ǉ�
				fout << ",";
			}
		}
		fout << std::endl;

		//�����o���I�u�W�F�N�g�̕����[�v
		for each (T* out in output)
		{
			//�����o���ϐ��̐����[�v
			FOR(idx,datanum)
			{
				//�o�͂���P��
				char* word = new char[256];
				ZeroMemory(word, sizeof(char) * 256);
				//�ϐ��̐擪�A�h���X�����߂�B
				//char*�^(1byte)�ɃL���X�g���邱�Ƃ�1�Â��炷���Ƃ��ł���B
				auto addres = (char*)out + datas[idx].offset;

				int i = 0;
				float f = 0;
				//�^�ɂ������ϊ�������
				switch (datas[idx].type)
				{
				case DataTypeE::INT:
					memcpy(&i, addres, datas[idx].size);
					sprintf(word, "%d", i);
					//ToString(i, word);
					break;
				case DataTypeE::INTARRAY:	// �z��ɑΉ�?�B
					FOR(num, datas[idx].size / sizeof(int)){
						memcpy(&i, addres, sizeof(int));
						sprintf(word, "%d", i);
						//ToString(i, word);
						//�z��̍Ō�ł͂Ȃ�
						if (num < (datas[idx].size / sizeof(int)) - 1)
						{
							//"/"��ǉ�
							strcat(word, "/");
						}
						addres += sizeof(int);	// int��v�f�����炷�B
					}
					break;
				case DataTypeE::FLOAT:
					memcpy(&f, addres, datas[idx].size);
					sprintf(word, "%f", f);
					//ToString(f, word);
					break;
				case DataTypeE::VECTOR2:
				case DataTypeE::VECTOR3:
				case DataTypeE::VECTOR4:
				case DataTypeE::QUATERNION:
					FOR(num, datas[idx].size / sizeof(float)){
						memcpy(&f, addres, sizeof(float));
						sprintf(word, "%f", f);
						//Vector�̍Ō�ł͂Ȃ�
						if (num < (datas[idx].size / sizeof(float)) - 1)
						{
							//"/"��ǉ�
							strcat(word, "/");
						}
						addres += sizeof(float);	// float��v�f�����炷�B
					}
					break;
				case DataTypeE::STRING:
					memcpy(word, addres, datas[idx].size);
					break;
				default:
					memcpy(word, addres, datas[idx].size);
					break;
				}

				//�s�̍Ō�ł͂Ȃ�
				if(idx != datanum -1)
				{
					//","��ǉ�
					strcat(word, ",");
				}

				//���s
				fout << word;
				//���������
				SAFE_DELETE_ARRAY(word);
			}
			//���s
			fout << std::endl;
		}
		return true;
	}
}