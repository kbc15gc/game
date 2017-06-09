/**
* �V���h�E�}�b�v�N���X�̎���.
*/
#include"fbstdafx.h"
#include"ShadowMap.h"

/**
* �R���X�g���N�^. 
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
* �쐬.
*/
void ShadowMap::Create()
{
	int w, h;
	w = h = 2048;

	//�A�X�y�N�g��.
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
* �X�V.
*/
void ShadowMap::Update()
{
	CalcLVPMatrixFromCamera();
}

/**
* �`��.
*/
void ShadowMap::Render()
{
	
	FOR(count, SHADOWMAP_NUM)
	{
		INSTANCE(RenderTargetManager)->SetRT(0, &_ShadowMapRT[count]);

		(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	
		_NowRenderShadowMap = count;

		//�w�ʂ̃J�����O�͂��Ȃ�.
		(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		for (auto& it : _CasterModelList)
		{
			//���f���`��.
			it->RenderToShadowMap();
		}

		//�w�ʂ������ŃJ�����O����.
		(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		_Blur[count].Render();
	}


	
	//�S�Ẵ��f�������X�g����폜.
	_CasterModelList.clear();

}

/**
* ���.
*/
void ShadowMap::Release()
{
}

/**
* ���C�g�r���[�v���W�F�N�V�����s����v�Z.
*/
void ShadowMap::CalcLVPMatrixFromCamera()
{
	if (!_Camera)
	{
		return;
	}

	//�J�����̎���.
	Vector3 cameraDir;
	cameraDir.Subtract(_Camera->GetTarget(), _Camera->GetPosition());
	
	if (fabsf(cameraDir.x) < FLT_EPSILON && fabsf(cameraDir.z) < FLT_EPSILON)
	{
		//�قڐ^��������Ă���.
		return;
	}

	//���ʕ����ɂ���.
	cameraDir.y = 0.0f;
	cameraDir.Normalize();

	//���C�g�̑O����.
	Vector3 lightViewForward;
	lightViewForward.Subtract(_LightTarget, _LightPosition);
	lightViewForward.Normalize();

	//���C�g�̏����.
	Vector3 lightViewUp;
	lightViewUp.Cross(lightViewForward, cameraDir);
	lightViewUp.Normalize();

	//���C�g�̉E����.
	Vector3 lightViewRight;
	lightViewRight.Cross(lightViewUp, lightViewForward);
	lightViewRight.Normalize();

	//���C�g�r���[��]�s��.
	D3DXMATRIX lightViewRot;
	D3DXMatrixIdentity(&lightViewRot);

	//���C�g�r���[�̉���ݒ肷��B
	lightViewRot.m[0][0] = lightViewRight.x;
	lightViewRot.m[0][1] = lightViewRight.y;
	lightViewRot.m[0][2] = lightViewRight.z;
	lightViewRot.m[0][3] = 0.0f;
	//���C�g�r���[�̏��ݒ肷��B
	lightViewRot.m[1][0] = lightViewUp.x;
	lightViewRot.m[1][1] = lightViewUp.y;
	lightViewRot.m[1][2] = lightViewUp.z;
	lightViewRot.m[1][3] = 0.0f;
	//���C�g�r���[�̑O��ݒ肷��B
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
	//���C�g�̎��_.
	Vector3 lightPos = CameraPos;
	//���C�g�̃I�t�Z�b�g.
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

		//����.
		Vector3 LightPos = lightPos + tmpVector;

		//��]�s���ݒ�.
		lightViewMatrix = lightViewRot;
		lightViewMatrix.m[3][0] = LightPos.x;
		lightViewMatrix.m[3][1] = LightPos.y;
		lightViewMatrix.m[3][2] = LightPos.z;
		lightViewMatrix.m[3][3] = 1.0f;

		//���C�g�r���[�s�񊮐�.
		D3DXMatrixInverse(&_LVMatrix[i], nullptr, &lightViewMatrix);

		//�J�����̉�p.
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
