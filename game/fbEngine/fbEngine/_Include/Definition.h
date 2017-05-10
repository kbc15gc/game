#pragma once
//�֗���define��`

//�C���X�^���X�����[�X
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p) = NULL; } }
//�C���X�^���X�폜
#define SAFE_DELETE(p)			{ if(p!=nullptr) { delete (p);     (p) = nullptr; } }
//�C���X�^���X�z��폜
#define SAFE_DELETE_ARRAY(p)	{ if(p!=nullptr) { delete[] (p);   (p) = nullptr; } }

//�z��̗v�f���𒲂ׂ�
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
//�ϐ�count���g����num��J��Ԃ�
#define FOR(count,num) for(int count = 0;count < (int)num;count++)
//1��n�r�b�g���炵�����̂�Ԃ�(�}�X�N�Ƃ��Ŏg���Ƃ�������)
#define BIT(n) ( 1 << (n) )
//�V���O���g���擾
#define INSTANCE(p) p::Instance()
//�󂯎�����l�𕶎���ɕϊ�(enum��ϐ������\�B)
#define STRING(var) #var