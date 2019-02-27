#include "Light.h"

Light::Light()
{
    type = Light::LIGHT_POINT;
    position.Set(0.0f, 10.0f, 0.0f);
    color.Set(1, 1, 1);

	/*
	power = 1.f;
    kC = 1.f;
    kL = 0.01f;
    kQ = 0.001f;
	*/

	power = 350.f;
	kC = 1.f;
	kL = 0.00001f;
	kQ = 0.00001f;
    cosCutoff = cos(Math::DegreeToRadian(45));
    cosInner = cos(Math::DegreeToRadian(30));
    exponent = 3.f;
    spotDirection.Set(0.f, 1.f, 0.f);
    glUniform1i(m_parameters[U_LIGHT0_TYPE], type);
    glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &color.r);
    glUniform1f(m_parameters[U_LIGHT0_POWER], power);
    glUniform1f(m_parameters[U_LIGHT0_KC], kC);
    glUniform1f(m_parameters[U_LIGHT0_KL], kL);
    glUniform1f(m_parameters[U_LIGHT0_KQ], kQ);
    glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], cosCutoff);
    glUniform1f(m_parameters[U_LIGHT0_COSINNER], cosInner);
    glUniform1f(m_parameters[U_LIGHT0_EXPONENT], exponent);
}

Light::~Light()
{

}

void Light::setType(std::string type)
{
    if (type == "Point")
        this->type = LIGHT_POINT;
    else if (type == "Spot")
        this->type = LIGHT_SPOT;
    else if (type == "Directional")
        this->type = LIGHT_DIRECTIONAL;
}

