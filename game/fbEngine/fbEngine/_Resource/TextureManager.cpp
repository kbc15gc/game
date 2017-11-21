#include"fbstdafx.h"
#include "TextureManager.h"

map<UINT64, TEXTURE*> TextureManager::_TextureMap;

//関数の前方定義
HRESULT SetTexture(TEXTURE& tex, char* path);

TextureManager::~TextureManager()
{
	map<UINT64, TEXTURE*>::iterator it = _TextureMap.begin();

	while (it != _TextureMap.end())
	{
		//テクスチャ解放
		it->second->Release();
	}
}

//登録、読み込みを行う
TEXTURE* TextureManager::LoadTexture(const char* filename)
{
	TEXTURE* tex;

	UINT64 hush = Support::MakeHash(filename);
	//一致するものなし
	if (_TextureMap[hush] == nullptr)
	{
		tex = new TEXTURE();
		char path[128] = "Asset/Texture/";
		//パスの作成
		strcat_s(path, strlen(path) + strlen(filename)+1, filename);

		//テクスチャ情報読み込みが成功すれば登録
		if (SUCCEEDED(SetTexture(*tex,path)))
		{
			//mapに登録
			_TextureMap[hush] = tex;
		}
		//読み込み失敗(画像がないとか)
		else
		{
			string p = path;
			string error = "画像パス：" + p + " の画像の読み込みに失敗しました。";
			
			MessageBoxA(NULL, error.c_str(), "error:画像読み込みエラー", MB_OK);
			tex = nullptr;
		}
	}
	else
	{
		tex = _TextureMap[hush];
	}

	return tex;
}

IDirect3DBaseTexture9 * TextureManager::LoadBaseTexture(char * filepath)
{
	char path[128] = "Asset/";
	//パスの作成
	strcat(path, filepath);

	//テクスチャの情報
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFileA(path, &info);

	//情報取得
	D3DXGetImageInfoFromFile(
		path,	//テクスチャパス
		&info	//情報格納先
	);
	IDirect3DBaseTexture9* texture = nullptr;
	//テクスチャのタイプに合った読み込み方法
	switch (info.ResourceType)
	{
	case _D3DRESOURCETYPE::D3DRTYPE_TEXTURE:
		//通常テクスチャ読み込み
		D3DXCreateTextureFromFile(graphicsDevice(), path, (LPDIRECT3DTEXTURE9*)&texture);
		break;
	case _D3DRESOURCETYPE::D3DRTYPE_CUBETEXTURE:
		//キューブテクスチャ読み込み
		D3DXCreateCubeTextureFromFile(graphicsDevice(), path, (LPDIRECT3DCUBETEXTURE9*)&texture);
		break;
	default:
		break;
	}
	return texture;
}

HRESULT SetTexture(TEXTURE& tex,char* path)
{
	//テクスチャの情報
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFileA(path,&info);
	
	//情報取得
	D3DXGetImageInfoFromFile(
		path,	//テクスチャパス
		&info	//情報格納先
	);
	HRESULT hr;
	////テクスチャのタイプに合った読み込み方法
	//switch (info.ResourceType)
	//{
	//case _D3DRESOURCETYPE::D3DRTYPE_TEXTURE:
	//	//通常テクスチャ読み込み
	//	hr = D3DXCreateTextureFromFile(graphicsDevice(), path, (LPDIRECT3DTEXTURE9*)&tex.pTexture);
	//	break;
	//case _D3DRESOURCETYPE::D3DRTYPE_CUBETEXTURE:
	//	//キューブテクスチャ読み込み
	//	hr = D3DXCreateCubeTextureFromFile(graphicsDevice(), path, (LPDIRECT3DCUBETEXTURE9*)&tex.pTexture);
	//	break;
	//default:
	//	break;
	//}

	//テクスチャ読込　各情報をセット(画像のサイズとか)
	hr = D3DXCreateTextureFromFileEx(
		graphicsDevice(),			//グラフィックデバイスへのポインタ
		path,						//ファイルパス
		D3DX_DEFAULT,				//幅、D3DX_DEFAULT ならファイルから取得
		D3DX_DEFAULT,				//高さ、D3DX_DEFAULT ならファイルから取得
		D3DX_DEFAULT,				//ミップレベル？
		0,							//使い方0以外だとレンダーターゲットとして使えたりする。
		D3DFMT_UNKNOWN,				//フォーマット,D3DFMT_UNKNOWN だとファイルから取得する
		D3DPOOL_DEFAULT,			//テクスチャの配置先のメモリ指定 よくわからんからデフォで
		D3DX_FILTER_LINEAR,			//フィルタリングの指定 線形補完を指定
		D3DX_DEFAULT,				//ミップフィルター？
		0x00000000,					//透明にする色(透明な黒を指定)
		&info,						//ファイルのデータを格納するポインタ
		NULL,					//パレット？？？
		(LPDIRECT3DTEXTURE9*)&tex.pTexture);				//作成されたテクスチャへのポインタ

	// テクスチャサイズの取得
	tex.Size = Vector2((float)info.Width, (float)info.Height);

	return hr;
}
