/**
* シャドウマップクラスの実装.
*/
#include"ShadowMap.h"

namespace
{
	const float ShadowAreaTable[ShadowMap::SHADOWMAP_NUM][2] =
	{
		{ 20.0f	, 20.0f },
		{ 45.0f	, 45.0f },
		{ 120.0f, 120.0f },
	};
}

/**
* コンストラクタ. 
*/
ShadowMap::ShadowMap()
{
	FOR(count, SHADOWMAP_NUM)
	{
		_ShadowArea[count].x = ShadowAreaTable[count][0];
		_ShadowArea[count].y = ShadowAreaTable[count][1];

		D3DXMatrixIdentity(&_LPMatrix[count]);
	}
	D3DXMatrixIdentity(&_LVMatrix);
}

/**
* 作成.
*/
void ShadowMap::Create()
{
	int w, h;
	w = h = 2048;

	//アスペクト比.
	_Aspect = (float)w / h;

	int size[SHADOWMAP_NUM][2] = 
	{
		{ w		,h		},
		{ w >> 1,h >> 1 },
		{ w >> 1,h >> 1 },
	};

	FOR(count, SHADOWMAP_NUM)
	{
		_ShadowMapRT[count].Create(Vector2(size[count][0], size[count][1]), D3DFMT_G16R16F, D3DFMT_D16);

		_Blur[count].Create(size[count][0], size[count][1], _ShadowMapRT[count].texture);
	}

}

/**
* 更新.
*/
void ShadowMap::Update()
{
	//視点と注視点で方向を計算.
	_LightDirection.Subtract(_LightTarget, _LightPosition);
	_LightDirection.Normalize();

	//ライトの上方向.
	Vector3 lightUp;
	
	//方向の角度を求める.
	float angle = fabsf(_LightDirection.Dot(Vector3::up));

	if (fabsf((angle - 1.0f)) < 0.00001f)
	{
		lightUp = Vector3::right;
	}
	else
	{
		lightUp = Vector3::up;
	}

	//ライトの注視点.
	Vector3 lightTarget;
	lightTarget.Add(_LightPosition, _LightDirection);

	//ライトビュー行列の計算.
	D3DXMatrixLookAtLH(&_LVMatrix, (D3DXVECTOR3*)&_LightPosition, (D3DXVECTOR3*)&lightTarget, (D3DXVECTOR3*)&lightUp);
	_ShadowReceiverParam._LVMatrix = _LVMatrix;

	FOR(count, SHADOWMAP_NUM)
	{
		//平行投影プロジェクション行列を計算
#ifdef NEKOMATA
		if (count == 0)
		{
			D3DXMatrixOrthoOffCenterLH(&_LPMatrix[count],
				0,
				_ShadowArea[count].x * _Aspect,
				_ShadowArea[count].y,
				0,
				_Near,
				_Far);
		}
		else
		{
			D3DXMatrixOrthoOffCenterLH(&_LPMatrix[count],
				_ShadowArea[count - 1].x * _Aspect,
				_ShadowArea[count].x * _Aspect,
				_ShadowArea[count].y,
				_ShadowArea[count - 1].y,
				_Near,
				_Far);
		}
#else
		D3DXMatrixOrthoLH(&_LPMatrix[count], ShadowAreaTable[count][0] * _Aspect, ShadowAreaTable[count][1], _Near, _Far);
#endif
		_ShadowReceiverParam._LPMatrix[count] = _LPMatrix[count];
	}
}

/**
* 描画.
*/
void ShadowMap::Render()
{
	//背面のカリングはしない.
	(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	FOR(count, SHADOWMAP_NUM)
	{
		INSTANCE(RenderTargetManager)->SetRT(0, &_ShadowMapRT[count]);

		(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	
		_NowRenderShadowMap = count;

		for (auto& it : _CasterModelList)
		{
			//モデル描画.
			it->RenderToShadowMap();
		}

		_Blur[count].Render();
	}

	//背面を左回りでカリングする.
	(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
	//全てのモデルをリストから削除.
	_CasterModelList.clear();

}

/**
* 解放.
*/
void ShadowMap::Release()
{
}
