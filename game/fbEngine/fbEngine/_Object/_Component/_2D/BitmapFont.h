#pragma once
#include "_Object\_Component\Component.h"

class Vertex;

class BitmapFont :public Component
{
public:
	BitmapFont(GameObject* g, Transform* t);
	void Start()override;
	void Update()override;
	void Render()override;

	void LoadFont(string filename);
	void SetText(string text);
private:
	//エフェクト
	Effect* _Effect;
	//ビットマップフォントのテクスチャ。
	TEXTURE* _Bitmap;
	//文字列。
	string _Text;
	//テキストサイズ。
	Vector2 _TextSize;

	//
	LPDIRECT3DVERTEXBUFFER9 _PosBuffer;
	LPDIRECT3DVERTEXBUFFER9 _UVBuffer;
	//デコレーション
	LPDIRECT3DVERTEXDECLARATION9 _Declaration;	
};