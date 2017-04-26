#pragma once
#include "_Object/Noncopyable.h"
//�}�e���A���N���X
class Material:Noncopyable
{
public:
	enum TextureHandleE
	{
		DiffuseMap,			//�f�B�t���[�Y�}�b�v�B
		ShadowMap0,			//�V���h�E�}�b�v0
		ShadowMap1,			//�V���h�E�}�b�v1
		ShadowMap2,			//�V���h�E�}�b�v2
		NormalMap,			//�@���}�b�v�B
		SpecularMap,		//�X�y�L�����}�b�v�B
		SplatMap,			//�X�v���b�g�}�b�v�B
		TerrainTex0,		//�n�`�e�N�X�`��0
		TerrainTex1,		//�n�`�e�N�X�`��1
		TerrainTex2,		//�n�`�e�N�X�`��2
		TerrainTex3,		//�n�`�e�N�X�`��3
		TerrainNormalMap0,	//�n�`�̖@���}�b�v0�B
		TerrainNormalMap1,	//�n�`�̖@���}�b�v1�B
		TerrainNormalMap2,	//�n�`�̖@���}�b�v2�B
		TerrainNormalMap3,	//�n�`�̖@���}�b�v3�B
		TextureNum,
	};
public:
	Material(char* name);
	~Material();
	//�e�N�X�`����ݒ�
	void SetTexture(const TextureHandleE& handle, IDirect3DBaseTexture9* tex);
	//�e�N�X�`���擾
	IDirect3DBaseTexture9* GetTexture(const TextureHandleE& handle);
	//������F��ݒ�
	void SetColor(Color color);
	const Color& GetBlendColor();
	void SetName(char* name);
	const char* GetName();
private:
	char* _Name;
	IDirect3DBaseTexture9 *_Textures[TextureHandleE::TextureNum];	//�e�N�X�`������
	Color _BlendColor;	//������F
};