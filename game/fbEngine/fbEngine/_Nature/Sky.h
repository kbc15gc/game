/**
* 空描画クラスの定義.
*/
#pragma once

#include"_Object\_GameObject\Plate.h"

static const float PI = 3.14159265358979323846f;

/**
* 大気散乱用パラメータ.
*/
#pragma pack (4)
struct AtmosphericScatteringParamS
{
	/**
	* コンストラクタ.
	*/
	AtmosphericScatteringParamS();

	Vector3 v3LightPos;
	Vector3 v3LightDirection;
	Vector3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
	float fCameraHeight;		// The camera's current height
	float fCameraHeight2;		// fCameraHeight^2
	float fOuterRadius;		// 大気圏を含む半径.
	float fOuterRadius2;		// fOuterRadius^2
	float fInnerRadius;		// 地球の半径
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
	* 更新.
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
* 空描画クラス.
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
	* コンストラクタ
	*/
	Sky(const char* name) : 
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~Sky()
	{
	}

	/**
	* 作成.
	*/
	void Awake()override;

	/**
	* 更新.
	*/
	void Update()override;

	/**
	* 描画.
	*/
	void Render()override;

	/**
	* 空のシミュレーションを有効にする.
	*/
	void SetEnable()
	{
		SetActive(true);
		_SunPlate->SetActive(true);
		_MoonPlate->SetActive(true);
	}

	/**
	* 太陽の座標を取得.
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
	* 日中の環境光を設定.
	*/
	void SetDayAmbientLight(const Vector3& ambient)
	{
		_DayAmbientLight = ambient;
	}

	/**
	* 夜間の環境光を設定.
	*/
	void SetNightAmbientLight(const Vector3& ambient)
	{
		_NightAmbientLight = ambient;
	}

	/**
	* 大気散乱用パラメータを取得.
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
	* モードを設定.
	*/
	void SetSunMode(SunMode mode, float angle = 0)
	{
		_SunMode = mode;
		_SunAngle = angle;
	}

private:

	/** 空モデル. */
	SkinModel* _SkyModel = nullptr;

	Plate* _SunPlate = nullptr;
	Plate* _MoonPlate = nullptr;

	/** 大気散乱用パラメータ. */
	AtmosphericScatteringParamS _AtomosphereParam;

	SunMode _SunMode;

	/** 太陽の座標. */
	Vector3 _SunPosition = Vector3::zero;
	/** 太陽の角度. */
	float _SunAngle = 0.0f;
	/** 太陽の方向. */
	Vector3 _SunDir = Vector3::zero;

	/** 日中の環境光. */
	Vector3 _DayAmbientLight = Vector3(0.3f, 0.3f, 0.3f);
	/** 夜間の環境光. */
	Vector3 _NightAmbientLight = Vector3(0.1f, 0.1f, 0.1f);

	Vector3 _ShadowLightPosition;

	LPDIRECT3DCUBETEXTURE9 _NightTexture = nullptr;
};