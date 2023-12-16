#include "EasyEngineAG2D.h"
#pragma once

namespace EE {
	enum CollisionType {
		COLLISION_NONE = 0,
		COLLISION_RECT = 1,
		COLLISION_DIST = 2
	};

	class Sprite:public Entity
	{
	private:

		Vector3 position;
		Vector3 velocity;
		bool imageLoaded;
		int state;
		int direction;

	protected:
		Texture* image;
		int width;
		int height;
		int animColumns;
		int frameStart;
		int frameTimer;
		int moveStart;
		int moveTimer;
		bool collideable;
		enum CollisionType collisionMethod;
		int curFrame, totalFrame, animdir;
		double faceAngle, moveAngle;
		int animstarx, animstarty;
		double rotation, scaling;
		D3DXMATRIX matRotate;
		D3DXMATRIX matScale;
		void transform();
		D3DCOLOR color;

	public:
		// Screen position
		Vector3 getPosition() { return position; }
		void setPosition(const Vector3& pos) { position = pos; }
		void setPosition(double x, double y, double z = 0) { position.Set(x, y, z); }
		double getX() { return position.getX(); }
		double getY() { return position.getY(); }
		void setX(double x) { position.setX(x); }
		void setY(double y) { position.setY(y); }

		// Movement velocity
		Vector3 getVelocity() { return velocity; }
		void setVelocity(const Vector3& vel) { velocity = vel; }
		void setVelocity(double x, double y) { velocity.setX(x); velocity.setY(y); }

		// Image size
		void setSize(int width, int height) { this->width = width; this->height = height; }
		int getWidth() { return width; }
		void setWidth(int w) { width = w; }
		int getHeight() { return height; }
		void setHeight(int h) { height = h; }

		int getState() { return state; }
		void setState(int value) { state = value; }

		int getDirection() { return direction; }
		void setDirection(int dir) { direction = dir; }

		int getColums() { return animColumns; }
		void setColumns(int value) { animColumns = value; }

		int getFrameTimer() { return frameTimer; }
		void setFrameTimer(int value) { frameTimer = value; }

		int getCurrentFrame() { return curFrame; }
		void setCurrentFrame(int value) { curFrame = value; }

		int getTotalFrame() { return totalFrame; }
		void setTotalFrame(int value) { totalFrame = value; }

		int getAnimationDirection() { return animdir; }
		void setAnimationDirection(int value) { animdir = value; }

		double getRotation() { return rotation; }
		void setRotation(double value) { rotation = value; }

		double getScale() { return scaling; }
		void setScale(double value) { scaling = value; }

		void setColor(D3DCOLOR col) { color = col; }
		D3DCOLOR getColor() { return color; }

		int getMoveTimer() { return moveTimer; }
		void setMoveTimer(int value) { moveTimer = value; }

		bool isCollidable() { return collideable; }
		void setCollidable(int value) { collideable = value; }

		CollisionType getCollisionMethod() { return collisionMethod; }
		void setCollisionMethod(CollisionType type) { collisionMethod = type; }

	public:
		Sprite();
		virtual ~Sprite();
		bool loadImage(std::string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255));
		void setImage(Texture* tx);
		void move();
		void animate();
		void draw();
	
	}; // class
};// namespace

