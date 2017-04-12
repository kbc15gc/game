#pragma once
//�}�e���A���N���X
class Material:Noncopyable
{
public:
	Material();
	~Material();
	//�}�e���A���̕���
	Material* CloneMaterial();
private:
	TEXTURE *_Diffuse;		//�g�U���ˌ�
	Color _BlendDiffuse;	//�g�U���ˌ��ɍ�����F
	TEXTURE *_Specular;		//���ʔ��ˌ�
	TEXTURE *_Normal;		//�@��
	TEXTURE *_Luminance;	//�P�x
	TEXTURE *_Occlusion;	//���̉A
	TEXTURE *_Height;		//����
	Color _Emission;		//���Ȕ���
};