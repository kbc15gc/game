#pragma once
#include "_Object\_Component\Component.h"

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

	/**
	* �������擾.
	*/
	const Vector3& GetAmbientLight()
	{
		return _AmbientLight;
	}

	/**
	* ������ݒ�.
	*/
	void SetAmbientLight(const Vector3& amb)
	{
		_AmbientLight = amb;
	}

	void SetPointLightPosition(const Vector3& pos)
	{
		_PointLightPos = pos;
	}

	void SetPointLightParam(const Vector4& param)
	{
		_PointLightParam = param;
	}

	const Vector3& GetPointLightPosition()
	{
		return _PointLightPos;
	}

	const Vector4& GetPointLightParam()
	{
		return _PointLightParam;
	}

private:
	//���C�g�̃|�C���^�B
	vector<DirectionalLight*> _LightVec;
	
	/** ����. */
	Vector3 _AmbientLight = Vector3(0.3f, 0.3f, 0.3f);

	/** �|�C���g���C�g�̐F�Ƌ���. */
	Vector4 _PointLightParam = Vector4(0, 0, 0, 0);
	/** �|�C���g���C�g�̍��W. */
	Vector3 _PointLightPos = Vector3::zero;

};

/**
* �L�����N�^�[���C�g�N���X.
*/
class CharacterLight 
{
public:

	/** ���C�g�̐�. */
	static const USHORT LIGHT_NUM = 4;

public:

	/**
	* �R���X�g���N�^.
	*/
	CharacterLight()
	{
		for (int i = 0; i < LIGHT_NUM; i++)
		{
			DiffuseLightDirection_[i] = Vector4(0, 0, 0, 0);
			DiffuseLightColor_[i] = Vector4(0, 0, 0, 0);
		}
		AmbientLight_ = Vector4(0, 0, 0, 0);
	}

	/**
	* �f�X�g���N�^.
	*/
 	~CharacterLight()
	{
	}

	/**
	* �f�B�t���[�Y���C�g�̌�����ݒ�.
	*
	* @param no	   ���C�g�̔ԍ�.
	* @param lightDir ���C�g�̕����A���K���ς݃x�N�g��.
	*/
	void SetDiffuseLightDirection(int no, const Vector3& lightDir)
	{
		DiffuseLightDirection_[no] = Vector4(lightDir.x, lightDir.y, lightDir.z, 1.0f);
	}

	/**
	* �f�B�t���[�Y���C�g�̌������擾.
	*
	* @param no ���C�g�̔ԍ�.
	*
	* @return The diffuse light direction.
	*/
	const Vector4& GetDiffuseLightDirection(int no) const
	{
		return DiffuseLightDirection_[no];
	}

	/**
	* �f�B�t���[�Y���C�g�̐F��ݒ�.
	*
	* @param no	   ���C�g�̔ԍ�.
	* @param lightColor ���C�g�̐F�A0.0f�`1.0f�̒l.
	*/
	void SetDiffuseLightColor(int no, const Vector4& lightColor)
	{
		DiffuseLightColor_[no] = lightColor;
	}

	/**
	* �f�B�t���[�Y���C�g�̐F���擾.
	*
	* @param no The ���C�g�̔ԍ�.
	*
	* @return The diffuse light color.
	*/
	const Vector4& GetDiffuseLightColor(int no) const
	{
		return DiffuseLightColor_[no];
	}

	/**
	* �A���r�G���g���C�g�̋�����ݒ�.
	*
	* @param amb �A���r�G���g�̐F.
	*/
	void SetAmbientLight(const Vector4& amb)
	{
		AmbientLight_ = amb;
	}

	/**
	* �A���r�G���g���C�g���擾.
	*
	* @return The ambient light.
	*/
	const Vector4& GetAmbientLight() const
	{
		return AmbientLight_;
	}

private:

	/** ���s�����̕���. */
	Vector4 DiffuseLightDirection_[LIGHT_NUM];
	/** ���s�����̐F. */
	Vector4	DiffuseLightColor_[LIGHT_NUM];
	/** ����. */
	Vector4	AmbientLight_;
	/** ���C�g��. */
	int LightCount_ = LIGHT_NUM;

};