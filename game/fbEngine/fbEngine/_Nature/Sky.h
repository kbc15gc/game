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
	AtmosphericScatteringParamS()
	{
		fScaleDepth = 0.25;
		const float fInvScaleDepth = 4;

		const int nSamples = 2;
		const float fSamples = 2.0f;
		const float km = 0.0010f;
		const float ESun = 30.0f;
		const float kr = 0.0025f;
		//大気錯乱パラメータの更新。
		fKm4PI = km * 4.0f * PI;
		fKmESun = km * ESun;

		fKr4PI = kr * 4.0f * PI;
		fKrESun = kr * ESun;
		fInnerRadius = 8.0f; //単位km
		fInnerRadius2 = fInnerRadius * fInnerRadius;
		fOuterRadius = fInnerRadius + (fInnerRadius * fScaleDepth*0.1f);
		fOuterRadius2 = fOuterRadius * fOuterRadius;

		fScale = 1.0f / (fOuterRadius - fInnerRadius);
		fScaleOverScaleDepth = (1.0f / (fOuterRadius - fInnerRadius)) / fScaleDepth;
		g = -0.990f;
		g2 = g * g;
		float fWavelength[3], fWavelength4[3];
		fWavelength[0] = 0.680f;		// 650 nm for red
		fWavelength[1] = 0.570f;		// 570 nm for green
		fWavelength[2] = 0.475f;		// 475 nm for blue
		fWavelength4[0] = powf(fWavelength[0], 4.0f);
		fWavelength4[1] = powf(fWavelength[1], 4.0f);
		fWavelength4[2] = powf(fWavelength[2], 4.0f);
		v3InvWavelength.Set(1 / fWavelength4[0], 1 / fWavelength4[1], 1 / fWavelength4[2]);
	}

	Vector3 v3LightPos;
	Vector3 v3LightDirection;
	Vector3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
	float fCameraHeight;		// The camera's current height
	float fCameraHeight2;		// fCameraHeight^2
	float fOuterRadius;		// The outer (atmosphere) radius
	float fOuterRadius2;		// fOuterRadius^2
	float fInnerRadius;		// The inner (planetary) radius
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

private:

	/** 空モデル. */
	SkinModel* _SkyModel = nullptr;

	Plate* _SunPlate = nullptr;
	Plate* _MoonPlate = nullptr;

	/** 大気散乱用パラメータ. */
	AtmosphericScatteringParamS _AtomosphereParam;

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

	LPDIRECT3DCUBETEXTURE9 _NightTexture = nullptr;
};