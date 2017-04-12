#pragma once

//�����_�[�^�[�Q�b�g�̏��
struct RenderTarget:Noncopyable
{
	//�J���[�o�b�t�@
	IDirect3DSurface9* buffer;
	//�[�x�o�b�t�@
	IDirect3DSurface9* depth;
	//�r���[�|�[�g
	D3DVIEWPORT9 viewport;
	//
	TEXTURE *texture;
	RenderTarget();
	void Create(Vector2 size,_D3DFORMAT colorfmt = D3DFMT_A8R8G8B8);
};

//�����_�[�^�[�Q�b�g�̓Y����
enum RTIdxE
{
	SHADOWDEPTH = 0
};

//�����_�����O�^�[�Q�b�g�Ǘ��N���X
class RenderTargetManager
{
public:
	//�����_�[�^�[�Q�b�g��ێ�
	RenderTargetManager();

	//�����_�[�^�[�Q�b�g�����ׂĕύX���A��ʂ��N���A����
	void ReSetRT();
	//�����_�[�^�[�Q�b�g��ύX���A��ʂ��N���A����
	//�������FDWORD ���Ԗڂ̃����_�[�^�[�Q�b�g�ɐݒ肷�邩,��j�����FRenderTarget*�@�ݒ肷�郌���_�[�^�[�Q�b�g
	void ReSetRT(DWORD RenderTargetIndex, RenderTarget* renderTarget,Color color = Color::blue);
	//�����_�[�^�[�Q�b�g��ύX���邾��
	//�������FDWORD ���Ԗڂ̃����_�[�^�[�Q�b�g�ɐݒ肷�邩,��j�����FRenderTarget*�@�ݒ肷�郌���_�[�^�[�Q�b�g
	void SetRT(DWORD RenderTargetIndex, RenderTarget* renderTarget);
	//�������FDWORD ���Ԗڂ̃����_�[�^�[�Q�b�g�ɐݒ肷�邩,int list�̉��Ԗڂ������_�[�^�[�Q�b�g�ɐݒ肷�邩
	void SetRT(DWORD RenderTargetIndex, int idx);
	//�����_�[�^�[�Q�b�g����O��
	//�������FDWORD�@�O�����������_�[�^�[�Q�b�g�̓Y����
	void RemoveRT(DWORD RenderTargetIndex);
	//�ύX���������_�[�^�[�Q�b�g���o�b�N�o�b�t�@�[�ɖ߂��B
	void BeforeRT();
	//�󂯎�������������ް���ޯẲ摜���R�s�[
	//void CopyTexture(DWORD Idx,TEXTURE* tex);

	//�����_�[�^�[�Q�b�g�̃e�N�X�`���擾
	TEXTURE* GetRTTextureFromList(DWORD idx);
	//�����_�[�^�[�Q�b�g�擾
	RenderTarget* GetRTFromList(DWORD idx);

	static RenderTargetManager* Instance()
	{
		if(_Instance == nullptr)
		{
			_Instance = new RenderTargetManager();
		}
		return _Instance;
	}
private:
	//�o�b�N�o�b�t�@�[(�ŏ��̃����_�[�^�[�Q�b�g)�ێ��p
	RenderTarget* _BackBuffer;
	//�����_�[�^�[�Q�b�g����
	vector<RenderTarget*> _RTList;

	//�����_�[�^�[�Q�b�g�̐�
	int _MaxRTNum;

	static RenderTargetManager* _Instance;
};