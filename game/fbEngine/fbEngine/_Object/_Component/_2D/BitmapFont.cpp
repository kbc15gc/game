#include "fbstdafx.h"
#include "BitmapFont.h"
#include "_Effect\EffectManager.h"
#include "_Object\Vertex.h"

BitmapFont::BitmapFont(GameObject * g, Transform * t) :
	Component(g, t, typeid(this).name())
{	
}

void BitmapFont::Start()
{
	//後で上下反転させるのであえて左回りにつくる。
	//ポジション定義
	VERTEX_POSITION position[] = {
		{ 0.0f, 0.0f, 0.0f, 1.0f },//左下
		{ 1.0f, 0.0f, 0.0f, 1.0f },//右下
		{ 0.0f, 1.0f, 0.0f, 1.0f },//左上
		{ 1.0f, 1.0f, 0.0f, 1.0f },//右上
	};

	//ポジションの頂点バッファ作成
	(*graphicsDevice()).CreateVertexBuffer(
		sizeof(VERTEX_POSITION) * 4,	//頂点バッファのサイズ(サイズ * 頂点数)
		D3DUSAGE_WRITEONLY,
		0,								//固定機能ではないので非ＦＶＦ頂点で作成する
		D3DPOOL_MANAGED,
		&_PosBuffer,					//頂点バッファ
		NULL
	);

	D3DVERTEXBUFFER_DESC desc;
	_PosBuffer->GetDesc(&desc);
	//頂点バッファの先頭アドレスへの参照を格納する。
	VERTEX_POSITION* pData;
	//アドレス取得
	_PosBuffer->Lock(
		0,						//ロックする先頭
		desc.Size,				//ロックする量
		(void**)&pData,			//先頭アドレス
		D3DLOCK_DISCARD			//フラグ
	);
	vector<VERTEX_POSITION> uv;
	//アドレスの参照へ頂点定義をコピー
	memcpy(pData, position, desc.Size);
	//ロック解除
	_UVBuffer->Unlock();


	/*
	頂点データのレイアウトを定義します。
	typedef struct D3DVERTEXELEMENT9 {
    WORD Stream;		ストリーム番号
    WORD Offset;		頂点データの先頭から、特定のデータ型に関連付けられるデータまでのオフセット。
    BYTE Type;			D3DDECLTYPE として指定されているデータ型。データのサイズを定義する
    BYTE Method;		テッセレータによる頂点データの解釈 「D3DDECLMETHOD」を参照してください。
    BYTE Usage;			シェーダーのセマンティックス。
    BYTE UsageIndex;	セマンティックスの添え字。(TEXCOORD"0",TEXCOORD"1")
	} D3DVERTEXELEMENT9, *LPD3DVERTEXELEMENT9;
	*/

	//頂点宣言(頂点がどのように構成されているか)
	D3DVERTEXELEMENT9 elements[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // 頂点座標
		{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, // UV
		D3DDECL_END()	//終端。
	};
	
	//デコレーション作成
	(*graphicsDevice()).CreateVertexDeclaration(elements, &_Declaration);
}

void BitmapFont::Update()
{
	//文字列に対応したUVを積む。
	
	
}

void BitmapFont::Render()
{
	//シェーダファイル読み込み
	_Effect = EffectManager::LoadEffect("BitmapFont.fx");

	D3DXMATRIX  matWorld, matSize, matScale, matRot, matTrans;
	//画像のサイズを設定
	D3DXMatrixScaling(&matSize, _TextSize.x, _TextSize.y, 1.0f);
	//設定されたスケールを設定
	D3DXMatrixScaling(&matScale, transform->GetScale().x, transform->GetScale().y, transform->GetScale().z);
	//回転
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(transform->GetAngle().z));
	//移動
	D3DXMatrixTranslation(&matTrans, transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z);

	//画像サイズ　*　スケール　*　回転　*　ポジション
	matWorld = matSize * matScale *matRot * matTrans;

	RECT rect;
	//ウィンドウハンドルからクライアントエリアのサイズを取得
	HWND hWnd = FindWindow("DECIDE", NULL);
	GetClientRect(hWnd, &rect);
	float w = (float)rect.right;
	float h = (float)rect.bottom;

	//射影変換行列
	//スクリーンのサイズに収める。
	D3DXMATRIX proj(
		2.0f / w, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0 / -h, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	);

	D3DXMATRIX WP = matWorld * proj;

	//テクニック設定
	_Effect->SetTechnique("SpriteTech");

	_Effect->Begin(NULL, 0);
	_Effect->BeginPass(0);

	//ワールドプロジェクション行列行列
	_Effect->SetMatrix("g_WP", &WP);
	//テクスチャ
	_Effect->SetTexture("g_Bitmap", _Bitmap->pTexture);

	//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。
	_Effect->CommitChanges();
	
	//描画。
	{
		//周波数パラメータなるものを設定。
		//(*graphicsDevice()).SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | Stack.size());
		//(*graphicsDevice()).SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
		//デコレーション設定。
		(*graphicsDevice()).SetVertexDeclaration(_Declaration);
		//ストリームセット。
		(*graphicsDevice()).SetStreamSource(0, _PosBuffer, 0, sizeof(VERTEX_POSITION));
		(*graphicsDevice()).SetStreamSource(1, _UVBuffer, 0, sizeof(Vector4));
		//描画。
		(*graphicsDevice()).DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_LINESTRIP, 0, 4);
	}

	_Effect->EndPass();
	_Effect->End();

	//後始末
	(*graphicsDevice()).SetStreamSourceFreq(0, 1);
	(*graphicsDevice()).SetStreamSourceFreq(1, 1);
}

void BitmapFont::LoadFont(string filename)
{
	string filepath = "Font/" + filename;
	//テクスチャロード。
	_Bitmap = TextureManager::LoadTexture(filepath.c_str());
}

void BitmapFont::SetText(string text)
{
	_Text = text;
	//UVの設定。

	D3DVERTEXBUFFER_DESC desc;
	_UVBuffer->GetDesc(&desc);
	//頂点バッファの先頭アドレスへの参照を格納する。
	VERTEX_POSITION* pData;
	//アドレス取得
	_UVBuffer->Lock(
		0,						//ロックする先頭
		desc.Size,				//ロックする量
		(void**)&pData,			//先頭アドレス
		D3DLOCK_DISCARD			//フラグ
	);
	vector<VERTEX_POSITION> uv;
	for (int i = 0; i < uv.size(); i++) {

		*pData = uv[i];
		pData++;
	}
	//ロック解除
	_UVBuffer->Unlock();
}