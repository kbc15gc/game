#pragma once
#include "_Object\_Component\Component.h"

//方向ライト
struct DirectionalLight
{
public:
	DirectionalLight()
	{
		_Rotation = Vector3::zero;
		_Color = Color::white;
	}
	//オイラー角で設定
	void SetEulerAngles(const Vector3& angle)
	{
		//奥方向ベクトル
		D3DXVECTOR3 in = { 0.0f,0.0f,1.0f };
		D3DXVECTOR3 out;
		//回転行列作成
		D3DXMATRIX rotateMatrix;
		D3DXMatrixRotationYawPitchRoll(&rotateMatrix, D3DXToRadian(angle.y), D3DXToRadian(angle.x), D3DXToRadian(angle.z));
		//ベクトルを回転行列を使って回転
		D3DXVec3TransformCoord(&out, &in, &rotateMatrix);
		//セットして正規化
		_Rotation.Set(out);
		_Rotation.Normalize();
	}
	//向きで設定
	void SetDirection(const Vector3& dir)
	{
		_Rotation = dir;
	}
	//ライトの色を設定
	void SetColor(const Color& color)
	{
		_Color = color;
	}
	const Color& GetColor()
	{
		return _Color;
	}
	//方向を返す
	Vector4 Direction() const
	{
		return Vector4(_Rotation.x, _Rotation.y, _Rotation.z, 1.0f);
	}
private:
	Vector3 _Rotation;	//回転
	Color _Color;	//色
};

class Light :public Component
{
public:
	Light::Light(GameObject* g, Transform* t) :Component(g, t, typeid(this).name())
	{
		
	}
	Light::~Light();
	void Awake()override;
	//ライトの追加
	void AddLight(DirectionalLight* l);
	//ベクター取得
	const vector<DirectionalLight*>& GetLight();
	//ライトの数取得
	const int GetNum()const;

	/**
	* 環境光を取得.
	*/
	const Vector3& GetAmbientLight()
	{
		return _AmbientLight;
	}

	/**
	* 環境光を設定.
	*/
	void SetAmbientLight(const Vector3& amb)
	{
		_AmbientLight = amb;
	}

	void SetPointLightPosition(const Vector3& pos)
	{
		_PointLightPos = pos;
	}

	void SetPointLightParam(const Vector4& param)
	{
		_PointLightParam = param;
	}

	const Vector3& GetPointLightPosition()
	{
		return _PointLightPos;
	}

	const Vector4& GetPointLightParam()
	{
		return _PointLightParam;
	}

private:
	//ライトのポインタ達
	vector<DirectionalLight*> _LightVec;
	
	/** 環境光. */
	Vector3 _AmbientLight = Vector3(0.3f, 0.3f, 0.3f);

	/** ポイントライトの色と強さ. */
	Vector4 _PointLightParam = Vector4(0, 0, 0, 0);
	/** ポイントライトの座標. */
	Vector3 _PointLightPos = Vector3::zero;

};

/**
* キャラクターライトクラス.
*/
class CharacterLight 
{
public:

	/** ライトの数. */
	static const USHORT LIGHT_NUM = 4;

public:

	/**
	* コンストラクタ.
	*/
	CharacterLight()
	{
		for (int i = 0; i < LIGHT_NUM; i++)
		{
			DiffuseLightDirection_[i] = Vector4(0, 0, 0, 0);
			DiffuseLightColor_[i] = Vector4(0, 0, 0, 0);
		}
		AmbientLight_ = Vector4(0, 0, 0, 0);
	}

	/**
	* デストラクタ.
	*/
 	~CharacterLight()
	{
	}

	/**
	* ディフューズライトの向きを設定.
	*
	* @param no	   ライトの番号.
	* @param lightDir ライトの方向、正規化済みベクトル.
	*/
	void SetDiffuseLightDirection(int no, const Vector3& lightDir)
	{
		DiffuseLightDirection_[no] = Vector4(lightDir.x, lightDir.y, lightDir.z, 1.0f);
	}

	/**
	* ディフューズライトの向きを取得.
	*
	* @param no ライトの番号.
	*
	* @return The diffuse light direction.
	*/
	const Vector4& GetDiffuseLightDirection(int no) const
	{
		return DiffuseLightDirection_[no];
	}

	/**
	* ディフューズライトの色を設定.
	*
	* @param no	   ライトの番号.
	* @param lightColor ライトの色、0.0f～1.0fの値.
	*/
	void SetDiffuseLightColor(int no, const Vector4& lightColor)
	{
		DiffuseLightColor_[no] = lightColor;
	}

	/**
	* ディフューズライトの色を取得.
	*
	* @param no The ライトの番号.
	*
	* @return The diffuse light color.
	*/
	const Vector4& GetDiffuseLightColor(int no) const
	{
		return DiffuseLightColor_[no];
	}

	/**
	* アンビエントライトの強さを設定.
	*
	* @param amb アンビエントの色.
	*/
	void SetAmbientLight(const Vector4& amb)
	{
		AmbientLight_ = amb;
	}

	/**
	* アンビエントライトを取得.
	*
	* @return The ambient light.
	*/
	const Vector4& GetAmbientLight() const
	{
		return AmbientLight_;
	}

private:

	/** 平行光源の方向. */
	Vector4 DiffuseLightDirection_[LIGHT_NUM];
	/** 平行光源の色. */
	Vector4	DiffuseLightColor_[LIGHT_NUM];
	/** 環境光. */
	Vector4	AmbientLight_;
	/** ライト数. */
	int LightCount_ = LIGHT_NUM;

};