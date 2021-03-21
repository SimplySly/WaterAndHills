#include "GenerateObjects.h"

float GenerateHeight(float x, float z) 
{ 
	return 0.05f*z*cos(3.f*x) + 0.05f*x*sin(3.f*z); 
};


vector<Vertex> GenerateGrid(float xWidth, float zWidth, vector<UINT> &indices, UINT texFactor)
{
	vector<Vertex> result;

	float du = 1.f/(texFactor), dv = 1.f/(texFactor);


	for (UINT i = 0; i < (UINT)(zWidth / Z_CELL_SPACING); i++)
	{
		vector<Vertex> row;

		for (UINT j = 0; j < (UINT)(xWidth / X_CELL_SPACING); j++)
		{
			Vertex v;

			v.Pos = XMFLOAT3(-0.5f*xWidth + j*X_CELL_SPACING, 0.f, -0.5f*zWidth + i*Z_CELL_SPACING);
			v.Tex = XMFLOAT2(i * du, j * dv);
			v.Norm = XMFLOAT3(0.f, 1.f, 0.f);
			
			row.push_back(v);
		}

		for (UINT j = 0; j < (xWidth / X_CELL_SPACING) - 1 && i != 0; j++)
		{
			indices.push_back(i*(UINT)(xWidth / X_CELL_SPACING) + j);
			indices.push_back((i - 1)*(UINT)(xWidth / X_CELL_SPACING) + j);
			indices.push_back((i - 1)*(UINT)(xWidth / X_CELL_SPACING) + j + 1);
			indices.push_back(i*(UINT)(xWidth / X_CELL_SPACING) + j);
			indices.push_back((i - 1)*(UINT)(xWidth / X_CELL_SPACING) + j + 1);
			indices.push_back(i*(UINT)(xWidth / X_CELL_SPACING) + j + 1);
		}

		result.resize(result.size() + row.size());
		copy(row.begin(), row.end(), result.end() - row.size());
	}

	return result;
}


vector<Vertex> GenerateHills(float xWidth, float zWidth, vector<UINT> &indices, UINT texFactor)
{
	vector<Vertex> result;

	float du = 1.f / (float)(texFactor - 1), dv = 1.f / (texFactor - 1);

	for (UINT i = 0; i < (UINT)(zWidth / Z_CELL_SPACING); i++)
	{
		vector<Vertex> row;

		for (UINT j = 0; j < (UINT)(xWidth / X_CELL_SPACING); j++)
		{
			Vertex v;
			float x = -0.5f*xWidth + j*X_CELL_SPACING;
			float z = -0.5f*zWidth + i*Z_CELL_SPACING;

			v.Pos = XMFLOAT3( x, GenerateHeight(x, z), z);
			v.Tex = XMFLOAT2(i*du, j*dv);
			v.Norm = XMFLOAT3(0.f, 1.f, 0.f);

			row.push_back(v);
		}

		for (UINT j = 0; j < (xWidth / X_CELL_SPACING) - 1 && i != 0; j++)
		{
			indices.push_back(i*(UINT)(xWidth / X_CELL_SPACING) + j);
			indices.push_back((i - 1)*(UINT)(xWidth / X_CELL_SPACING) + j);
			indices.push_back((i - 1)*(UINT)(xWidth / X_CELL_SPACING) + j + 1);
			indices.push_back(i*(UINT)(xWidth / X_CELL_SPACING) + j);
			indices.push_back((i - 1)*(UINT)(xWidth / X_CELL_SPACING) + j + 1);
			indices.push_back(i*(UINT)(xWidth / X_CELL_SPACING) + j + 1);
		}

		result.resize(result.size() + row.size());
		copy(row.begin(), row.end(), result.end() - row.size());
	}

	return result;
}