#pragma once

//�J���[
class Color {
public:
	union {
		struct { float r, g, b, a; };
		float color[4];
	};
	//��
	static const Color red;
	//��
	static const Color green;
	//��
	static const Color blue;
	//���F
	static const Color yellow;
	//��
	static const Color water;
	//��
	static const Color purple;
	//��
	static const Color white;
	//��
	static const Color black;
	//��
	static const Color zero;

	//�h�J�x���t�H���g�p�J���[
	static const Color dokaben;
public:
	operator D3DXCOLOR() { return static_cast<D3DXCOLOR>(*this); }
	operator LPVOID() { return static_cast<LPVOID>(this); }

	Color()
	{
		this->r = this->g = this->b = this->a = 0;
	}
	//float�^�R���X�g���N�^ 0.0f~1.0f�̊ԂŎw��B
	Color(const float& r,const float& g,const float& b,const float& a)
	{
		Set(r, g, b, a);
	}
	//int�^�R���X�g���N�^ 0~255�Ŏw��B
	Color(const int& r, const int& g, const int& b, const int& a)
	{
		Set(r, g, b, a);
	}
	//D3DXCOLOR��Color�ɕϊ�
	Color(D3DXCOLOR& c)
	{
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->a = c.a;
	}
	void Set(const float& r, const float& g, const float& b, const float& a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	void Set(const int& r, const int& g, const int& b, const int& a)
	{
		this->r = (float)r / 255.0f;
		this->g = (float)g / 255.0f;
		this->b = (float)b / 255.0f;
		this->a = (float)a / 255.0f;
	}
	//Color��D3DXVECTOR3�ɕϊ��R�s�[����
	void CopyFrom(D3DXCOLOR& c) const
	{
		c.r = r;
		c.g = g;
		c.b = b;
		c.a = a;
	}

	Color operator * (float f)
	{
		this->r *= f;
		this->g *= f;
		this->b *= f;
		return *this;
	}
};