#include "Scene.h"
#include "Effect.h"
#include "VertexDefinition.h"
#include "Vertex.h"

Vertex* Scene::_Vertex = nullptr;
Effect* Scene::_Effect = nullptr;
float Scene::_FadeAlpha = 0.0f;
float Scene::_AddAlpha = 0.0f;
bool Scene::_IsFade = false;

Scene::Scene():
	_ChangeScene(false)
{
	if (_Vertex == nullptr)
	{
		_Vertex = new Vertex();

		//�|�W�V������`(�t���X�N���[���T�C�Y)
		VERTEX_POSITION position[] = {
			{ -1.0f, -1.0f, 0.0f, 1.0f },//����
			{ -1.0f, 1.0f, 0.0f, 1.0f },//����
			{ 1.0f, -1.0f, 0.0f, 1.0f },//�E��
			{ 1.0f, 1.0f, 0.0f, 1.0f },//�E��
		};
		//UV��`
		VERTEX_TEXCOORD texcoord[] = {
			{ 0.0f, 1.0f },//����
			{ 0.0f, 0.0f },//����
			{ 1.0f, 1.0f },//�E��
			{ 1.0f, 0.0f },//�E��
		};

		//���_�錾(���_���ǂ̂悤�ɍ\������Ă��邩)
		D3DVERTEXELEMENT9 elements[] = {
			{ 0, 0              , D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // ���_���W
			{ 1, 0              , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD   , 0 }, // UV
			D3DDECL_END()
		};

		_Vertex->Initialize(fbEngine::PRIMITIVETYPE::TRIANGLESTRIP, 4);
		_Vertex->CreateVertexBuffer(position, 4, sizeof(VERTEX_POSITION), elements[0]);
		_Vertex->CreateVertexBuffer(texcoord, 4, sizeof(VERTEX_TEXCOORD), elements[1]);
		_Vertex->CreateDeclaration();
	}

	if(_Effect == nullptr)
	{
		_Effect = EffectManager::LoadEffect("Scene.fx");
	}
}

Scene::~Scene()
{
}

void Scene::Fade()
{
	//1�t���[��������̉��Z��
	float add = _AddAlpha * Time::DeltaTime();
	//0.0f~1.0f�̊ԂɎ��߂�
	if(_AddAlpha > 0)
	{
		//+
		_FadeAlpha = min(1.0f, _FadeAlpha + add);
	}
	else if(_AddAlpha < 0)
	{
		//-
		_FadeAlpha = max(0.0f, _FadeAlpha + add);
	}

	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�e�N�j�b�N�ݒ�
	_Effect->SetTechnique("SceneTech");

	_Effect->Begin(NULL, 0);
	_Effect->BeginPass(0);

	//�F�ݒ�
	_Effect->SetValue("g_FadeColor", &Color::black, sizeof(Color));
	//���l
	_Effect->SetFloat("g_Alpha", _FadeAlpha);

	//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
	_Effect->CommitChanges();
	//�|���`��
	_Vertex->DrawPrimitive();

	_Effect->EndPass();
	_Effect->End();

	//�ύX�����X�e�[�g�����ɖ߂�
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	_IsFade = (0.0f < _FadeAlpha && _FadeAlpha < 1.0f);
}

void Scene::SetFade(bool inout, float fadetime)
{
	//���������H
	int NegaPosi = (inout) ? 1 : -1;
	//1�b������̉��Z��
	_AddAlpha = (1.0f / fadetime) * NegaPosi;
	_IsFade = true;
}