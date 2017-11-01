#pragma once
#include "_Object\_Component\Component.h"

class Effect;
class Camera;
class Light;
class CharacterLight;
class ShadowCamera;
class IObjectCulling;

//�G�t�F�N�g�������邩�H
enum ModelEffectE
{
	NONE = BIT(0),				//�Ȃ�
	CAST_SHADOW = BIT(1),		//�e�����
	RECEIVE_SHADOW = BIT(2),	//�e�𗎂Ƃ�
	SPECULAR = BIT(3),			//���ʔ���
	TOON = BIT(4),				//�g�D�[��
	LIMLIGHT = BIT(5),			//�������C�g
	CAST_ENVIRONMENT = BIT(6),	//���}�b�v�����.
	FRUSTUM_CULLING = BIT(7),	//�t���X�^���J�����O���s�����ǂ����H
	ALPHA = BIT(8),				//!< �A���t�@.
};

/**
* ��C�U���̎��.
*/
enum AtmosphereFunc
{
	enAtomosphereFuncNone = 0,				//��C�����V�~�����[�V�����Ȃ��B
	enAtomosphereFuncObjectFromAtomosphere,	//�I�u�W�F�N�g���C�����猩���ꍇ�̑�C�����V�~�����[�V�����B
	enAtomosphereFuncSkyFromAtomosphere,	//����C�����猩���ꍇ�̑�C�����V�~�����[�V�����B
	enAtomosphereFuncNum,
};

//���f���̕`����s���N���X
class SkinModel:public Component{
public:
	SkinModel::SkinModel(GameObject* g, Transform* t);
	~SkinModel();

	void Awake()override;
	void Start()override;
	void LateUpdate()override;
	void PreRender()override;
	void Render()override;

	/**
	* �V���h�E�}�b�v�ɐ[�x����������.
	* �V���h�E�}�b�v�N���X����Ă΂�Ă���.
	*/
	void RenderToShadowMap();

	/**
	* ���}�b�v�ɕ`�悷��.
	* ���}�b�v�N���X����Ă΂�Ă���.
	*/
	void RenderToEnvironmentMap();

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
	SkinModelData* GetModelData() {
		return _ModelDate;
	}

	//���b�V���R���C�_�[���Ƃ���
	LPD3DXMESH GetOrgMeshFirst() const
	{
		return _ModelDate->GetOrgMeshFirst();
	}
	LPD3DXFRAME GetFrameRoot() const
	{
		return _ModelDate->GetFrameRoot();
	}
	void SetSky(bool f)
	{
		_SkyBox = f;
		SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncSkyFromAtomosphere);
	}
	void SetTextureBlend(const Color& c)
	{
		_TextureBlend = c;
	}
	void SetAllBlend(const Color& c)
	{
		_AllBlend = c;
	}

	/**
	* �A���t�@�̂������l��ݒ�.
	*/
	void SetAlpha(bool flag,float a = 0.0f)
	{
		SetModelEffect(ModelEffectE::ALPHA, flag);
		_Alpha = a;
	}

	/**
	* ��C�U���V�~�����[�V�����̎�ނ�ݒ�.
	*/
	void SetAtomosphereFunc(AtmosphereFunc func)
	{
		_AtomosphereFunc = func;
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
	//�w�ʕ`�惂�[�h�̐ݒ�B
	//[in] �`�悷��ʂ̃t���O
	void SetCullMode(D3DCULL flg)
	{
		_CullMode = flg;
	}
	bool terain = false;

	/**
	* �L�����N�^�[���C�g��ݒ�.
	*/
	void SetCharacterLight(CharacterLight* cLight)
	{
		_CharaLight = cLight;
	}

	void SetTree()
	{
		hoge = true;
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
	//�J�����B
	Camera* _Camera;
	//���C�g�B
	Light* _Light;
	//�u�����h����F
	Color _TextureBlend, _AllBlend;

	/** �L�����N�^�[���C�g. */
	CharacterLight* _CharaLight = nullptr;

	//�G�t�F�N�g��`���邩�ǂ����̃t���O
	ModelEffectE _ModelEffect;
	//�X�J�C�{�b�N�X���ǂ���(��������)
	bool _SkyBox;

	//�ő吔
	static const int MAX_MATRIX_PALLET = 50;
	//�ꎞ�I�Ƀ{�[���s����i�[�����Ɨp�ϐ��H
	D3DXMATRIX _BoneMatrixPallets[MAX_MATRIX_PALLET];

	/** ��C�U��. */
	AtmosphereFunc _AtomosphereFunc = AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere;

	//�`�悷���
	D3DCULL _CullMode;

	//�I�u�W�F�N�g�J�����O�B
	IObjectCulling* _Culling;

	/** �A���t�@��臒l. */
	float _Alpha = 0.0f;

	bool hoge = false;
};