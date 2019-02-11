#include "SceneWorld.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"



SceneWorld::SceneWorld()
{
}

SceneWorld::~SceneWorld()
{
}

void SceneWorld::Init()
{
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Generate a default VAO for now
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    //glEnable(GL_LIGHTING);

    //glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    camera.Init(Vector3(10, 10, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));

    Mtx44 projection;
    projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
    projectionStack.LoadMatrix(projection);

    //Load vertex and fragment shaders
    m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

    m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
    m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
    m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
    m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
    m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
    m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
    m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
    m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
    m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
    m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
    m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
    m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
    m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
    m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
    m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
    m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
    m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
    m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
    m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
    m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

    m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
    m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
    m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
    m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
    m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
    m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
    m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
    m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
    m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
    m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
    m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

    m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
    m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
    m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
    m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
    m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
    m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
    m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
    m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
    m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
    m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
    m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

    // Get a handle for our "colorTexture" uniform
    m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
    m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

    // Use our shader
    glUseProgram(m_programID);

    lights[0].type = Light::LIGHT_SPOT;
    lights[0].position.Set(-35.6f, 24.277f, -23);
    lights[0].color.Set(1, 1, 1);
    lights[0].power = 1;
    lights[0].kC = 1.f;
    lights[0].kL = 0.01f;
    lights[0].kQ = 0.001f;
    lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
    lights[0].cosInner = cos(Math::DegreeToRadian(30));
    lights[0].exponent = 3.f;
    lights[0].spotDirection.Set(-1.f, -0.f, 0.2f);

    lights[1].type = Light::LIGHT_POINT;
    lights[1].position.Set(-8.84f, 46.0f, -19.21f);
    lights[1].color.Set(1, 0, 0);
    lights[1].power = 0.0f;
    lights[1].kC = 1.f;
    lights[1].kL = 0.01f;
    lights[1].kQ = 0.001f;
    lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
    lights[1].cosInner = cos(Math::DegreeToRadian(30));
    lights[1].exponent = 3.f;
    lights[1].spotDirection.Set(-1.f, -0.f, 0.2f);

    lights[2].type = Light::LIGHT_POINT;
    lights[2].position.Set(-20.74f,45.75f, 4.93f);
    lights[2].color.Set(0, 0, 1);
    lights[2].power = 1;
    lights[2].kC = 1.f;
    lights[2].kL = 0.01f;
    lights[2].kQ = 0.001f;
    lights[2].cosCutoff = cos(Math::DegreeToRadian(45));
    lights[2].cosInner = cos(Math::DegreeToRadian(30));
    lights[2].exponent = 3.f;
    lights[2].spotDirection.Set(-1.f, -0.f, 0.2f);

    glUniform1i(m_parameters[U_NUMLIGHTS], 3);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

    glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
    glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
    glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
    glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
    glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
    glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
    glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
    glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

    glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
    glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
    glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
    glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
    glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
    glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
    glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
    glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

    glUniform1i(m_parameters[U_LIGHT2_TYPE], lights[2].type);
    glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &lights[2].color.r);
    glUniform1f(m_parameters[U_LIGHT2_POWER], lights[2].power);
    glUniform1f(m_parameters[U_LIGHT2_KC], lights[2].kC);
    glUniform1f(m_parameters[U_LIGHT2_KL], lights[2].kL);
    glUniform1f(m_parameters[U_LIGHT2_KQ], lights[2].kQ);
    glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], lights[2].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT2_COSINNER], lights[2].cosInner);
    glUniform1f(m_parameters[U_LIGHT2_EXPONENT], lights[2].exponent);

    meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 200, 200, 200);

    //** FontLoader Instance **//
    FLInstance.Loadfnt("Font/fnt.fnt");

    //// The fontsheet on a big mesh
    meshList[GEO_TEXT] = MeshBuilder::GenerateText("saofontsheet", this->FLInstance);
    meshList[GEO_TEXT]->textureID = LoadTGA("Font//fnt_0.tga", GL_LINEAR, GL_REPEAT);

}

void SceneWorld::Update(double dt)
{
    static const float LSPEED = 10.0f;

    if (Application::IsKeyPressed('1'))
    {
        glEnable(GL_CULL_FACE);
    }
    if (Application::IsKeyPressed('2'))
    {
        glDisable(GL_CULL_FACE);
    }
    if (Application::IsKeyPressed('3'))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (Application::IsKeyPressed('4'))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (Application::IsKeyPressed(VK_SPACE))
    {

    }
    if (Application::IsKeyPressed('I'))
        lights[this->selector].position.z -= (float)(LSPEED * dt);
    if (Application::IsKeyPressed('K'))
        lights[this->selector].position.z += (float)(LSPEED * dt);
    if (Application::IsKeyPressed('J'))
        lights[this->selector].position.x -= (float)(LSPEED * dt);
    if (Application::IsKeyPressed('L'))
        lights[this->selector].position.x += (float)(LSPEED * dt);
    if (Application::IsKeyPressed('O'))
        lights[this->selector].position.y -= (float)(LSPEED * dt);
    if (Application::IsKeyPressed('P'))
        lights[this->selector].position.y += (float)(LSPEED * dt);
    
    if (Application::IsKeyPressed('9') && !this->opendoor && this->lastkeypress > 0.5)
    {
        this->opendoor = true;
        std::cout << "Open door" << std::endl;
        this->lastkeypress = 0.0;
    }
    else if (Application::IsKeyPressed('9') && this->opendoor && this->lastkeypress > 0.5)
    {
        this->opendoor = false;
        std::cout << "Open door" << std::endl;
        this->lastkeypress = 0.0;
    }
    if (this->doormovement < 15.0f && this->opendoor)
    {
        this->doormovement += 10.0f * (float)dt;
    }
    else if (!this->opendoor && this->doormovement > 0.0)
    {
        this->doormovement -= 10.0f * (float)dt;
    }

    if (Application::IsKeyPressed('8') && !this->openlid && this->lastkeypress > 0.5)
    {
        this->openlid = true;
        std::cout << "Open lid" << std::endl;
        this->lastkeypress = 0.0;
    }
    else if (Application::IsKeyPressed('8') && this->openlid && this->lastkeypress > 0.5)
    {
        this->openlid = false;
        std::cout << "lid closed" << std::endl;
        this->lastkeypress = 0.0;
    }
    if (
        ((this->camera.pos.x < this->lights[1].position.x - 5.0f) || (this->camera.pos.x < this->lights[1].position.x + 5.0f)) &&
        ((this->camera.pos.y < this->lights[1].position.y - 5.0f) || (this->camera.pos.y < this->lights[1].position.y + 5.0f)) &&
        ((this->camera.pos.z < this->lights[1].position.z - 5.0f) || (this->camera.pos.z < this->lights[1].position.z + 5.0f))
        )
    {
        this->renable = true;
    }
    else
    {
        this->renable = false;
    }
    if (this->lidmovement < 0.1f && this->openlid)
    {
        this->lidmovement += 1.0f * (float)dt;
    }
    else if (!this->openlid && this->lidmovement > 0.0)
    {
        this->lidmovement -= 1.0f * (float)dt;
    }
    if (this->renable && this->lights[1].power < 1.0f)
    {
        this->lights[1].power += 0.5f * (float)dt;
        glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
    }
    else if (!this->renable  && this->lights[1].power > 0.0f)
    {
        this->lights[1].power -= .5f * (float)dt;
        glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
    }

    if (Application::IsKeyPressed('7'))
    {
        this->leg = true;
        this->legdance += 10.f * (float)dt;
    }
    else
    {
        this->leg = false;
    }
    this->lastkeypress += dt;
    rotateStar += (float)10 * (float)dt;
    camera.Update(dt);
    this->dtimestring = "FPS:";
    this->dtimestring += std::to_string(1.0f / dt);
    this->dtimestring += "\nCam X:";
    this->dtimestring += std::to_string(this->camera.pos.x);
    this->dtimestring += "\nCam Y:";
    this->dtimestring += std::to_string(this->camera.pos.y);
    this->dtimestring += "\nCam Z:";
    this->dtimestring += std::to_string(this->camera.pos.z);
    this->dtimestring += "\n" + std::to_string(this->lights[this->selector].position.x) + "| " 
                       + std::to_string(this->lights[this->selector].position.y) + "| "
                       + std::to_string(this->lights[this->selector].position.z);

    this->dtimestring += "\nDoor Opened? :";
    this->dtimestring += std::to_string(this->opendoor);
}

void SceneWorld::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

static const float SKYBOXSIZE = 200.0f;

// Main Render loop
void SceneWorld::Render()
{
    //Clear color & depth buffer every frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    viewStack.LoadIdentity();

    viewStack.LookAt(camera.pos.x, camera.pos.y, camera.pos.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
    modelStack.LoadIdentity();
    if (lights[0].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
            &lightDirection_cameraspace.x);
    }
    else if (lights[0].type == Light::LIGHT_SPOT)
    {
        Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
            &lightPosition_cameraspace.x);
        Vector3 spotDirection_cameraspace = viewStack.Top() *
            lights[0].spotDirection;
        glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1,
            &spotDirection_cameraspace.x);
    }
    else
    {
        Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
            &lightPosition_cameraspace.x);
    }

    modelStack.PushMatrix();
        modelStack.Translate(0, 0, 0);
        RenderMesh(meshList[GEO_AXES], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
        modelStack.Translate(5.0f, 10.0f, 2.0f);
        RenderText(meshList[GEO_TEXT], this->dtimestring , Color(255, 255, 0));
    modelStack.PopMatrix();
}

/*-------------
| - Self Renderer Works... but derpy.
-------------*/
void SceneWorld::RenderText(Mesh* mesh, std::string text, Color color)
{
    if (!mesh || mesh->textureID <= 0) //Proper error check
        return;
    //glDisable(GL_DEPTH_TEST);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);


    float advance = 0;
    float yadvance = 0.0f;
    // 
    charData buffer;
    FontResult res;
    for (unsigned i = 0; i < text.length(); ++i)
    {
        if (text[i] != '\n')
        {
            Mtx44 characterSpacing;
            res = this->FLInstance.getFontData((unsigned int)text[i]);
            buffer = res.font;
            characterSpacing.SetToTranslation(advance, yadvance, 0);
            Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
            glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
            advance += 1.0f / (float)buffer.advance + 0.3f; // advance the text
            mesh->Render(res.index * 6, 6); // count is the index Size.
        }
        else
        {
            yadvance -= 1.0f;
            advance = 0.0f;
        }
        
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    //glEnable(GL_DEPTH_TEST);
}

void SceneWorld::Exit()
{
    // Cleanup VBO here
    glDeleteProgram(m_programID);

}
