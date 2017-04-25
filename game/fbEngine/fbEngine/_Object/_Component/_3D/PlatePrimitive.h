#pragma once
#include "_Object\_Component\Component.h"

class Vertex;
namespace fbEngine
{
	enum BlendModeE
	{
		Trans = 1,	//��Z����
		Add,		//���Z����
	};
}
//�|���S�����o��
class PlatePrimitive :public Component
{
public:
	PlatePrimitive(GameObject* g, Transform* t);
	void Awake()override;
	void Start()override;
	void Update()override;
	void Render()override;

	void SetBillboard(const bool& f);
	void SetBlendMode(fbEngine::BlendModeE mode);
	void SetTexture(TEXTURE* t);
	void SetSize(const Vector2& size);
	void SetPivot(const Vector2& v2);
	void SetPivot(float x, float y);
	void SetUV(const Vector4& uv);
	void SetBlendColor(const Color& c);
private:
	//�r���{�[�h���邩�ǂ����H
	bool _IsBillboard;
	//�u�����h���[�h
	fbEngine::BlendModeE _BlendMode;
	//�V�F�[�_�[�G�t�F�N�g
	Effect* _Effect;
	//�傫��
	Vector2 _Size;
	//�s�{�b�g
	Vector2 _Pivot;
	//UVxy��u��zw��v��\���B
	Vector4 _UV;
	//�e�N�X�`���[
	LPDIRECT3DTEXTURE9 _Texture;
	//�J����
	Camera* _Camera;
	//���C�g
	Light* _Light;
	//������F
	Color _BlendColor;

	static Vertex* _Vertex;	//���_
};