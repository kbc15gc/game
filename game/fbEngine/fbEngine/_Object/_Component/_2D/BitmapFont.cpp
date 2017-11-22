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
	//��ŏ㉺���]������̂ł����č����ɂ���B
	//�|�W�V������`
	VERTEX_POSITION position[] = {
		{ 0.0f, 0.0f, 0.0f, 1.0f },//����
		{ 1.0f, 0.0f, 0.0f, 1.0f },//�E��
		{ 0.0f, 1.0f, 0.0f, 1.0f },//����
		{ 1.0f, 1.0f, 0.0f, 1.0f },//�E��
	};

	//�|�W�V�����̒��_�o�b�t�@�쐬
	(*graphicsDevice()).CreateVertexBuffer(
		sizeof(VERTEX_POSITION) * 4,	//���_�o�b�t�@�̃T�C�Y(�T�C�Y * ���_��)
		D3DUSAGE_WRITEONLY,
		0,								//�Œ�@�\�ł͂Ȃ��̂Ŕ�e�u�e���_�ō쐬����
		D3DPOOL_MANAGED,
		&_PosBuffer,					//���_�o�b�t�@
		NULL
	);

	D3DVERTEXBUFFER_DESC desc;
	_PosBuffer->GetDesc(&desc);
	//���_�o�b�t�@�̐擪�A�h���X�ւ̎Q�Ƃ��i�[����B
	VERTEX_POSITION* pData;
	//�A�h���X�擾
	_PosBuffer->Lock(
		0,						//���b�N����擪
		desc.Size,				//���b�N�����
		(void**)&pData,			//�擪�A�h���X
		D3DLOCK_DISCARD			//�t���O
	);
	vector<VERTEX_POSITION> uv;
	//�A�h���X�̎Q�Ƃ֒��_��`���R�s�[
	memcpy(pData, position, desc.Size);
	//���b�N����
	_UVBuffer->Unlock();


	/*
	���_�f�[�^�̃��C�A�E�g���`���܂��B
	typedef struct D3DVERTEXELEMENT9 {
    WORD Stream;		�X�g���[���ԍ�
    WORD Offset;		���_�f�[�^�̐擪����A����̃f�[�^�^�Ɋ֘A�t������f�[�^�܂ł̃I�t�Z�b�g�B
    BYTE Type;			D3DDECLTYPE �Ƃ��Ďw�肳��Ă���f�[�^�^�B�f�[�^�̃T�C�Y���`����
    BYTE Method;		�e�b�Z���[�^�ɂ�钸�_�f�[�^�̉��� �uD3DDECLMETHOD�v���Q�Ƃ��Ă��������B
    BYTE Usage;			�V�F�[�_�[�̃Z�}���e�B�b�N�X�B
    BYTE UsageIndex;	�Z�}���e�B�b�N�X�̓Y�����B(TEXCOORD"0",TEXCOORD"1")
	} D3DVERTEXELEMENT9, *LPD3DVERTEXELEMENT9;
	*/

	//���_�錾(���_���ǂ̂悤�ɍ\������Ă��邩)
	D3DVERTEXELEMENT9 elements[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // ���_���W
		{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, // UV
		D3DDECL_END()	//�I�[�B
	};
	
	//�f�R���[�V�����쐬
	(*graphicsDevice()).CreateVertexDeclaration(elements, &_Declaration);
}

void BitmapFont::Update()
{
	//������ɑΉ�����UV��ςށB
	
	
}

void BitmapFont::Render()
{
	//�V�F�[�_�t�@�C���ǂݍ���
	_Effect = EffectManager::LoadEffect("BitmapFont.fx");

	D3DXMATRIX  matWorld, matSize, matScale, matRot, matTrans;
	//�摜�̃T�C�Y��ݒ�
	D3DXMatrixScaling(&matSize, _TextSize.x, _TextSize.y, 1.0f);
	//�ݒ肳�ꂽ�X�P�[����ݒ�
	D3DXMatrixScaling(&matScale, transform->GetScale().x, transform->GetScale().y, transform->GetScale().z);
	//��]
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(transform->GetAngle().z));
	//�ړ�
	D3DXMatrixTranslation(&matTrans, transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z);

	//�摜�T�C�Y�@*�@�X�P�[���@*�@��]�@*�@�|�W�V����
	matWorld = matSize * matScale *matRot * matTrans;

	RECT rect;
	//�E�B���h�E�n���h������N���C�A���g�G���A�̃T�C�Y���擾
	HWND hWnd = FindWindow("DECIDE", NULL);
	GetClientRect(hWnd, &rect);
	float w = (float)rect.right;
	float h = (float)rect.bottom;

	//�ˉe�ϊ��s��
	//�X�N���[���̃T�C�Y�Ɏ��߂�B
	D3DXMATRIX proj(
		2.0f / w, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0 / -h, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	);

	D3DXMATRIX WP = matWorld * proj;

	//�e�N�j�b�N�ݒ�
	_Effect->SetTechnique("SpriteTech");

	_Effect->Begin(NULL, 0);
	_Effect->BeginPass(0);

	//���[���h�v���W�F�N�V�����s��s��
	_Effect->SetMatrix("g_WP", &WP);
	//�e�N�X�`��
	_Effect->SetTexture("g_Bitmap", _Bitmap->pTexture);

	//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
	_Effect->CommitChanges();
	
	//�`��B
	{
		//���g���p�����[�^�Ȃ���̂�ݒ�B
		//(*graphicsDevice()).SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | Stack.size());
		//(*graphicsDevice()).SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
		//�f�R���[�V�����ݒ�B
		(*graphicsDevice()).SetVertexDeclaration(_Declaration);
		//�X�g���[���Z�b�g�B
		(*graphicsDevice()).SetStreamSource(0, _PosBuffer, 0, sizeof(VERTEX_POSITION));
		(*graphicsDevice()).SetStreamSource(1, _UVBuffer, 0, sizeof(Vector4));
		//�`��B
		(*graphicsDevice()).DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_LINESTRIP, 0, 4);
	}

	_Effect->EndPass();
	_Effect->End();

	//��n��
	(*graphicsDevice()).SetStreamSourceFreq(0, 1);
	(*graphicsDevice()).SetStreamSourceFreq(1, 1);
}

void BitmapFont::LoadFont(string filename)
{
	string filepath = "Font/" + filename;
	//�e�N�X�`�����[�h�B
	_Bitmap = TextureManager::LoadTexture(filepath.c_str());
}

void BitmapFont::SetText(string text)
{
	_Text = text;
	//UV�̐ݒ�B

	D3DVERTEXBUFFER_DESC desc;
	_UVBuffer->GetDesc(&desc);
	//���_�o�b�t�@�̐擪�A�h���X�ւ̎Q�Ƃ��i�[����B
	VERTEX_POSITION* pData;
	//�A�h���X�擾
	_UVBuffer->Lock(
		0,						//���b�N����擪
		desc.Size,				//���b�N�����
		(void**)&pData,			//�擪�A�h���X
		D3DLOCK_DISCARD			//�t���O
	);
	vector<VERTEX_POSITION> uv;
	for (int i = 0; i < uv.size(); i++) {

		*pData = uv[i];
		pData++;
	}
	//���b�N����
	_UVBuffer->Unlock();
}