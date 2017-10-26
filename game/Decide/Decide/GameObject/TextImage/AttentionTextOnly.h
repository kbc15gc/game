#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
class TextObject;

//注意文だけを表示するクラス。
class AttentionTextOnly :
	public GameObject
{
public:

	//テキストの移動方向。
	enum MoveType
	{
		Invalid = -1,
		Not = 0,		//移動しない(初期値)。
		Up,				//上。
		Down,			//下。
		Max
	};
	//コンストラクタ。
	AttentionTextOnly(const char* name);

	//デストラクタ。
	~AttentionTextOnly();
	
	//テキストを作成。
	//引数：文字列、座標,サイズ、カラー、移動させたいならNot以外を選択(何も指定しない場合は移動しない)。
	void CreateText(const wchar_t * string,
		const Vector3& textPos,
		float size,
		const Color& color,
		MoveType type = MoveType::Not);

	//更新。
	void Update()override;

	//テキストリストの全削除。
	void DeleteList();
private:
	//vectorに積まれた各テキストの情報格納用。
	struct TextInfo {
		TextObject* _Text;	//テキスト。
		Color		_Color;	//テキストのカラー。
		Vector3		_Dir;	//移動する方向。
	};
private:
	//テキストリスト。
	vector<TextInfo*>	_TextList;

	//テキストの移動量
	float				_MoveSpeed = 0.001f;

	//Vector3(1000.0f,250.0f,0.0f),33.0fアイテムの効果が無いテキストの丁度良い感じの設定。
};

namespace {
	static Vector3 MoveTypeS[static_cast<int>(AttentionTextOnly::MoveType::Max)] = {
		{0.0f,0.0f,0.0f},		//移動しない。
		{ 0.0f,-1.0f,0.0f },	//上。　
		{ 0.0f,1.0f,0.0f },		//下。
	};
}