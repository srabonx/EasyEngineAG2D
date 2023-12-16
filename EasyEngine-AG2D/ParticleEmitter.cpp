#include "EasyEngineAG2D.h"

namespace EE {
	ParticleEmitter::ParticleEmitter()
	{
		// initialize particles to default
		image = NULL;
		max = 100;
		length = 100;
		direction = 0;
		alphaMin = 254;
		alphaMax = 255;
		minR = 0; maxR = 255;
		minG = 0; maxG = 255;
		minB = 0; maxB = 255;
		spread = 10;
		velocity = 1.0f;
		scale = 1.0f;
		lifeTime = 0;
	}

	ParticleEmitter::~ParticleEmitter()
	{
		delete image;
		// Destroy particles
		for (iter i = particles.begin(); i != particles.end(); ++i)
			delete (*i);
		particles.clear();
	}

	bool ParticleEmitter::loadImage(std::string imageFile)
	{
		image = new Texture();
		return image->Load(imageFile);
	}

	void ParticleEmitter::setAlphaRange(int min, int max)
	{
		alphaMin = min;
		alphaMax = max;
	}

	void ParticleEmitter::setColorRange(int r1, int g1, int b1, int r2, int g2, int b2)
	{
		minR = r1; maxR = r2;
		minG = g1; maxG = g2;
		minB = b1; maxB = b2;
	}


	void ParticleEmitter::draw()
	{
		// draw Particles
		for (iter i = particles.begin(); i != particles.end(); ++i)
			(*i)->draw();
	}

	void ParticleEmitter::update(int ms)
	{
		static Timer timer;
		// do we need to add new particles?
		if ((int)particles.size() < max)
		{
			// trivial but necessary slowdown
			if (timer.stopwatch(ms))
				add();
		}
		for (iter i = particles.begin(); i != particles.end(); ++i)
		{
			// update particles position
			(*i)->move();

			// is particle beyond emitter's range
			if ((*i)->getPosition().Distance(this->position) > length)
			{
				// reset particle to the origin
				(*i)->setX(position.getX());
				(*i)->setY(position.getY());
			}
		}
	}

	void ParticleEmitter::add()
	{
		static double PI_DIV_180 = 3.1415926535 / 180.0f;
		double vx, vy;

		// create new particle
		Sprite* p = new Sprite();
		p->setImage(image);
		p->setPosition(position.getX(), position.getY());

		// add randomness to the spread
		double variation = (rand() % spread - spread / 2) / 100.0f;

		// set Linear velocity
		double dir = direction - 90.0f;
		vx = cos(dir * PI_DIV_180) + variation;
		vy = sin(dir * PI_DIV_180) + variation;
		p->setVelocity(vx * velocity, vy * velocity);

		// set random color based on ranges
		int r = rand() % (maxR - minR) + minR;
		int g = rand() % (maxG - minG) + minG;
		int b = rand() % (maxB - minB) + minB;
		int a = rand() % (alphaMax - alphaMin) + alphaMin;
		p->setColor(D3DCOLOR_RGBA(r, g, b, a));

		// set scale
		p->setScale(scale);

		p->setLifeTime(lifeTime);

		// add particles to the emitter
		particles.push_back(p);
	}

	void ParticleEmitter::setLifeTime(int ms)
	{
		lifeTime = ms;
	}
};
