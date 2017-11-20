#include "fbstdafx.h"
#include "BitmapFont.h"
#include "_Object\Vertex.h"

BitmapFont::BitmapFont(GameObject * g, Transform * t) :
	Component(g, t, typeid(this).name())
{	
}

void BitmapFont::Start()
{
	if (_Vertex == nullptr)
	{
		_Vertex = new Vertex();
		//��ŏ㉺���]������̂ł����č����ɂ���B
		//�|�W�V������`
		VERTEX_POSITION position[] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f },//����
			{ 1.0f, 0.0f, 0.0f, 1.0f },//�E��
			{ 0.0f, 1.0f, 0.0f, 1.0f },//����
			{ 1.0f, 1.0f, 0.0f, 1.0f },//�E��
		};
		//UV��`
		VERTEX_TEXCOORD texcoord[] = {
			{ 0.0f, 0.0f },//����
			{ 1.0f, 0.0f },//�E��
			{ 0.0f, 1.0f },//����
			{ 1.0f, 1.0f },//�E��
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
}
