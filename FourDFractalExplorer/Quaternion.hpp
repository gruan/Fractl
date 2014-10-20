#ifndef QUATERNION_H
#define QUATERNION_H

#include "CONSTANTS.H"
#include <Effect.hpp>
#include <math.h>
#include <Vec3.hpp>
#include <iostream>

#include "Main.h"
#include "Leap.cpp"
class Quaternion : public Effect
{
public:
	Quaternion():
		Effect("Quaternion"),
		time(0),
		rotation(0),
		thetax(0),
		thetay(0)
	{}

	bool onLoad()
	{
		std::cout << "onLoad" << std::endl;
		if(!shader.loadFromFile(
			"I:\\NewShaderDistorted.frag",
			sf::Shader::Fragment)){

				std::cout << "failed to load" << std::endl;
				if(!shader.loadFromFile(
					"c:\\Users\\Aaron\\NewShaderDistorted.frag",
					sf::Shader::Fragment)){
						std::cout << "failed to load" << std::endl;
						return false;
				}
		}

		std::cout << "loaded" << std::endl;

		return true;
	}

	void onUpdate()
	{
		FracListener * fracListener1 = getListener();

		FracListener fracListener = *fracListener1;

		float rad = fracListener.radius/30.;
		
		thetax -= 0.01f;// fracListener.xRotation / 1000.f;
		thetay -= fracListener.yRotation/1000.f;
		fracListener.xRotation *= 0.95;
		fracListener.yRotation *= 0.95;
		
		const float CAM_X = rad*sinf(thetax);
		const float CAM_Y = rad*cosf(thetax);
		const float CAM_Z = 0;//rad*sin(thetay);

		
		//const float CAM_X = 2.f*sinf(rotation);
		//const float CAM_Y = 2.f*cosf(rotation);
		//const float CAM_Z = 0.f;

		const Vec3 CAM_POINTED_AT(0.f,0.f,0.f);

		Vec3 CAM_E(CAM_X, CAM_Y, CAM_Z);
		Vec3 lookdir = (CAM_POINTED_AT - CAM_E).normalized();
		Vec3 upvec = Vec3(0.f,0.f,1.f);

		Vec3 CAM_W = lookdir*Vec3(-1.f);
		Vec3 CAM_U = upvec.cross(CAM_W).normalized();
		Vec3 CAM_V = CAM_W.cross(CAM_U).normalized();

		const float C_X = fracListener.a/2.;
		const float C_Y = fracListener.b/2.;
		const float C_Z = fracListener.c/2.;
		const float C_W = fracListener.d/2.;

		shader.setParameter("C_X", C_X);
		shader.setParameter("C_Y", C_Y);
		shader.setParameter("C_Z", C_Z);
		shader.setParameter("C_W", C_W);

		shader.setParameter("CAM_X", CAM_X);
		shader.setParameter("CAM_Y", CAM_Y);
		//shader.setParameter("CAM_Z", CAM_Z);

		shader.setParameter("CAM_UX", CAM_U.x);
		shader.setParameter("CAM_UY", CAM_U.y);
		shader.setParameter("CAM_UZ", CAM_U.z);
		shader.setParameter("CAM_VX", CAM_V.x);
		shader.setParameter("CAM_VY", CAM_V.y);
		shader.setParameter("CAM_VZ", CAM_V.z);
		shader.setParameter("CAM_WX", CAM_W.x);
		shader.setParameter("CAM_WY", CAM_W.y);
		shader.setParameter("CAM_WZ", CAM_W.z);

		shader.setParameter("eye", sf::Vector3f(CAM_X, CAM_Y, CAM_Z));
		shader.setParameter("axisX", sf::Vector3f(CAM_U.x, CAM_U.y, CAM_U.z));
		shader.setParameter("axisZ", -sf::Vector3f(CAM_W.x, CAM_W.y, CAM_W.z));
		shader.setParameter("axisY", sf::Vector3f(CAM_V.x, CAM_V.y, CAM_V.z));
		//shader.setParameter("axisX", sf::Vector3f(-1,0,0));
		//shader.setParameter("axisY", sf::Vector3f(0,1,0));
		//shader.setParameter("axisZ", sf::Vector3f(0,0,1));


		shader.setParameter("RESOLUTION", sf::Vector2f(WIDTH, HEIGHT));
		shader.setParameter("TIME", time);
		time += 0.005f;
		rotation += 0.005f;

		screenRect.setPosition(0,0);
		screenRect.setSize(sf::Vector2f(WIDTH,HEIGHT));
	}

	void onDraw(sf::RenderTarget &target, sf::RenderStates states) const
	{


		states.shader = &shader;
		target.draw(screenRect, states);
	}

	virtual void onMouseButtonRelease(sf::Event event)
	{}

private:
	sf::Shader shader;

	sf::RectangleShape screenRect;
	float time;
	float rotation;
	float thetax, thetay;
};

#endif