#ifndef _GENERATEOBJECTS_H_
#define _GENERATEOBJECTS_H_

#include "InclFiles.h"
#include "VertexTypes.h"

#define X_CELL_SPACING 0.1f
#define Z_CELL_SPACING 0.1f

vector<Vertex> GenerateGrid(float xWidth, float zWidth, vector<UINT> &indices, UINT texFactor);

vector<Vertex> GenerateHills(float xWidth, float zWidth, vector<UINT> &indices, UINT texFactor);

float GenerateHeight(float x, float z);

#if defined(X_CELL_SPACING) && defined(Z_CELL_SPACING)

#define NumberOfVertices(width, height) ( (UINT)(width/X_CELL_SPACING)*(UINT)(height/Z_CELL_SPACING) )
#define NumberOfIndices(width, height) ( ((UINT)(width/X_CELL_SPACING) - 1)*((UINT)(height/Z_CELL_SPACING) - 1) * 6 )

#endif // defined(X_CELL_SPACING) && defined(Z_CELL_SPACING)


#endif //_GENERATEOBJECTS_H_
