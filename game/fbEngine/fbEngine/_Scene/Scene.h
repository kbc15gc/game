#pragma once

class Vertex;
class Effect;

namespace fbScene
{
	//フェードのステート
	enum class FadeStateE
	{
		WAIT,		//待機
		START,		//開始
		RUNNING,	//実行中
		END	=100,	//終了
		INEND,		//フェードインの終了
		OUTEND,		//フェードアウトの終了
	};
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

	//フェード
	//フェード中はtrueが帰る。
	void Fade();
	//フェードの実行
	//第一引数はフェードインかフェードアウトか指定
	//第二引数はフェードにかかる時間(秒)
	static void StartFade(const bool& fade,const float& fadetime = 2.0f);
	//待機状態へ移行する
	static void WaitFade()
	{
		_FadeState = fbScene::FadeStateE::WAIT;
	}

	/**
	* シャドウマップの有効フラグ.
	*/
	bool GetIsShadowMap()
	{
		return _isShadowMap;
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
	//フェード中
	static fbScene::FadeStateE _FadeState;

protected:

	/** シャドウマップフラグ. */
	bool _isShadowMap = false;


};
