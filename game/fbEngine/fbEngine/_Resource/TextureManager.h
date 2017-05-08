#pragma once
#include "_Object/Noncopyable.h"

//�e�N�X�`���\����
struct TEXTURE :Noncopyable
{
	IDirect3DTexture9* pTexture;	//�e�N�X�`���ւ̃|�C���^
	Vector2 Size;					//�摜�T�C�Y

	//�R���X�g���N�^
	TEXTURE::TEXTURE()
	{
		pTexture = nullptr;
	}

	//�e�N�X�`���ւ̃|�C���^�������[�X����
	void TEXTURE::Release()
	{
		SAFE_DELETE(pTexture);
	}

};	

//�ǂݍ��񂾃e�N�X�`�����Ǘ�
class TextureManager
{
public:
	~TextureManager();

	//�O������g���p
	//�����Ƃ��Ď󂯎�����t�@�C��������e�N�X�`���̏����擾���Ԃ��֐�
	//�摜�t�@�C����Image�t�H���_�ɓ����Ă��镨�̂ݓǂݍ��߂�
	//����:char* �摜�t�@�C���̖��O(�g���q�܂�)
	static TEXTURE* LoadTexture(char* filename);
	//�e�N�X�`����ǂݎ��
	//�ǂݍ��ނ����Ń}�b�v�ɂ͓o�^����Ȃ�
	//�t�@�C���p�X��"Asset/"����̃p�X������
	static IDirect3DBaseTexture9* LoadBaseTexture(char* filepath);
private:
	//�e�N�X�`�����i�[����map
	static map<UINT64,TEXTURE*> _TextureMap;
};

//�e�N�X�`���ǂݍ���
#define LOADTEXTURE(filename) TextureManager::LoadTexture(filename)