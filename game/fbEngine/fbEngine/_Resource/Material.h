#pragma once
#include "_Object/Noncopyable.h"
//マテリアルクラス
class Material:Noncopyable
{
public:
	enum TextureHandleE
	{
		DiffuseMap,			//ディフューズマップ。
		ShadowMap0,			//シャドウマップ0
		ShadowMap1,			//シャドウマップ1
		ShadowMap2,			//シャドウマップ2
		NormalMap,			//法線マップ。
		SpecularMap,		//スペキュラマップ。
		SplatMap,			//スプラットマップ。
		TerrainTex0,		//地形テクスチャ0
		TerrainTex1,		//地形テクスチャ1
		TerrainTex2,		//地形テクスチャ2
		TerrainTex3,		//地形テクスチャ3
		TerrainNormalMap0,	//地形の法線マップ0。
		TerrainNormalMap1,	//地形の法線マップ1。
		TerrainNormalMap2,	//地形の法線マップ2。
		TerrainNormalMap3,	//地形の法線マップ3。
		TextureNum,
	};
public:
	Material(char* name);
	~Material();
	//テクスチャを設定
	void SetTexture(const TextureHandleE& handle, IDirect3DBaseTexture9* tex);
	//テクスチャ取得
	IDirect3DBaseTexture9* GetTexture(const TextureHandleE& handle);
	//混ぜる色を設定
	void SetColor(Color color);
	const Color& GetBlendColor();
	void SetName(char* name);
	const char* GetName();
private:
	char* _Name;
	IDirect3DBaseTexture9 *_Textures[TextureHandleE::TextureNum];	//テクスチャたち
	Color _BlendColor;	//混ぜる色
};