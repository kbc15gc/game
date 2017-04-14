#pragma once
#include "Component.h"

//�������C�g
struct DirectionalLight
{
public:
	DirectionalLight()
	{
		_Rotation = Vector3::zero;
		_Color = Color::white;
	}
	//�I�C���[�p�Őݒ�
	void SetEulerAngles(const Vector3& angle)
	{
		//�������x�N�g��
		D3DXVECTOR3 in = { 0.0f,0.0f,1.0f };
		D3DXVECTOR3 out;
		//��]�s��쐬
		D3DXMATRIX rotateMatrix;
		D3DXMatrixRotationYawPitchRoll(&rotateMatrix, D3DXToRadian(angle.y), D3DXToRadian(angle.x), D3DXToRadian(angle.z));
		//�x�N�g������]�s����g���ĉ�]
		D3DXVec3TransformCoord(&out, &in, &rotateMatrix);
		//�Z�b�g���Đ��K��
		_Rotation.Set(out);
		_Rotation.Normalize();
	}
	//�����Őݒ�
	void SetDirection(const Vector3& dir)
	{
		_Rotation = dir;
	}
	//���C�g�̐F��ݒ�
	void SetColor(const Color& color)
	{
		_Color = color;
	}
	const Color& GetColor()
	{
		return _Color;
	}
	//������Ԃ�
	Vector4 Direction() const
	{
		return Vector4(_Rotation.x, _Rotation.y, _Rotation.z, 1.0f);
	}
private:
	Vector3 _Rotation;	//��]
	Color _Color;	//�F
};

class Light :public Component
{
public:
	Light::Light(GameObject* g, Transform* t) :Component(g, t, typeid(this).name())
	{
		
	}
	Light::~Light();
	void Awake()override;
	//���C�g�̒ǉ�
	void AddLight(DirectionalLight* l);
	//�x�N�^�[�擾
	const vector<DirectionalLight*>& GetLight();
	//���C�g�̐��擾
	const int GetNum()const;
private:
	//���C�g�̃|�C���^�B
	vector<DirectionalLight*> _LightVec;
};