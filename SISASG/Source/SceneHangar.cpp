#include "SceneHangar.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"



SceneHangar::SceneHangar()
{
}

SceneHangar::~SceneHangar()
{
}

void SceneHangar::Init()
{

#pragma region StartupLoads // Just Compressing Space for scrolling //

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(0, 10, 120), Vector3(0, 0, 0), Vector3(0, 1, 0));

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

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Use our shader
	glUseProgram(m_programID);

#pragma endregion
	lights[0].position.Set(camera.position.x, 45, 0);
	lights[0].power = 50.f;
	lights[0].type = lights->LIGHT_SPOT;
	lights[0].spotDirection.Set(0, 1, 0);

	glUniform1i(m_parameters[U_NUMLIGHTS], 8);
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

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 200, 200, 200);

	//** FontLoader Instance **//
	FLInstance.Loadfnt("Font/fnt.fnt");

	//// The fontsheet on a big mesh
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("saofontsheet", this->FLInstance);
	meshList[GEO_TEXT]->textureID = LoadTGA("Font//fnt_0.tga", GL_LINEAR, GL_REPEAT);

	// Test Cube
	meshList[GEO_TESTCUBE] = MeshBuilder::GenerateOBJ("testcube", "OBJ//TestCube.obj")[0];
	meshList[GEO_TESTCUBE]->textureID = LoadTGA("TGA//TestCube.tga", GL_LINEAR, GL_CLAMP);

	// Lightball
	meshList[GEO_LIGHT] = MeshBuilder::GenerateOBJ("Ceilinglight", "OBJ//Hangar_Ceilinglight.obj")[0];
	meshList[GEO_LIGHT]->textureID = LoadTGA("TGA//Hangar_Ceilinglight.tga", GL_LINEAR, GL_CLAMP);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Left_Wall", Color((248/255.f), (248/255.f), (255/255.f)), 1.f);
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Right_Wall", Color((248/255.f), (248/255.f), (255/255.f)), 1.f);
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Back", Color((248/255.f), (248/255.f), (255/255.f)), 1.f);
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Floor", Color((248/255.f), (248/255.f), (255/255.f)), 1.f);
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Roof", Color((248/255.f), (248/255.f), (255/255.f)), 1.f);


}

static const float SKYBOXSIZE = 150.0f;

void SceneHangar::Update(double dt)
{
	static const float LSPEED = 10.0f;
	
	 //For movement
	if (Application::IsKeyPressed('A') && (camera.position.x > 0) && Delay == 0)
	{
		Delay += 10;
		Shift = -SKYBOXSIZE / Delay;
		shiftmovement = true;
	}

	if (Application::IsKeyPressed('D') && (camera.position.x < SKYBOXSIZE*(NumberOfShips-1)) && Delay == 0)
	{
		Delay += 10;
		Shift = SKYBOXSIZE / Delay;
		shiftmovement = true;
	}

	if (Delay > 0) // Handles movement
	{
		if (shiftmovement)
		{
			camera.position.x += Shift;
			camera.target.x += Shift;
			lights[0].position.x = camera.position.x;
		}
		Delay--;
	}
	else
	{
		shiftmovement = false;
	}

	// For culling and line / fill modes
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
	if (Application::IsKeyPressed(VK_SPACE) && Delay == 0)
	{
		if (lit == true)
			lit = false;
		else
			lit = true;
		Delay += 10;
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

	this->lastkeypress += dt;
	camera.Updatealt(dt);
	this->dtimestring = "FPS:";
	this->dtimestring += std::to_string(1.0f / dt);
	this->dtimestring += "\nCam X:";
	this->dtimestring += std::to_string(this->camera.position.x);
	this->dtimestring += "\nCam Y:";
	this->dtimestring += std::to_string(this->camera.position.y);
	this->dtimestring += "\nCam Z:";
	this->dtimestring += std::to_string(this->camera.position.z);
	this->dtimestring += "\n" + std::to_string(this->lights[this->selector].position.x) + " | "
		+ std::to_string(this->lights[this->selector].position.y) + " | "
		+ std::to_string(this->lights[this->selector].position.z);
}

void SceneHangar::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight)
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
	if (mesh->textureID > 0)
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
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


// Main Render loop
void SceneHangar::Render()
{

#pragma region Before Pushmatrix and such // Just Compressing for Scrolling

	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();

	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
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

#pragma endregion

	RenderSkybox();

	//modelStack.PushMatrix();
	//modelStack.Translate(0, 0, 0);
	//RenderMesh(meshList[GEO_AXES], false);
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(5.0f, 5.0f, 5.0f);
	RenderMesh(meshList[GEO_TESTCUBE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5.0f, 10.0f, 2.0f);
	RenderTextOnScreen(meshList[GEO_TEXT], this->dtimestring, Color(255, 255, 0), 1.5,1,1);
	modelStack.PopMatrix();

}

/*-------------
| - Self Renderer Works... but derpy.
-------------*/
void SceneHangar::RenderText(Mesh* mesh, std::string text, Color color)
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
			mesh->Render(res.index * 6, 6); // count is the index Size
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

// Based on RenderText
void SceneHangar::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;

	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	float txt_vertical = 0;
	float txt_horizontal = 0;

	charData buffer;
	FontResult res;

	for (unsigned i = 0; i < text.length(); ++i)
	{

		Mtx44 characterSpacing;
		res = this->FLInstance.getFontData((unsigned int)text[i]);
		buffer = res.font;
		characterSpacing.SetToTranslation(txt_horizontal * 0.75f, txt_vertical, 0); //1.0f is the spacing of each character, you may change this value
		txt_horizontal += 1;
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render(res.index * 6, 6); // count is the index Size
		if (text[i] == '\n')
		{
			txt_vertical -= 1.f;
			txt_horizontal = 0;
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

}

void SceneHangar::RenderSkybox()
{

	for (int j = 0; 2 > j; j++)
	{
		//left
		modelStack.PushMatrix();
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(j*-SKYBOXSIZE, 0, -SKYBOXSIZE / 2 + 2.f);
		modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
		RenderMesh(meshList[GEO_LEFT], true);
		modelStack.PopMatrix();

		//right
		modelStack.PushMatrix();
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Translate(j*SKYBOXSIZE, 0, (NumberOfShips *-SKYBOXSIZE) + (SKYBOXSIZE / 2 + 2.f));
		modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
		RenderMesh(meshList[GEO_RIGHT], true);
		modelStack.PopMatrix();

		// Adjusts for Vertical and back skyboxing
		for (int i = 0; NumberOfShips > i; i++)
		{
			// Back
			modelStack.PushMatrix();
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Translate(-i * SKYBOXSIZE, 0, 3*(-SKYBOXSIZE / 3 + 2.f));
			modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
			RenderMesh(meshList[GEO_BACK], true);
			modelStack.PopMatrix();

			// Top
			modelStack.PushMatrix();
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Translate(i*SKYBOXSIZE, (j - 1)*-SKYBOXSIZE, -SKYBOXSIZE / 3 + 2.f);
			modelStack.Rotate(90, 0, 0, 1);
			modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
			RenderMesh(meshList[GEO_TOP], true);
			modelStack.PopMatrix();

			// Bottom
			modelStack.PushMatrix();
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Translate(i*SKYBOXSIZE, (j-1)*SKYBOXSIZE, (-SKYBOXSIZE / 3 + 2.f));
			modelStack.Rotate(-90, 0, 0, 1);
			modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
			RenderMesh(meshList[GEO_BOTTOM], true);
			modelStack.PopMatrix();
		}
	}

}



void SceneHangar::Exit()
{
	// Cleanup VBO here
	glDeleteProgram(m_programID);

}