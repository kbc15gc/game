#pragma once
#include "Component.h"
#include "SkinModelData.h"

class Effect;
class Camera;
class Light;
class ShadowCamera;

//�G�t�F�N�g�������邩�H
enum ModelEffectE
{
	NONE = 0,				//�Ȃ�
	CAST_SHADOW = 1,		//�e�����
	RECEIVE_SHADOW = 2,		//�e�𗎂Ƃ�
	SPECULAR = 4,			//���ʔ���
	TOON = 8,				//�g�D�[��
};

//���f���̕`����s���N���X
class SkinModel:public Component{
public:
	SkinModel::SkinModel(GameObject* g, Transform* t);
	~SkinModel();

	void Awake()override;
	void LateUpdate()override;
	void PreRender()override;
	void Render()override;

	void SetCamera(Camera* c) 
	{
		_Camera = c;
	}
	void SetLight(Light* l)
	{
		_Light = l;
	}

	//���f���f�[�^�փA�N�Z�X���邽�߂̃|�C���^�ݒ�
	void SetModelData(SkinModelData* pD)
	{
		_ModelDate = pD;
	}
	//���b�V���R���C�_�[���Ƃ���
	LPD3DXMESH GetOrgMeshFirst() const
	{
		return _ModelDate->GetOrgMeshFirst();
	}
	void SetSky(bool f)
	{
		_SkyBox = f;
	}
	void SetTextureBlend(const Color& c)
	{
		_TextureBlend = c;
	}
	void SetAllBlend(const Color& c)
	{
		_AllBlend = c;
	}
	
	//�㏑���Z�b�g
	void SkinModel::SetModelEffect(const ModelEffectE& e)
	{
		_ModelEffect = e;
	}
	//���̏�Ԃɉ��Z�A���Z
	void SkinModel::SetModelEffect(const ModelEffectE& e, const bool& f)
	{
		//���ɗL�����ǂ����H
		if ((_ModelEffect & e) > 0 && f == false)
		{
			//������
			_ModelEffect = ModelEffectE(_ModelEffect - e);
		}
		else if ((_ModelEffect & e) == 0 && f == true)
		{
			//�L����
			_ModelEffect = ModelEffectE(_ModelEffect + e);
		}
	}
private:
	//�q�Ƃ��Z����ꊇ�ŕ`�悷�邽�߂̍ċA�֐�
	void DrawFrame(LPD3DXFRAME pFrame);

	//���f���`��
	void SkinModel::DrawMeshContainer(
		D3DXMESHCONTAINER_DERIVED* pMeshContainerBase,
		LPD3DXFRAME pFrameBase
	);

	//�e�쐬
	void CreateShadow(D3DXMESHCONTAINER_DERIVED* pMeshContainer, D3DXFRAME_DERIVED* pFrame);
private:
	//�G�t�F�N�g�ւ̎Q��
	Effect* _Effect;
	//���f���f�[�^�փA�N�Z�X���邽�߂̃|�C���^�ێ�
	SkinModelData* _ModelDate;
	Camera* _Camera;
	Light* _Light;
	ShadowCamera* _ShadowCamera;
	//�u�����h����F
	Color _TextureBlend, _AllBlend;

	//�G�t�F�N�g��`���邩�ǂ����̃t���O
	ModelEffectE _ModelEffect;
	//�X�J�C�{�b�N�X���ǂ���(��������)
	bool _SkyBox;

	//�ő吔
	static const int MAX_MATRIX_PALLET = 50;
	//�ꎞ�I�Ƀ{�[���s����i�[�����Ɨp�ϐ��H
	D3DXMATRIX _BoneMatrixPallets[MAX_MATRIX_PALLET];
};