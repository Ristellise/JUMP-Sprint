#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "FontLoader.h"

/******************************************************************************/
/*!
        Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
    static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
    static Mesh* GenerateQuad(const std::string &meshName, Color color, float length);
    static Mesh* GenerateCube(const std::string &meshName, Color color, float length);
    static Mesh* GenerateCircle(const std::string &meshName, float radius);
    static Mesh* GenerateRing(const std::string &meshName, float innerRadius, float outerRadius);
    static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
    static Mesh* GenerateTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR);
    static Mesh* GenerateCylinder(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float height);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height);
    static Mesh * GenerateText(const std::string & meshName, FontLoader loaderInstance);
    static Mesh * GenerateText(const std::string & meshName, FontLoader loaderInstance, float scale = 5.0f);
    static std::vector<Mesh*> GenerateOBJ(const std::string & meshName, const std::string & file_path);
};

#endif