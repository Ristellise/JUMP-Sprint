#include "MeshBuilder.h"
#include <GL\glew.h>
#include "Vertex.h"
#include "MyMath.h"
#include <vector>
#include "LoadOBJ.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{

    Vertex v;
    std::vector<Vertex> vertex_buffer_data;
    v.pos.Set(-1000, 0, 0);
    v.color.Set(1, 0, 0);
    vertex_buffer_data.push_back(v);
    v.pos.Set(1000, 0, 0);
    v.color.Set(1, 0, 0);
    vertex_buffer_data.push_back(v);
    v.pos.Set(0, -1000, 0);
    v.color.Set(0, 1, 0);
    vertex_buffer_data.push_back(v);
    v.pos.Set(0, 1000, 0);
    v.color.Set(0, 1, 0);
    vertex_buffer_data.push_back(v);
    v.pos.Set(0, 0, -1000);
    v.color.Set(0, 0, 1);
    vertex_buffer_data.push_back(v);
    v.pos.Set(0, 0, 1000);
    v.color.Set(0, 0, 1);
    vertex_buffer_data.push_back(v);

    std::vector<GLuint> index_buffer_data;
    index_buffer_data.push_back(0);
    index_buffer_data.push_back(1);
    index_buffer_data.push_back(2);
    index_buffer_data.push_back(3);
    index_buffer_data.push_back(4);
    index_buffer_data.push_back(5);

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_LINES;

    return mesh;
}

/******************************************************************************/
/*!
Degree to Radian or use MyMath.h
*/
/******************************************************************************/

float DegreeToRadian(float value)
{
    return value * 3.142f / 180.0f;
}

/******************************************************************************/
/*!
Ring
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateRing(const std::string &meshName, float innerRadius, float outerRadius)
{
    Vertex v;
    std::vector<Vertex> vertex_buffer_data;


    for (float theta = 0.f; theta <= 360; theta += 10)
    {
        float x = outerRadius * cosf(DegreeToRadian(theta));
        float y = outerRadius * sinf(DegreeToRadian(theta));

        v.pos.Set(x, 0, y);
        v.color.Set(1, 0, 0);
        vertex_buffer_data.push_back(v);

        float x1 = innerRadius * cosf(DegreeToRadian(theta));
        float y1 = innerRadius * sinf(DegreeToRadian(theta));

        v.pos.Set(x1, 0, y1);
        v.color.Set(1, 0, 0);
        vertex_buffer_data.push_back(v);

    }

    std::vector<GLuint> index_buffer_data;

    for (unsigned int i = 0; i < vertex_buffer_data.size(); ++i)
    {
        index_buffer_data.push_back(i);
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}


/******************************************************************************/
/*!
For Sphere use 
*/
/******************************************************************************/
float sphereX(float phi, float theta)
{
    return cos(DegreeToRadian(phi)) * cos(DegreeToRadian(theta));
}
float sphereY(float phi, float theta)
{
    return sin(DegreeToRadian(phi));
}
float sphereZ(float phi, float theta)
{
    return cos(DegreeToRadian(phi)) * sin(DegreeToRadian(theta));
}


/******************************************************************************/
/*!
Sphere
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    float degreePerStack = 180.0f / numStack;
    float degreePerSlice = 360.0f / numSlice;


    for (unsigned stack = 0; stack < numStack + 1; ++stack)
    {
        float phi = -90.0f + stack * degreePerStack;			   //0 is hemisphere

        for (unsigned slice = 0; slice < numSlice + 1; ++slice)
        {
            float theta = slice * degreePerSlice;
            Vertex v;

            v.pos.Set(radius* sphereX(phi, theta), radius*sphereY(phi, theta), radius*sphereZ(phi, theta));
            v.color = color;
            v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
            vertex_buffer_data.push_back(v);
        }
    }

    for (unsigned stack = 0; stack < numStack; ++stack)
    {
        for (unsigned slice = 0; slice <= numSlice; ++slice)
        {
            index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
            index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
        }
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

/******************************************************************************/
/*!
Circle
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCircle(const std::string &meshName, float radius)
{
    Vertex v;
    std::vector<Vertex> vertex_buffer_data;


    for (float theta = 0.f; theta <= 360; theta += 10)
    {
        float x = radius * cosf(DegreeToRadian(theta));
        float y = radius * sinf(DegreeToRadian(theta));

        v.pos.Set(x, 0, y);
        v.color.Set(1, 0, 0);
        vertex_buffer_data.push_back(v);

        v.pos.Set(0, 0, 0);
        v.color.Set(1, 0, 0);
        vertex_buffer_data.push_back(v);

    }

    std::vector<GLuint> index_buffer_data;

    for (unsigned int i = 0; i < vertex_buffer_data.size(); ++i)
    {
        index_buffer_data.push_back(i);
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float length)
{
    std::vector<Vertex> vertex_buffer_data;
    Vertex v;

    v.pos.Set(-0.5f * length, -0.5f * length, 0.f); v.color = color; v.texCoord.Set(0, 0); v.normal.Set(0, 0, -1);
    vertex_buffer_data.push_back(v);

    v.pos.Set(0.5f * length, -0.5f * length, 0.f); v.color = color; v.texCoord.Set(1, 0); v.normal.Set(0, 0, -1);
    vertex_buffer_data.push_back(v);

    v.pos.Set(0.5f * length, 0.5f * length, 0.f); v.color = color; v.texCoord.Set(1, 1); v.normal.Set(0, 0, -1);
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, 0.5f * length, 0.f); v.color = color; v.texCoord.Set(0, 1); v.normal.Set(0, 0, -1);
    vertex_buffer_data.push_back(v);


    std::vector<GLuint> index_buffer_data;
    index_buffer_data.push_back(0);
    index_buffer_data.push_back(1);
    index_buffer_data.push_back(2);
    index_buffer_data.push_back(2);
    index_buffer_data.push_back(3);
    index_buffer_data.push_back(0);

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLES;

    return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float length)
{
    std::vector<Vertex> vertex_buffer_data;
    Vertex v;

    v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, -0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, -0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, -0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, -0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, 0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, 0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, -0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, -0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, 0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, 0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, 0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, 0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, -0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, -0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, -0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, -0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, -0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, -0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, -0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, -0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, -0.5f * length, 0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, 0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, -0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, -0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(-0.5f * length, 0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f* length, 0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, -0.5f * length, -0.5f * length);
    v.color = color;
    vertex_buffer_data.push_back(v);


    std::vector<GLuint> index_buffer_data;

    for (unsigned int i = 0; i < vertex_buffer_data.size(); ++i)
    {
        index_buffer_data.push_back(i);
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLES;
    return mesh;
}


/******************************************************************************/
/*!
Cylinder use
*/
/******************************************************************************/
float cylinderX(float theta)
{
    return cos(Math::DegreeToRadian(theta));
}
float cylinderZ(float theta)
{
    return sin(Math::DegreeToRadian(theta));
}


/******************************************************************************/
/*!
Cylinder
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCylinder(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float height)
{
    Vertex v;

    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;
    float degreePerStack = 180.0f / numStack;
    float degreePerSlice = 360.0f / numSlice;
    float stackHeight = height / numStack;
    for (unsigned stack = 0; stack < numStack + 1; ++stack)
    {
        float phi = -90 + stack * degreePerStack;
        for (unsigned slice = 0; slice < numSlice + 1; ++slice)
        {
            float theta = slice * degreePerSlice;
            v.pos.Set(radius * cylinderX(theta), -height / 2 + stack * stackHeight, radius*cylinderZ(theta));
            v.color = color;
            //v.normal.Set(cylinderX(theta), -height / 2 + stack * stackHeight, cylinderZ(theta));
            vertex_buffer_data.push_back(v);
        }
    }

    for (unsigned stack = 0; stack < numStack; ++stack)
    {
        for (unsigned slice = 0; slice < numSlice + 1; ++slice)
        {
            index_buffer_data.push_back(stack * (numSlice + 1) + slice);
            index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
        }

    }

    Mesh *mesh = new Mesh(meshName);
    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    return mesh;
}
/*--------------------
- Functions used for Generate Text
--------------------*/
void pixel2UV(float pointX, float pointY,
    float textureSize, TexCoord &texCoord)
{
    
    float UVx = pointX / textureSize;
    float UVy = 256.0f - pointY / textureSize;
    texCoord.Set(UVx, UVy);
}

Mesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	Vertex v;
	float degreePerSlice = 360.f / numSlice;

	for (unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
	{
		float theta = slice * degreePerSlice;

		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, height, 0);
		v.color = color;
		vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(slice * 2 + 0);
		index_buffer_data.push_back(slice * 2 + 1);
	}

	Mesh *mesh = new Mesh(meshName);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, FontLoader loaderInstance, float scale)
{
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;
    Vertex v;
    charData buff;
    unsigned int offsetcount = 0;
    float characterW = 0.0f;
    float characterH = 0.0f;
    FontInfo ft = loaderInstance.fntInfo;
    for (size_t i = 0; i < loaderInstance.characters.size(); i++)
    {
        //TODO: Figure out a way to make positions be dynamic to the size of the glyph.
        // TexCoord Math is annoying...
        // bottom left
        // ((float)(buff.offset.b) / (float)(ft.scaleH)) * scale
        buff = loaderInstance.characters[i];
        v.pos.Set(scale * ((float)(buff.offset.a) / (float)(ft.scaleH)),
            scale - ((scale * (float)(buff.characterSize.b) / (float)(ft.scaleH)) + scale * ((float)(buff.offset.b) / (float)(ft.scaleH))),
                  0.0f);
        v.normal.Set(1, 0, 0);
        pixel2UV((float)buff.Coordinate.a,
                 (float)(buff.Coordinate.b + buff.characterSize.b),
                 (float)ft.scaleW,v.texCoord);
        vertex_buffer_data.push_back(v);

        v.pos.Set(scale * ((float)(buff.offset.a) / (float)(ft.scaleH)),
            scale - (scale * ((float)(buff.offset.b) / (float)(ft.scaleH)))
                  ,
                  0);
        pixel2UV((float)buff.Coordinate.a,
                 (float)(buff.Coordinate.b),
                 (float)ft.scaleW, v.texCoord);
        v.normal.Set(1, 1, 0);
        vertex_buffer_data.push_back(v);

        v.pos.Set(((float)(buff.characterSize.a) / (float)(ft.scaleW)) * scale + scale * ((float)(buff.offset.a) / (float)(ft.scaleH)),
            scale - (scale * ((float)(buff.offset.b) / (float)(ft.scaleH))),0);
        pixel2UV((float)buff.Coordinate.a + buff.characterSize.a,
                 (float)(buff.Coordinate.b),
                 (float)ft.scaleW, v.texCoord);
        v.normal.Set(1, 0, 0);
        vertex_buffer_data.push_back(v);


        v.pos.Set(((float)(buff.characterSize.a) / (float)(ft.scaleW)) * scale + scale * ((float)(buff.offset.a) / (float)(ft.scaleH)),
            scale - ((scale * (float)(buff.characterSize.b) / (float)(ft.scaleH)) + scale * ((float)(buff.offset.b) / (float)(ft.scaleH))), 0);
        pixel2UV((float)(buff.Coordinate.a + buff.characterSize.a),
                 (float)(buff.Coordinate.b + buff.characterSize.b),
                 (float)ft.scaleW, v.texCoord);
        v.normal.Set(0, 1, 0);
        vertex_buffer_data.push_back(v);


        
        index_buffer_data.push_back(offsetcount + 0);
        index_buffer_data.push_back(offsetcount + 1);
        index_buffer_data.push_back(offsetcount + 2);
        index_buffer_data.push_back(offsetcount + 0);
        index_buffer_data.push_back(offsetcount + 2);
        index_buffer_data.push_back(offsetcount + 3);
        offsetcount += 4;
    }
    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLES;
    return mesh;

    
}

std::vector<Mesh*> MeshBuilder::GenerateOBJ(const std::string & meshName, const std::string & file_path)
{
    std::vector<PTVPair> Pairs;
    std::vector<Mesh*> Meshes;
    bool success = LoadOBJ(file_path.c_str(), Pairs);
    if (!success)
    {
        printf_s("Failed to load OBJ. Check previous line for a reason.\n");
        return Meshes;
    }
    else
    {
        for (size_t i = 0; i < Pairs.size(); i++)
        {
            
            if (Pairs[i].normal.size() != 0)
            {
                std::vector<Vertex> vertex_buffer_data;
                std::vector<GLuint> index_buffer_data;
                IndexVBO(Pairs[i].pos, Pairs[i].texCoord, Pairs[i].normal, index_buffer_data, vertex_buffer_data);

                Mesh *mesh = new Mesh(meshName);
                mesh->indexSize = index_buffer_data.size();
                mesh->mode = Mesh::DRAW_TRIANGLES;

                glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
                glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

                Meshes.push_back(mesh);
            }
        }
        
    }
    return Meshes;
}


/******************************************************************************/
/*!
Torus
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR)
{
    Vertex v;
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    float degreePerStack = 180.f / numStack;  // 180
    float degreePerSlice = 180.f / numSlice;  // 180 both for 1/2
    float x1, z1;
    float x2, y2, z2;

    for (unsigned stack = 0; stack < numStack + 1; stack++)
    {
        for (unsigned slice = 0; slice < numSlice + 1; slice++)
        {
            z1 = outerR * cos(Math::DegreeToRadian(stack * degreePerStack));
            x1 = outerR * sin(Math::DegreeToRadian(stack * degreePerStack));
            x2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * cos(Math::DegreeToRadian(stack * degreePerStack));
            y2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * sin(Math::DegreeToRadian(stack * degreePerStack));
            z2 = innerR * sin(Math::DegreeToRadian(slice * degreePerSlice));

            v.pos.Set(x2, y2, z2);
            v.color = color;

            //float magnitude = sqrt((x2 * x2) + (y2 * y2) + (z2 * z2));
            //v.normal.Set((x2 / magnitude), (y2 / magnitude), (z2 / magnitude));
            vertex_buffer_data.push_back(v);
        }
    }
    for (unsigned stack = 0; stack < numStack; stack++)
    {
        for (unsigned slice = 0; slice < numSlice + 1; slice++)
        {
            index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
            index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
        }
    }
    Mesh *mesh = new Mesh(meshName);
    // Set the current active buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    // Transfer vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer); // bind index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

    return mesh;
}