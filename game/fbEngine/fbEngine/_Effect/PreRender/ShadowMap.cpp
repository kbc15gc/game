/**
* シャドウマップクラスの実装.
*/
#include"fbstdafx.h"
#include"ShadowMap.h"

/**
* コンストラクタ. 
*/
ShadowMap::ShadowMap()
{
	FOR(count, SHADOWMAP_NUM)
	{
		D3DXMatrixIdentity(&_LPMatrix[count]);
		D3DXMatrixIdentity(&_LVMatrix[count]);
	}
}

/**
* 作成.
*/
void ShadowMap::Create()
{
	int w, h;
	w = h = 2048;

	//アスペクト比.
	_Aspect = (float)w / (float)h;

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

	_isVSM = true;

}

/**
* 更新.
*/
void ShadowMap::Update()
{
	CalcLVPMatrixFromCamera();
}

/**
* 描画.
*/
void ShadowMap::Render()
{
	
	FOR(count, SHADOWMAP_NUM)
	{
		INSTANCE(RenderTargetManager)->SetRT(0, &_ShadowMapRT[count]);

		(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	
		_NowRenderShadowMap = count;

		//背面のカリングはしない.
		(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		for (auto& it : _CasterModelList)
		{
			//モデル描画.
			it->RenderToShadowMap();
		}

		//背面を左回りでカリングする.
		(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		_Blur[count].Render();
	}


	
	//全てのモデルをリストから削除.
	_CasterModelList.clear();

}

/**
* 解放.
*/
void ShadowMap::Release()
{
}

/**
* ライトビュープロジェクション行列を計算.
*/
void ShadowMap::CalcLVPMatrixFromCamera()
{
	if (!_Camera)
	{
		return;
	}

	//カメラの視線.
	Vector3 cameraDir;
	cameraDir.Subtract(_Camera->GetTarget(), _Camera->GetPosition());
	
	if (fabsf(cameraDir.x) < FLT_EPSILON && fabsf(cameraDir.z) < FLT_EPSILON)
	{
		//ほぼ真上を向いている.
		return;
	}

	//平面方向にする.
	cameraDir.y = 0.0f;
	cameraDir.Normalize();

	//ライトの前方向.
	Vector3 lightViewForward;
	lightViewForward.Subtract(_LightTarget, _LightPosition);
	lightViewForward.Normalize();

	//ライトの上方向.
	Vector3 lightViewUp;
	lightViewUp.Cross(lightViewForward, cameraDir);
	lightViewUp.Normalize();

	//ライトの右方向.
	Vector3 lightViewRight;
	lightViewRight.Cross(lightViewUp, lightViewForward);
	lightViewRight.Normalize();

	//ライトビュー回転行列.
	D3DXMATRIX lightViewRot;
	D3DXMatrixIdentity(&lightViewRot);

	//ライトビューの横を設定する。
	lightViewRot.m[0][0] = lightViewRight.x;
	lightViewRot.m[0][1] = lightViewRight.y;
	lightViewRot.m[0][2] = lightViewRight.z;
	lightViewRot.m[0][3] = 0.0f;
	//ライトビューの上を設定する。
	lightViewRot.m[1][0] = lightViewUp.x;
	lightViewRot.m[1][1] = lightViewUp.y;
	lightViewRot.m[1][2] = lightViewUp.z;
	lightViewRot.m[1][3] = 0.0f;
	//ライトビューの前を設定する。
	lightViewRot.m[2][0] = lightViewForward.x;
	lightViewRot.m[2][1] = lightViewForward.y;
	lightViewRot.m[2][2] = lightViewForward.z;
	lightViewRot.m[2][3] = 0.0f;

	static float shadowAreaTbl[SHADOWMAP_NUM] = 
	{
		20.0f,
		40.0f,
		120.0f
	};

	const Vector3& CameraPos = _Camera->GetPosition();
	//ライトの視点.
	Vector3 lightPos = CameraPos;
	//ライトのオフセット.
	Vector3 lightOffset = cameraDir;
	lightOffset.Scale(shadowAreaTbl[0] * 0.2f);
	lightPos.Add(lightOffset);

	Vector3 tmpVector;
	tmpVector = lightViewForward;
	tmpVector.Scale(-30.0f);

	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		D3DXMATRIX lightViewMatrix;
		D3DXMatrixIdentity(&lightViewMatrix);

		//結果.
		Vector3 LightPos = lightPos + tmpVector;

		//回転行列を設定.
		lightViewMatrix = lightViewRot;
		lightViewMatrix.m[3][0] = LightPos.x;
		lightViewMatrix.m[3][1] = LightPos.y;
		lightViewMatrix.m[3][2] = LightPos.z;
		lightViewMatrix.m[3][3] = 1.0f;

		//ライトビュー行列完成.
		D3DXMatrixInverse(&_LVMatrix[i], nullptr, &lightViewMatrix);

		//カメラの画角.
		float viewAngle = _Camera->GetViewAngle();

		viewAngle = D3DXToRadian(viewAngle);

		D3DXMatrixOrthoLH(&_LPMatrix[i], 
			shadowAreaTbl[i] * _Aspect,
			tan(viewAngle * 0.5f) * (shadowAreaTbl[i] * (i + 1)) * 2.0f,
			_Near,
			_Far
		);

		D3DXMatrixMultiply(&_ShadowReceiverParam._LVPMatrix[i], &_LVMatrix[i], &_LPMatrix[i]);
	
		lightOffset = cameraDir;
		lightOffset.Scale(shadowAreaTbl[i] * 0.9f);
		lightPos.Add(lightOffset);

	}


}
