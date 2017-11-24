#include"fbstdafx.h"
#include "TextureManager.h"

map<UINT64, TEXTURE*> TextureManager::_TextureMap;

//�֐��̑O����`
HRESULT SetTexture(TEXTURE& tex, char* path);

TextureManager::~TextureManager()
{
	map<UINT64, TEXTURE*>::iterator it = _TextureMap.begin();

	while (it != _TextureMap.end())
	{
		//�e�N�X�`�����
		it->second->Release();
	}
}

//�o�^�A�ǂݍ��݂��s��
TEXTURE* TextureManager::LoadTexture(const char* filename)
{
	TEXTURE* tex;

	UINT64 hush = Support::MakeHash(filename);
	//��v������̂Ȃ�
	if (_TextureMap[hush] == nullptr)
	{
		tex = new TEXTURE();
		char path[128] = "Asset/Texture/";
		//�p�X�̍쐬
		strcat_s(path, strlen(path) + strlen(filename)+1, filename);

		//�e�N�X�`�����ǂݍ��݂���������Γo�^
		if (SUCCEEDED(SetTexture(*tex,path)))
		{
			//map�ɓo�^
			_TextureMap[hush] = tex;
		}
		//�ǂݍ��ݎ��s(�摜���Ȃ��Ƃ�)
		else
		{
			string p = path;
			string error = "�摜�p�X�F" + p + " �̉摜�̓ǂݍ��݂Ɏ��s���܂����B";
			
			MessageBoxA(NULL, error.c_str(), "error:�摜�ǂݍ��݃G���[", MB_OK);
			tex = nullptr;
		}
	}
	else
	{
		tex = _TextureMap[hush];
	}

	return tex;
}

IDirect3DBaseTexture9 * TextureManager::LoadBaseTexture(char * filepath)
{
	char path[128] = "Asset/";
	//�p�X�̍쐬
	strcat(path, filepath);

	//�e�N�X�`���̏��
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFileA(path, &info);

	//���擾
	D3DXGetImageInfoFromFile(
		path,	//�e�N�X�`���p�X
		&info	//���i�[��
	);
	IDirect3DBaseTexture9* texture = nullptr;
	//�e�N�X�`���̃^�C�v�ɍ������ǂݍ��ݕ��@
	switch (info.ResourceType)
	{
	case _D3DRESOURCETYPE::D3DRTYPE_TEXTURE:
		//�ʏ�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(graphicsDevice(), path, (LPDIRECT3DTEXTURE9*)&texture);
		break;
	case _D3DRESOURCETYPE::D3DRTYPE_CUBETEXTURE:
		//�L���[�u�e�N�X�`���ǂݍ���
		D3DXCreateCubeTextureFromFile(graphicsDevice(), path, (LPDIRECT3DCUBETEXTURE9*)&texture);
		break;
	default:
		break;
	}
	return texture;
}

HRESULT SetTexture(TEXTURE& tex,char* path)
{
	//�e�N�X�`���̏��
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFileA(path,&info);
	
	//���擾
	D3DXGetImageInfoFromFile(
		path,	//�e�N�X�`���p�X
		&info	//���i�[��
	);
	HRESULT hr;
	////�e�N�X�`���̃^�C�v�ɍ������ǂݍ��ݕ��@
	//switch (info.ResourceType)
	//{
	//case _D3DRESOURCETYPE::D3DRTYPE_TEXTURE:
	//	//�ʏ�e�N�X�`���ǂݍ���
	//	hr = D3DXCreateTextureFromFile(graphicsDevice(), path, (LPDIRECT3DTEXTURE9*)&tex.pTexture);
	//	break;
	//case _D3DRESOURCETYPE::D3DRTYPE_CUBETEXTURE:
	//	//�L���[�u�e�N�X�`���ǂݍ���
	//	hr = D3DXCreateCubeTextureFromFile(graphicsDevice(), path, (LPDIRECT3DCUBETEXTURE9*)&tex.pTexture);
	//	break;
	//default:
	//	break;
	//}

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
		D3DX_FILTER_LINEAR,			//�t�B���^�����O�̎w�� ���`�⊮���w��
		D3DX_DEFAULT,				//�~�b�v�t�B���^�[�H
		0x00000000,					//�����ɂ���F(�����ȍ����w��)
		&info,						//�t�@�C���̃f�[�^���i�[����|�C���^
		NULL,					//�p���b�g�H�H�H
		(LPDIRECT3DTEXTURE9*)&tex.pTexture);				//�쐬���ꂽ�e�N�X�`���ւ̃|�C���^

	// �e�N�X�`���T�C�Y�̎擾
	tex.Size = Vector2((float)info.Width, (float)info.Height);

	return hr;
}
