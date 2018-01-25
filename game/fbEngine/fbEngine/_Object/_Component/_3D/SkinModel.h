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
	DITHERING = BIT(9),			//!< �f�B�U�����O.
	RECEIVE_POINTLIGHT = BIT(10),	//�|�C���g���C�g�𗎂Ƃ�.
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

/**
* �t�H�O�p�����[�^.
*/
enum class FogFunc
{
	FogFuncNone,	// �t�H�O�Ȃ�.
	FogFuncDist,	// �����t�H�O.
	FogFuncHeight,	// �����t�H�O.
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
	void SetSky()
	{
		_IsSky = true;
		SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncSkyFromAtomosphere);
	}
	//void SetTextureBlend(const Color& c)
	//{
	//	_TextureBlend = c;
	//}

	// ���f���ɏ�Z����J���[����ݒ�B
	void SetAllBlend(const Color& c)
	{
		_AllBlend = c;
	}
	inline const Color& GetAllBlend()const {
		return _AllBlend;
	}

	/**
	* �A���t�@�̂������l��ݒ�(���f���̕s�����x�ł͂Ȃ��_�ɒ���).
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
		_IsTree = true;
		SetModelEffect(ModelEffectE::DITHERING, true);
	}

	/**
	* �f�B�U�W����ݒ肷��.
	* 0~65�̒l������.
	* 0������Ə����Ȃ�.
	* �l���傫���Ȃ�قǏ����Ă���.
	* ���f���G�t�F�N�g�̕����ݒ肵�Ȃ��ƈӖ��Ȃ���H
	* 
	* ��삱�ꂾ��B���ꂱ��B����ł����Ă΁B
	*/
	void SetDitherCoefficient(float value)
	{
		_DitherCoefficient = value;
	}

	/**
	* �t�H�O�p�����[�^��ݒ�.
	*
	* @param fogfunc �t�H�O�̎��.
	* @param idx0    �t�H�O��������n�߂鋗��.
	* @param idx1    �t�H�O�������肫�鋗��.
	* @param color	�@�t�H�O�̐F
	*/
	void SetFogParam(FogFunc fogFunc, float idx0, float idx1, const Vector4& color, bool isNight = false)
	{
		_FogFunc = fogFunc;
		_FogParam[0] = idx0;
		_FogParam[1] = idx1;
		_FogParam[3] = isNight;
		_FogColor = color;
	}

	/**
	* ���P�x���������ނ��̃t���O.
	*/
	void SetIsLuminance(bool value)
	{
		_IsLuminance = value;
	}

	/**
	* �F�X�ȐF�̋P�x��ݒ�ł����.
	* a��1������Ə������܂���.
	*/
	void SetLuminanceColor(Color lumColor)
	{
		_LuminanceColor = lumColor;
	}

	/**
	* �t���l�����˂̃p�����[�^��ݒ�.
	*/
	void SetFresnelParam(bool is, const Vector4& param = Vector4(1.0f, 1.0f, 1.0f, 1.5f))
	{
		_IsFresnel = is;
		_FresnelParam = param;
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
	Color /*_TextureBlend,*/ _AllBlend;	// ���f���̍ŏI�I�ȃs�N�Z���J���[�ɏ�Z����l(�����ɂ���ꍇ�͂��̒l�̃A���t�@������0�ɂ���)�B

	/** �L�����N�^�[���C�g. */
	CharacterLight* _CharaLight = nullptr;

	//�G�t�F�N�g��`���邩�ǂ����̃t���O
	ModelEffectE _ModelEffect;

	bool _IsSky = false;

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

	/** ��. */
	bool _IsTree = false;

	/** �f�B�U�W��. */
	float _DitherCoefficient = 0;

	/** �t�H�O�̎��. */
	FogFunc _FogFunc = FogFunc::FogFuncNone;
	/** �t�H�O�̃p�����[�^. */
	float _FogParam[2];
	/** �t�H�O�̐F. */
	Vector4 _FogColor = Vector4(0, 0, 0, 1);

	/** �P�x�̐F. */
	Color _LuminanceColor = Color(0.0f, 0.0f, 0.0f, 0.0f);

	/** ���P�x���������ނ��t���O. */
	bool _IsLuminance = true;

	/** �t���l�����˂��s���t���O. */
	bool _IsFresnel = false;
	/** �t���l�����˂̃p�����[�^. */
	Vector4 _FresnelParam = Vector4(1.0f, 1.0f, 1.0f, 1.5f);

};