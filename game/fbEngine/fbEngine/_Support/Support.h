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
	//�������������
	extern double StringToDouble(const char* string);
	//�������l�̌ܓ�����
	extern void Round(double& num, const int& decimal = 1);
	//�����񒆂̎w�蕶�����ʂ̕�����ɒu��
	//�ϊ����s�������� in.out
	//�u����������������
	//�u�������镶����
	extern void StrReplace(char* sorce, const char* find, const char* place);

	//�ϐ��̌^���w��B
	enum DataTypeE
	{
		INT,	//����
		FLOAT,	//��������
		VECTOR3,//Vector3
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

	//CSV�t�@�C���������ǂݎ��A�e���v���[�g�N���X�Ɋi�[����B
	template<class T>
	extern bool LoadCSVData(const char* filepath, const Support::DATARECORD* datas, const int& datanum, vector<T*>& output)
	{
		//�t�@�C�����͗p
		std::ifstream fin(filepath, std::ios::in);

		//�t�@�C�����J���Ȃ�������G���[
		if (fin.fail())
		{
			char error[256];
			strcat(error, filepath);
			strcat(error, "\n���J���܂���ł����B");
			MessageBoxA(0, error, "�t�@�C���ǂݍ��݃G���[", MB_ICONWARNING);
			return fin.fail();
		}

		//�t�@�C������f�[�^���󂯎��o�b�t�@
		char line[256], copy[256];

		//�X�L�b�v���鏈��(�v���P)
		fin.getline(line, 256);

		while (true)
		{
			//���������Ă���V�����s��ǂݍ���
			ZeroMemory(line, sizeof(char) * 256);
			fin.getline(line, 256);

			//�t�@�C���̏I�[�ɂȂ����Ȃ甲����B
			if (fin.eof())
				break;

			//csv��"\n"��"//n"�ƔF���������(���{�I�ȉ����ł͂Ȃ�����)�B
			StrReplace(line, "\\n", "\n");

			int idx = 0;		//���Ԗڂ̗v�f���H
			int offset = 0;		//�擪�����offset��
			memcpy(copy, line, sizeof(char) * 256);
			//�A�h���X�m��
			T* tmp = new T();
			//1�s�̏I�[�܂Ń��[�v
			//�͈̓`�F�b�N
			while (*(line + offset) != '\0' && idx < datanum)
			{
				//","��؂�ŒP��𒊏o�A","�̕����ɂ�'\0'�����ߍ��܂��̂ŃR�s�[���g��
				char* word = strtok(copy + offset, ",");

				//�t�@�C������ǂݎ�����l������
				void* val = nullptr;
				int i = 0;
				float f = 0;
				Vector3 v3 = Vector3::zero;
				//�^�ɂ������ϊ�������
				DataTypeE type = datas[idx].type;
				if (type == Support::DataTypeE::INT)
				{
					i = Support::StringToDouble(word);
					val = &i;
				}
				else if (type == Support::DataTypeE::FLOAT)
				{
					f = Support::StringToDouble(word);
					val = &f;
				}
				else if (type == Support::DataTypeE::VECTOR3)
				{
					int offset2 = 0;
					char wordC[256];
					strcpy(wordC, word);
					FOR(i, 3)
					{
						//�����̕��������o��
						char* num = strtok(wordC + offset2, "/");
						//�A�h���X�ɒl�R�s�[
						float value = Support::StringToDouble(num);
						memcpy((char*)(&v3) + (sizeof(float) * i), &value, sizeof(float));
						offset2 += strlen(num) + 1;
					}
					val = &v3;
				}
				else if (type == Support::DataTypeE::STRING)
				{
					val = word;
				}
				else
				{
					val = nullptr;
				}
				

				//�ϐ��̐擪�A�h���X�����߂�Bchar*�^(1byte)�ɃL���X�g���邱�Ƃ�1�Â��炷���Ƃ��ł���B
				auto addres = ((char*)(tmp)) + datas[idx].offset;
				//�A�h���X�ɒl���R�s�[
				memcpy(addres, val, datas[idx].size);

				//�P��̒���+1(","��)���I�t�Z�b�g�ʂɉ��Z
				offset += strlen(word) + 1;
				idx++;
			}
			//�v�f�ǉ�
			output.push_back(tmp);
		}
		//�ǂݍ��ݏI��
		return true;
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
				//�ϐ��̐擪�A�h���X�����߂�Bchar*�^(1byte)�ɃL���X�g���邱�Ƃ�1�Â��炷���Ƃ��ł���B
				auto addres = ((char*)(out)) + datas[idx].offset;

				int i = 0;
				float f = 0;
				//�^�ɂ������ϊ�������
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