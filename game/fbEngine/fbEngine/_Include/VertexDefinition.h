#pragma once
//頂点定義

//頂点座標
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
//頂点テクスチャコード
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
//頂点法線
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
//接ベクトル.
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
//頂点カラー
struct VERTEX_COLOR
{
	float r, g, b, a;
};