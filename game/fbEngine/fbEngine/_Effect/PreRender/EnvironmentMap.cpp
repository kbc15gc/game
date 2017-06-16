/**
* ���}�b�v�N���X�̎���.
*/
#include"fbstdafx.h"
#include"EnvironmentMap.h"

namespace
{

	/** �J�����̌���. */
	D3DXVECTOR3 lookAt[6] =
	{
		D3DXVECTOR3(1.0f,  0.0f,  0.0f),	// +X
		D3DXVECTOR3(-1.0f,  0.0f,  0.0f),	// -X
		D3DXVECTOR3(0.0f,  1.0f,  0.0f),	// +Y
		D3DXVECTOR3(0.0f, -1.0f,  0.0f),	// -Y
		D3DXVECTOR3(0.0f,  0.0f,  1.0f),	// +Z
		D3DXVECTOR3(0.0f,  0.0f, -1.0f) 	// -Z
	};

	/** �J�����̏����. */
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
* �쐬.
*/
bool EnvironmentMap::Create()
{

	HRESULT hr;

	//�L���[�u�e�N�X�`���̍쐬.
	hr = (*graphicsDevice()).CreateCubeTexture(
		512,	// �e�N�X�`���̒���.
		1,		// �~�b�v�}�b�v���x��.
		D3DUSAGE_RENDERTARGET,	// �L���[�u�e�N�X�`���̎g����.
		D3DFMT_A8R8G8B8,	// �t�H�[�}�b�g.
		D3DPOOL_DEFAULT,	// �e�N�X�`���̊Ǘ���(D3DPOOL_DEFAULT�͎����ŉ������K�v����).
		&_D3DCubeTexture,	// �L���[�u�}�b�v�e�N�X�`���̃|�C���^.
		nullptr	// �V�F�A�[�h�n���h��.
		);

	if (FAILED(hr))
	{
		return false;
	}

	//�v���W�F�N�V�����s��̌v�Z.
	D3DXMatrixPerspectiveFovLH(&_ProjMatrix, D3DXToRadian(90.0f), 1.0f, 1.0f, 10000.0f);

	_RenderTime = 0.0f;

	return true;
	
}

/**
* �X�V.
*/
void EnvironmentMap::Update()
{
	if (_LocalTime > _RenderTime)
	{
		_isRender = true;
		_LocalTime = 0.0f;
	}

	//���[�J���^�C�����X�V.
	_LocalTime += Time::DeltaTime();
}

/**
* �`��.
*/
void EnvironmentMap::Render()
{
	if (_isRender)
	{
		//�L���[�u�e�N�X�`���̃����_�����O�^�[�Q�b�g.
		IDirect3DSurface9* renderTarget = nullptr;

		for (int i = 0; i < 6; i++)
		{
			//�����_�����O�^�[�Q�b�g���擾.
			_D3DCubeTexture->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, &renderTarget);
			renderTarget->Release();

			//�����_�����O�^�[�Q�b�g��ݒ�.
			(*graphicsDevice()).SetRenderTarget(0, renderTarget);
			(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

			Vector3 cameraTarget = INSTANCE(GameObjectManager)->mainCamera->GetTarget();

			D3DXVECTOR3 eye = D3DXVECTOR3(cameraTarget.x, cameraTarget.y, cameraTarget.z);
			D3DXVECTOR3 at = lookAt[i] + eye;
			//�r���[�s��̌v�Z.
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
