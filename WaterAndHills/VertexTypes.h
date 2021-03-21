#ifndef _VERTEXTYPES_H_
#define _VERTEXTYPES_H_

#include "InclFiles.h"

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
	XMFLOAT3 Norm;

	Vertex()
	{

	}

	
	Vertex(XMFLOAT3 pos, XMFLOAT2 tex, XMFLOAT3 norm) : Pos(pos), Tex(tex), Norm(norm)
	{

	}
};

#endif //_VERTEXTYPES_H_