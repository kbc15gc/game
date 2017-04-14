#include "TextureManager.h"

map<UINT64, TEXTURE*> TextureManager::textureMap;

//�֐��̑O����`
HRESULT SetTexture(TEXTURE& tex, char* path);

TextureManager::~TextureManager()
{
	map<UINT64, TEXTURE*>::iterator it = textureMap.begin();

	while (it != textureMap.end())
	{
		//�e�N�X�`�����
		it->second->Release();
	}
}

//�o�^�A�ǂݍ��݂��s��
TEXTURE* TextureManager::LoadTexture(char* filename)
{
	TEXTURE* tex;

	UINT64 hush = Support::MakeHash(filename);
	//��v������̂Ȃ�
	if (textureMap[hush] == nullptr)
	{
		tex = new TEXTURE();
		char path[128] = "Asset/Texture/";
		//�p�X�̍쐬
		strcat_s(path, strlen(path) + strlen(filename)+1, filename);

		//�e�N�X�`�����ǂݍ��݂���������Γo�^
		if (SUCCEEDED(SetTexture(*tex,path)))
		{
			//map�ɓo�^
			textureMap[hush] = tex;
		}
		//�ǂݍ��ݎ��s(�摜���Ȃ��Ƃ�)
		else
		{
			string p = path;
			string error = "�摜�p�X�F" + p + " �̉摜�̓ǂݍ��݂Ɏ��s���܂����B";
			
			MessageBoxA(NULL, error.c_str(), "error:�摜�ǂݍ��݃G���[", MB_OK);
		}
	}
	else
	{
		tex = textureMap[hush];
	}

	return tex;
}

HRESULT SetTexture(TEXTURE& tex,char* path)
{
	D3DXIMAGE_INFO info;
	
	//�߂�l�󂯎��
	HRESULT hr;
	////���擾
	//D3DXGetImageInfoFromFile(
	//	path,	//�e�N�X�`���p�X
	//	&info	//���i�[��
	//);
	////�ǂݍ���
	//hr = D3DXCreateTextureFromFile(
	//	graphicsDevice(),	//�O���t�B�b�N�f�o�C�X
	//	path,				//�e�N�X�`���p�X
	//	&tex.pTexture		//�e�N�X�`���̊i�[��
	//);

	//�e�N�X�`���Ǎ��@�e�����Z�b�g(�摜�̃T�C�Y�Ƃ�)
	hr = D3DXCreateTextureFromFileEx(
		graphicsDevice(),			//�O���t�B�b�N�f�o�C�X�ւ̃|�C���^
		path,						//�t�@�C���p�X
		D3DX_DEFAULT,				//���AD3DX_DEFAULT �Ȃ�t�@�C������擾
		D3DX_DEFAULT,				//�����AD3DX_DEFAULT �Ȃ�t�@�C������擾
		D3DX_DEFAULT,				//�~�b�v���x���H
		0,							//�g����0�ȊO���ƃ����_�[�^�[�Q�b�g�Ƃ��Ďg�����肷��B
		D3DFMT_UNKNOWN,				//�t�H�[�}�b�g,D3DFMT_UNKNOWN ���ƃt�@�C������擾����
		D3DPOOL_DEFAULT,			//�e�N�X�`���̔z�u��̃������w�� �悭�킩��񂩂�f�t�H��
		D3DX_DEFAULT,				//�t�B���^�����O�̎w��
		D3DX_DEFAULT,				//�~�b�v�t�B���^�[�H
		0x00000000,					//�����ɂ���F(�����ȍ����w��)
		&info,						//�t�@�C���̃f�[�^���i�[����|�C���^
		NULL,						//�p���b�g�H�H�H
		&tex.pTexture);				//�쐬���ꂽ�e�N�X�`���ւ̃|�C���^

									// �e�N�X�`���T�C�Y�̎擾
	tex.Size = Vector2((float)info.Width, (float)info.Height);

	return hr;
}
