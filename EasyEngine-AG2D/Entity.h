#include "EasyEngineAG2D.h"
#pragma once
namespace EE {

	enum RenderType {
		Render2D,
		Render3D
	};

	class Entity
	{
	private:
		int id;
		std::string name;
		bool visible;
		bool alive;
		enum RenderType renderType;
		int objectType;
		int lifeTimeLength;
		Timer lifeTimeTimer;

	public:
		Entity(enum RenderType renderType);
		virtual ~Entity() {}
		virtual void move() = 0;
		virtual void animate() = 0;
		virtual void draw() = 0;
		void setID(int value) { id = value; }
		int getId() { return id; }
		void setRenderType(enum RenderType type) { renderType = type; }
		enum RenderType getRenderType() { return renderType; }
		std::string getName() { return name; }
		void setName(std::string value) { name = value; }
		bool getVisible() { return visible; }
		void setVisible(bool value) { visible = value; }
		bool getAlive() { return alive; }
		void setAlive(bool value) { alive = value; }
		int getLifeTime() { return lifeTimeLength; }
		void setLifeTime(int millisecond) {
			lifeTimeLength = millisecond;
			lifeTimeTimer.reset();
		}
		bool lifeTimeExpired() {
			return lifeTimeTimer.stopwatch(lifeTimeLength);
		}
		int getObjectType() { return objectType; }
		void setObjectType(int value) { objectType = value; }
	};	// Class
}; // Namespace
