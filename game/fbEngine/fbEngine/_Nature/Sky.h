/**
* ��`��N���X�̒�`.
*/
#pragma once

#include"_Object\_GameObject\Plate.h"

static const float PI = 3.14159265358979323846f;

/**
* ��C�U���p�p�����[�^.
*/
#pragma pack (4)
struct AtmosphericScatteringParamS
{
	/**
	* �R���X�g���N�^.
	*/
	AtmosphericScatteringParamS();

	Vector3 v3LightPos;
	Vector3 v3LightDirection;
	Vector3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
	float fCameraHeight;		// The camera's current height
	float fCameraHeight2;		// fCameraHeight^2
	float fOuterRadius;		// ��C�����܂ޔ��a.
	float fOuterRadius2;		// fOuterRadius^2
	float fInnerRadius;		// �n���̔��a
	float fInnerRadius2;		// fInnerRadius^2
	float fKrESun;				// Kr * ESun
	float fKmESun;				// Km * ESun
	float fKr4PI;				// Kr * 4 * PI
	float fKm4PI;				// Km * 4 * PI
	float fScale;				// 1 / (fOuterRadius - fInnerRadius)
	float fScaleDepth;
	float fScaleOverScaleDepth;// fScale / fScaleDepth
	float g;
	float g2;
	
	/**
	* �X�V.
	*/
	void Update(const Vector3& cameraPos, const Vector3& sunPos) 
	{
		v3LightPos = sunPos;
		v3LightPos.Scale(0.001f);
		v3LightDirection = v3LightPos;
		v3LightDirection.Normalize();
		Vector3 cameraPosLocal = cameraPos;
		cameraPosLocal.Scale(0.001f);
		cameraPosLocal.y += fInnerRadius;
		fCameraHeight = cameraPosLocal.Length();
		fCameraHeight2 = fCameraHeight * fCameraHeight;
	}
};
#pragma pack ()

/**
* ��`��N���X.
*/
class Sky : public GameObject
{
public:

	enum class SunMode
	{
		Move,
		Stop,
	};

	/**
	* �R���X�g���N�^
	*/
	Sky(const char* name) : 
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~Sky()
	{
	}

	/**
	* �쐬.
	*/
	void Awake()override;

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* �`��.
	*/
	void Render()override;

	/**
	* ��̃V�~�����[�V������L���ɂ���.
	*/
	void SetEnable()
	{
		SetActive(true);
		_SunPlate->SetActive(true);
		_MoonPlate->SetActive(true);
	}

	/**
	* ���z�̍��W���擾.
	*/
	const Vector3& GetSunPosition() const
	{
		return _SunPosition;
	}

	const Vector3& GetShadowLightPosition()
	{
		return _ShadowLightPosition;
	}

	/**
	* �����̊�����ݒ�.
	*/
	void SetDayAmbientLight(const Vector3& ambient)
	{
		_DayAmbientLight = ambient;
	}

	/**
	* ��Ԃ̊�����ݒ�.
	*/
	void SetNightAmbientLight(const Vector3& ambient)
	{
		_NightAmbientLight = ambient;
	}

	/**
	* ��C�U���p�p�����[�^���擾.
	*/
	const AtmosphericScatteringParamS& GetAtmosphericScatteringParam() const
	{
		return _AtomosphereParam;
	}

	LPDIRECT3DCUBETEXTURE9 GetNightTexture()
	{
		return _NightTexture;
	}

	/**
	* ���[�h��ݒ�.
	*/
	void SetSunMode(SunMode mode, float angle = 0)
	{
		_SunMode = mode;
		_SunAngle = angle;
	}

private:

	/** �󃂃f��. */
	SkinModel* _SkyModel = nullptr;

	Plate* _SunPlate = nullptr;
	Plate* _MoonPlate = nullptr;

	/** ��C�U���p�p�����[�^. */
	AtmosphericScatteringParamS _AtomosphereParam;

	SunMode _SunMode;

	/** ���z�̍��W. */
	Vector3 _SunPosition = Vector3::zero;
	/** ���z�̊p�x. */
	float _SunAngle = 0.0f;
	/** ���z�̕���. */
	Vector3 _SunDir = Vector3::zero;

	/** �����̊���. */
	Vector3 _DayAmbientLight = Vector3(0.3f, 0.3f, 0.3f);
	/** ��Ԃ̊���. */
	Vector3 _NightAmbientLight = Vector3(0.1f, 0.1f, 0.1f);

	Vector3 _ShadowLightPosition;

	LPDIRECT3DCUBETEXTURE9 _NightTexture = nullptr;
};