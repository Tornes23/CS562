#include "Color.h"
#include <imgui/imgui.h>

//setting the static variables
Color Color::White = Color(1.0F, 1.0F, 1.0F, 1.0F);
Color Color::Black = Color(0.0F, 0.0F, 0.0F, 1.0F);
Color Color::Red = Color(1.0F, 0.0F, 0.0F, 1.0F);
Color Color::Green = Color(0.0F, 1.0F, 0.0F, 1.0F);
Color Color::Blue = Color(0.0F, 0.0F, 1.0F, 1.0F);

Color::Color(float r, float g, float b, float a)
{
	//setting the values
	mR = static_cast<unsigned char>(r * 255.99F);
	mG = static_cast<unsigned char>(g * 255.99F);
	mB = static_cast<unsigned char>(b * 255.99F);
	mA = static_cast<unsigned char>(a * 255.99F);
}

Color::Color(float c)
{
	mR = static_cast<unsigned char>(c * 255.99F);
	mG = static_cast<unsigned char>(c * 255.99F);
	mB = static_cast<unsigned char>(c * 255.99F);
	mA = static_cast <unsigned char>(255);
}

Color::Color(const std::vector<double>& c)
{
	//setting the values
	mR = static_cast<unsigned char>(c[0] * 255.99);
	mG = static_cast<unsigned char>(c[1] * 255.99);
	mB = static_cast<unsigned char>(c[2] * 255.99);
	mA = static_cast<unsigned char>(c[3] * 255.99);
}

Color::Color(const std::vector<float>& c)
{
	//setting the values
	mR = static_cast<unsigned char>(c[0] * 255.99F);
	mG = static_cast<unsigned char>(c[1] * 255.99F);
	mB = static_cast<unsigned char>(c[2] * 255.99F);
	mA = static_cast<unsigned char>(c[3] * 255.99F);
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	//setting the values
	mR = r;
	mG = g;
	mB = b;
	mA = a;
}

void Color::SetColor(glm::vec3& color)
{
	mR = static_cast<unsigned char>(color.r * 255);
	mG = static_cast<unsigned char>(color.g * 255);
	mB = static_cast<unsigned char>(color.b * 255);
	mA = 255;
}

void Color::SetColor(const std::vector<double>& c)
{
	mR = static_cast<unsigned char>(c[0] * 255.99);
	mG = static_cast<unsigned char>(c[1] * 255.99);
	mB = static_cast<unsigned char>(c[2] * 255.99);
	mA = static_cast<unsigned char>(c[3] * 255.99);
}

void Color::SetColor(const std::vector<float>& c)
{
	mR = static_cast<unsigned char>(c[0] * 255.99F);
	mG = static_cast<unsigned char>(c[1] * 255.99F);
	mB = static_cast<unsigned char>(c[2] * 255.99F);
	mA = static_cast<unsigned char>(c[3] * 255.99F);
}

glm::vec4 Color::GetColor()
{
	float r = static_cast<float>(mR) / 255.0F;
	float g = static_cast<float>(mG) / 255.0F;
	float b = static_cast<float>(mB) / 255.0F;
	float a = static_cast<float>(mA) / 255.0F;

	return glm::vec4(r, g, b, a);
}




