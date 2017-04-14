#include "RenderTargetManager.h"

RenderTargetManager*  RenderTargetManager::_Instance;

RenderTarget::RenderTarget()
{
	texture = new TEXTURE();
}

void RenderTarget::Create(Vector2 size, _D3DFORMAT colorfmt)
{
	//�O�̃e�N�X�`��������Ȃ�
	if(texture->pTexture)
	{
		//�폜
		SAFE_DELETE(texture->pTexture);
	}
	//�e�N�X�`���쐬
	HRESULT hr = (*graphicsDevice()).CreateTexture(
		size.x, size.y,
		1,
		D3DUSAGE_RENDERTARGET,
		colorfmt,
		D3DPOOL_DEFAULT,
		&texture->pTexture,
		NULL);

	//�摜�T�C�Y����ʃT�C�Y��
	texture->Size = size;

	//�r���[�|�[�g�ݒ�
	viewport = { 0, 0, (DWORD)size.x, (DWORD)size.y, 0.0f, 1.0f };

	//�T�[�t�F�X�̃C���^�[�t�F�[�X�擾
	texture->pTexture->GetSurfaceLevel(0, &buffer);

	//�[�x�o�b�t�@�쐬
	hr = (*graphicsDevice()).CreateDepthStencilSurface(
		size.x, size.y,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&depth,
		NULL);
}

RenderTargetManager::RenderTargetManager()
{
	if (_BackBuffer == nullptr)
	{
		_BackBuffer = new RenderTarget();

		//�o�b�N�o�b�t�@(�����_�����O�^�[�Q�b�g)�ێ�
		//�Q�ƃJ�E���^��������
		(*graphicsDevice()).GetRenderTarget(0, &_BackBuffer->buffer);
		//�[�x�o�b�t�@�ێ�
		(*graphicsDevice()).GetDepthStencilSurface(&_BackBuffer->depth);
		//�r���[�|�[�g�ێ�
		(*graphicsDevice()).GetViewport(&_BackBuffer->viewport);
		//�Q�ƃJ�E���^��������
		//_BackBuffer->buffer->Release();

		//�����_�����O�^�[�Q�b�g���擾
		D3DCAPS9 Caps;
		(*graphicsDevice()).GetDeviceCaps(&Caps);
		DWORD MaxRT = Caps.NumSimultaneousRTs;
		_MaxRTNum = MaxRT;

		FOR(i,_MaxRTNum)
		{
			RenderTarget* ren = new RenderTarget();
			//�Ƃ肠�������������_�o�b�t�@�ō���Ă݂�(��Ζ��ʂ�����)
			ren->Create(g_WindowSize*2);
			ren->texture->Size = g_WindowSize;
			SetRT(i, ren);
			_RTList.push_back(ren);
		}
	}
}

void RenderTargetManager::ReSetRT(DWORD Index, RenderTarget* renderTarget, Color color)
{
	//�e�N�X�`���������_�����O�^�[�Q�b�g��
	(*graphicsDevice()).SetRenderTarget(Index, renderTarget->buffer);
	//�e�N�X�`���p�̐[�x�o�b�t�@�ݒ�
	(*graphicsDevice()).SetDepthStencilSurface(renderTarget->depth);
	//�r���[�|�[�g��ύX��
	(*graphicsDevice()).SetViewport(&renderTarget->viewport);

	//�����_�����O�^�[�Q�b�g������
	(*graphicsDevice()).Clear(
		Index,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_RGBA((int)(color.r*255), (int)(color.g*255), (int)(color.b * 255), (int)(color.a* 255)),
		1.0f,
		0);
}

void RenderTargetManager::ReSetRT()
{
	FOR(i,_MaxRTNum)
	{
		ReSetRT(i, _RTList[i]);
	}
}

void RenderTargetManager::SetRT(DWORD Index, RenderTarget* renderTarget)
{
	//�e�N�X�`���������_�����O�^�[�Q�b�g��
	(*graphicsDevice()).SetRenderTarget(Index, renderTarget->buffer);
	//�e�N�X�`���p�̐[�x�o�b�t�@�쐬
	(*graphicsDevice()).SetDepthStencilSurface(renderTarget->depth);
	//�r���[�|�[�g��ύX��
	(*graphicsDevice()).SetViewport(&renderTarget->viewport);
}

void RenderTargetManager::SetRT(DWORD Index, int idx)
{
	//�e�N�X�`���������_�����O�^�[�Q�b�g��
	(*graphicsDevice()).SetRenderTarget(Index, _RTList[idx]->buffer);
	//�e�N�X�`���p�̐[�x�o�b�t�@�쐬
	(*graphicsDevice()).SetDepthStencilSurface(_RTList[idx]->depth);
	//�r���[�|�[�g��ύX��
	(*graphicsDevice()).SetViewport(&_RTList[idx]->viewport);
}

void RenderTargetManager::RemoveRT(DWORD idx)
{
	(*graphicsDevice()).SetRenderTarget(idx, NULL);
	(*graphicsDevice()).SetDepthStencilSurface(NULL);
}

void RenderTargetManager::BeforeRT()
{
	//0�`�ő吔�̃����_�[�^�[�Q�b�g��ޔ�
	for (short i = 0; i < _MaxRTNum; i++)
	{
		RemoveRT(i);
	}
	//���ɖ߂�
	SetRT(0, _BackBuffer);
}

TEXTURE* RenderTargetManager::GetRTTextureFromList(DWORD idx)
{
	return _RTList[idx]->texture;
}

RenderTarget* RenderTargetManager::GetRTFromList(DWORD idx)
{
	return _RTList[idx];
}