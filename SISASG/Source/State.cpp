#include "State.h"

GState::GState()
{
}


GState::~GState()
{
}

void GState::OnCreate(unsigned *parameters,FontLoader *St_FLInstance)
{
    this->state_params = parameters;
    this->St_FLInstance = St_FLInstance;
}

void GState::RenderText(Mesh* mesh, std::string text, Color color)
{
    if (!mesh || mesh->textureID <= 0) //Proper error check
        return;
    //glDisable(GL_DEPTH_TEST);
    glUniform1i(state_params[U_TEXT_ENABLED], 1);
    glUniform3fv(state_params[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(state_params[U_LIGHTENABLED], 0);
    glUniform1i(state_params[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(state_params[U_COLOR_TEXTURE], 0);


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
            res = (*this->St_FLInstance).getFontData((unsigned int)text[i]);
            buffer = res.font;
            characterSpacing.SetToTranslation(advance, yadvance, 0);
            
            Mtx44 MVP = (*this->projectionStack).Top() * (*this->viewStack).Top() * (*this->modelStack).Top() * characterSpacing;
            glUniformMatrix4fv(state_params[U_MVP], 1, GL_FALSE, &MVP.a[0]);
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
    glUniform1i(state_params[U_TEXT_ENABLED], 0);
    //glEnable(GL_DEPTH_TEST);
}

void GState::RenderMesh(Mesh *mesh, bool enableLight)
{
    Mtx44 MVP, modelView, modelView_inverse_transpose;

    MVP = (*this->projectionStack).Top() * (*this->viewStack).Top() * (*this->modelStack).Top();
    glUniformMatrix4fv(state_params[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    if (enableLight)
    {
        glUniform1i(state_params[U_LIGHTENABLED], 1);
        modelView = (*this->viewStack).Top() * (*this->modelStack).Top();
        glUniformMatrix4fv(state_params[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
        modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
        glUniformMatrix4fv(state_params[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

        //load material
        glUniform3fv(state_params[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
        glUniform3fv(state_params[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
        glUniform3fv(state_params[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
        glUniform1f(state_params[U_MATERIAL_SHININESS], mesh->material.kShininess);
    }
    else
    {
        glUniform1i(state_params[U_LIGHTENABLED], 0);
    }
    if (mesh->textureID > 0)
    {
        glUniform1i(state_params[U_COLOR_TEXTURE_ENABLED], 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->textureID);
        glUniform1i(state_params[U_COLOR_TEXTURE], 0);
    }
    else
    {
        glUniform1i(state_params[U_COLOR_TEXTURE_ENABLED], 0);
    }
    mesh->Render();
    if (mesh->textureID > 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}