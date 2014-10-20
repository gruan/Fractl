#ifndef QUATERNION_H
#define QUATERNION_H

#include <Effect.hpp>
#include <math.h>
#include <Vec3.hpp>
#include <iostream>
class Quaternion : public Effect
{
public:
	Quaternion():
		Effect("Quaternion"),
		time(0)
	{}

	bool onLoad()
	{
		std::cout << "onLoad" << std::endl;
		if(!shader.loadFromFile(
			"C:\\Users\\Aaron\\NewShaderDistorted.frag",
			sf::Shader::Fragment)){
				
				std::cout << "failed to load" << std::endl;
				return false;
		}

		std::cout << "loaded" << std::endl;

		return true;
	}

	void onUpdate()
	{
		const float CAM_X = 2.f*sinf(time);
		const float CAM_Y = 2.f*cosf(time);
		const float CAM_Z = 0.f;

		const Vec3 CAM_POINTED_AT(0.f,0.f,0.f);

		Vec3 CAM_E(CAM_X, CAM_Y, CAM_Z);
		Vec3 lookdir = (CAM_POINTED_AT - CAM_E).normalized();
		Vec3 upvec = Vec3(0.f,0.f,1.f);

		Vec3 CAM_W = lookdir*Vec3(-1.f);
		Vec3 CAM_U = upvec.cross(CAM_W).normalized();
		Vec3 CAM_V = CAM_W.cross(CAM_U).normalized();

		const float C_X = -1.f;
		const float C_Y = 0.f;
		const float C_Z = 0.f;
		const float C_W = 0.f;
		
		shader.setParameter("C_X", C_X);
		shader.setParameter("C_Y", C_Y);
		shader.setParameter("C_Z", C_Z);
		shader.setParameter("C_W", C_W);


		shader.setParameter("CAM_X", CAM_X);
		shader.setParameter("CAM_Y", CAM_Y);
		shader.setParameter("CAM_Z", CAM_Z);

		shader.setParameter("CAM_UX", CAM_U.x);
		shader.setParameter("CAM_UY", CAM_U.y);
		shader.setParameter("CAM_UZ", CAM_U.z);
		shader.setParameter("CAM_VX", CAM_V.x);
		shader.setParameter("CAM_VY", CAM_V.y);
		shader.setParameter("CAM_VZ", CAM_V.z);
		shader.setParameter("CAM_WX", CAM_W.x);
		shader.setParameter("CAM_WY", CAM_W.y);
		shader.setParameter("CAM_WZ", CAM_W.z);

		
		shader.setParameter("RESOLUTION", sf::Vector2f(800.f, 600.f));
		shader.setParameter("TIME", time);
		time += 0.01f;
		
		screenRect.setPosition(0,0);
		screenRect.setSize(sf::Vector2f(800,600));
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
};

#endif