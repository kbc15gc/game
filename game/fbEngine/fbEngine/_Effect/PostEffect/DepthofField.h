/**
* DOF(被写界深度)クラスの定義.
*/
#pragma once

/**
* DOF(被写界深度)クラス.
*/
class DepthofField : Noncopyable
{
public:

	/**
	* コンストラクタ.
	*/
	DepthofField()
	{
	}
	
	/**
	* デストラクタ.
	*/
	~DepthofField()
	{
	}

private:

	/** 有効フラグ. */
	bool _isEnable = true;

	/** 深度書き込み用レンダリングターゲット. */
	RenderTarget _DepthRT;



};