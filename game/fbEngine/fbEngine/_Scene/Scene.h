#pragma once

class Vertex;
class Effect;

namespace fbScene
{
	//フェードのステート
	enum class FadeStateE
	{
		In = BIT(1),
		Out = BIT(2),
		//実行中。
		Fade = BIT(3),
		FadeIn = Fade | In,
		FadeOut = Fade | Out,
		//フェード終了。
		End	= BIT(4),
		EndFadeIn = End | In,
		EndFadeOut = End | Out,
	};
	FadeStateE operator-(FadeStateE L, FadeStateE R);
	FadeStateE operator|(FadeStateE L, FadeStateE R);
	FadeStateE operator&(FadeStateE L, FadeStateE R);
}

//シーンの基底クラス
class Scene
{
public:
	Scene();
	~Scene();
	//初期化関数
	//シーンが切り替えられた後に呼ばれる
	virtual void Start() {};
	//更新関数
	//切り替えられたら毎フレーム呼び出される処理(いるのか？)
	virtual void Update() {};

	//フェードの処理。
	void Fade();
	//フェードの実行
	//[in] フェードインかどうか？(falseならフェードアウト)
	//[in] フェードにかかる時間(秒)
	static void StartFade(bool fadein,float fadetime = 2.0f);

	/**
	* シャドウマップの有効フラグ.
	*/
	bool GetIsShadowMap()
	{
		return _isShadowMap;
	}

	/**
	* 環境マップフラグを取得.
	*/
	bool GetIsEnvironmentMap()
	{
		return _isEnvironmentMap;
	}

	static fbScene::FadeStateE GetFadeState()
	{
		return _FadeState;
	}

private:
	//フェード描画
	void _DrawFade();
private:
	//全てのシーンで共有したいのでstatic
	//頂点
	static Vertex *_Vertex;
	//シェーダ
	static Effect *_Effect;
	//フェードのα
	static float _FadeAlpha;
	//1秒間あたりに加算されるα値
	static float _AddPerSecA;
protected:
	//フェードのステート
	static fbScene::FadeStateE _FadeState;

protected:

	/** シャドウマップフラグ. */
	bool _isShadowMap = false;

	/** 環境マップフラグ. */
	bool _isEnvironmentMap = false;


};
