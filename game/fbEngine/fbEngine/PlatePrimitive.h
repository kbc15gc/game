#pragma once
#include "GameObject.h"

class Vertex;

class PlatePrimitive :public GameObject
{
public:
	PlatePrimitive(char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
	void Render()override;

	void SetTexture(TEXTURE* t);
	void SetPivot(Vector2 v2);
	void SetPivot(float x, float y);
	void SetBlendColor(Color c);
private:
	Effect* _Effect;
	//�傫��
	Vector2 _Size;
	//�s�{�b�g
	Vector2 _Pivot;
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