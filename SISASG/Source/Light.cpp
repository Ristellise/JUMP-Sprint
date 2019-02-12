#include "Light.h"

Light::Light()
{
	type = Light::LIGHT_POINT;
	position.Set(0.0f, 10.0f, 0.0f);
	color.Set(1, 1, 1);
	power = 1.f;
	kC = 1.f;
	kL = 0.01f;
	kQ = 0.001f;
	cosCutoff = cos(Math::DegreeToRadian(45));
	cosInner = cos(Math::DegreeToRadian(30));
	exponent = 3.f;
	spotDirection.Set(0.f, 1.f, 0.f);
}

Light::~Light()
{

}
