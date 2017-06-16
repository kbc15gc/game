/**
* 環境マップクラスの実装.
*/
#include"fbstdafx.h"
#include"EnvironmentMap.h"

namespace
{

	/** カメラの向き. */
	D3DXVECTOR3 lookAt[6] =
	{
		D3DXVECTOR3(1.0f,  0.0f,  0.0f),	// +X
		D3DXVECTOR3(-1.0f,  0.0f,  0.0f),	// -X
		D3DXVECTOR3(0.0f,  1.0f,  0.0f),	// +Y
		D3DXVECTOR3(0.0f, -1.0f,  0.0f),	// -Y
		D3DXVECTOR3(0.0f,  0.0f,  1.0f),	// +Z
		D3DXVECTOR3(0.0f,  0.0f, -1.0f) 	// -Z
	};

	/** カメラの上方向. */
	D3DXVECTOR3 up[6] =
	{
		D3DXVECTOR3(0.0f,  1.0f,  0.0f),	// +X(Up = +Y)
		D3DXVECTOR3(0.0f,  1.0f,  0.0f),	// -X(Up = +Y)
		D3DXVECTOR3(0.0f,  0.0f, -1.0f),	// +Y(Up = -Z)
		D3DXVECTOR3(0.0f,  0.0f,  1.0f),	// -X(Up = +Z)
		D3DXVECTOR3(0.0f,  1.0f,  0.0f),	// +Z(Up = +Y)
		D3DXVECTOR3(0.0f,  1.0f,  0.0f),	// -Z(Up = +Y)
	};

}

/**
* 作成.
*/
bool EnvironmentMap::Create()
{

	HRESULT hr;

	//キューブテクスチャの作成.
	hr = (*graphicsDevice()).CreateCubeTexture(
		512,	// テクスチャの長さ.
		1,		// ミップマップレベル.
		D3DUSAGE_RENDERTARGET,	// キューブテクスチャの使われ方.
		D3DFMT_A8R8G8B8,	// フォーマット.
		D3DPOOL_DEFAULT,	// テクスチャの管理先(D3DPOOL_DEFAULTは自分で解放する必要あり).
		&_D3DCubeTexture,	// キューブマップテクスチャのポインタ.
		nullptr	// シェアードハンドル.
		);

	if (FAILED(hr))
	{
		return false;
	}

	//プロジェクション行列の計算.
	D3DXMatrixPerspectiveFovLH(&_ProjMatrix, D3DXToRadian(90.0f), 1.0f, 1.0f, 10000.0f);

	_RenderTime = 0.0f;

	return true;
	
}

/**
* 更新.
*/
void EnvironmentMap::Update()
{
	if (_LocalTime > _RenderTime)
	{
		_isRender = true;
		_LocalTime = 0.0f;
	}

	//ローカルタイムを更新.
	_LocalTime += Time::DeltaTime();
}

/**
* 描画.
*/
void EnvironmentMap::Render()
{
	if (_isRender)
	{
		//キューブテクスチャのレンダリングターゲット.
		IDirect3DSurface9* renderTarget = nullptr;

		for (int i = 0; i < 6; i++)
		{
			//レンダリングターゲットを取得.
			_D3DCubeTexture->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, &renderTarget);
			renderTarget->Release();

			//レンダリングターゲットを設定.
			(*graphicsDevice()).SetRenderTarget(0, renderTarget);
			(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

			Vector3 cameraTarget = INSTANCE(GameObjectManager)->mainCamera->GetTarget();

			D3DXVECTOR3 eye = D3DXVECTOR3(cameraTarget.x, cameraTarget.y, cameraTarget.z);
			D3DXVECTOR3 at = lookAt[i] + eye;
			//ビュー行列の計算.
			D3DXMatrixLookAtLH(&_ViewMatrix, &eye, &at, &up[i]);
		
			for (auto& it : _ModelList)
			{
				it->RenderToEnvironmentMap();
			}

		}

		_isRender = false;

	}

	_ModelList.clear();

}
