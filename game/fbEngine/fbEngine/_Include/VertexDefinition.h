#pragma once
//���_��`

//���_���W
struct VERTEX_POSITION
{
	float x, y, z, w;
	VERTEX_POSITION()
	{
	}
	VERTEX_POSITION(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	VERTEX_POSITION operator+(VERTEX_POSITION v)
	{
		return VERTEX_POSITION(x + v.x, y + v.y, z + v.z, w + v.w);
	}
};
//���_�e�N�X�`���R�[�h
struct VERTEX_TEXCOORD
{
	float u, v;
	VERTEX_TEXCOORD()
	{
	}
	VERTEX_TEXCOORD(float u, float v)
	{
		this->u = u;
		this->v = v;
	}
	VERTEX_TEXCOORD operator+(VERTEX_TEXCOORD tex)
	{
		return VERTEX_TEXCOORD(u + tex.u, v + tex.v);
	}
};
//���_�@��
struct VERTEX_NORMAL
{
	float x, y, z, w;
	VERTEX_NORMAL()
	{
	}
	VERTEX_NORMAL(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};
//�ڃx�N�g��.
struct VERTEX_TANGENT
{
	float x, y, z, w;
	VERTEX_TANGENT()
	{
	}
	VERTEX_TANGENT(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};
//���_�J���[
struct VERTEX_COLOR
{
	float r, g, b, a;
};