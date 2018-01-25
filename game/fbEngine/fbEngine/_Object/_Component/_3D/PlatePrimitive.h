#pragma once
#include "_Object\_Component\Component.h"

class Vertex;
namespace fbEngine
{
	enum BlendModeE
	{
		Trans = 1,	//乗算合成
		Add,		//加算合成
	};
}
//板ポリゴンを出す
class PlatePrimitive :public Component
{
public:
	PlatePrimitive(GameObject* g, Transform* t);
	void Awake()override;
	void Start()override;
	void Update()override;
	void Render()override;

	void SetBillboard(const bool& f);
	void SetBlendMode(fbEngine::BlendModeE mode);
	void SetTexture(TEXTURE* t);
	void SetSize(const Vector2& size);
	Vector2& GetSize()
	{
		return _Size;
	}
	void SetPivot(const Vector2& v2);
	void SetPivot(float x, float y);
	void SetUV(const Vector4& uv);
	void SetBlendColor(const Color& c);
	const Color& GetBlendColor() const;

	/**
	* 溢れ輝度を書き込むかのフラグ.
	*/
	void SetIsLuminance(bool value)
	{
		_IsLuminance = value;
	}

	/**
	* 色々な色の輝度を設定できるよ.
	* aに1を入れると書き込まれるよ.
	*/
	void SetLuminanceColor(Color lumColor)
	{
		_LuminanceColor = lumColor;
	}

private:
	//ビルボードするかどうか？
	bool _IsBillboard;
	//ブレンドモード
	fbEngine::BlendModeE _BlendMode;
	//シェーダーエフェクト
	Effect* _Effect;
	//大きさ
	Vector2 _Size;
	//ピボット
	Vector2 _Pivot;
	//UVxyはuをzwはvを表す。
	Vector4 _UV;
	//テクスチャー
	LPDIRECT3DTEXTURE9 _Texture;
	//カメラ
	Camera* _Camera;
	//ライト
	Light* _Light;
	//混ぜる色
	Color _BlendColor;

	/** 輝度の色. */
	Color _LuminanceColor = Color(0.0f, 0.0f, 0.0f, 0.0f);

	/** 溢れ輝度を書き込むかフラグ. */
	bool _IsLuminance = true;


	static Vertex* _Vertex;	//頂点
};