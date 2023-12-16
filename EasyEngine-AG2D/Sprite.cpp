#include "EasyEngineAG2D.h"
namespace EE {
	
	Sprite::Sprite():Entity(Render2D)
	{
		image = NULL;
		imageLoaded = false;
		setPosition(0.0f, 0.0f);
		setVelocity(0.0f, 0.0f);
		state = 0;
		direction = 0;
		width = 1;
		height = 1;
		curFrame = 0;
		totalFrame = 1;
		animdir = 1;
		animColumns = 1;
		frameStart = 0;
		frameTimer = 0;
		animstarx = 0;
		animstarty = 0;
		faceAngle = 0;
		moveAngle = 0;
		rotation = 0;
		scaling = 1.0f;
		color = 0xFFFFFFFF;
		moveTimer = 16;
		moveStart = 0;
		collideable = true;
		collisionMethod = COLLISION_RECT;

	}

	Sprite::~Sprite()
	{
		if (imageLoaded) {
			delete image;
			image = NULL;
		}
	}

	bool Sprite::loadImage(std::string filename, D3DCOLOR transcolor)
	{
		// if image already exists, free it
		if (imageLoaded && image != NULL) delete image;

		// Create texture and load image
		image = new Texture();
		if (image->Load(filename, transcolor))
		{
			this->setSize(image->getWidth(), image->getHeight());
			imageLoaded = true;
			return true;
		}
		else
			return false;
	}

	void Sprite::setImage(Texture* tx)
	{
		image = tx;
		setWidth(image->getWidth());
		setHeight(image->getHeight());
		imageLoaded = false;
	}

	void Sprite::move()
	{
		if (moveTimer > 0) {
			if (timeGetTime() > (DWORD)(moveStart + moveTimer)) {
				// reset move timer
				moveStart = timeGetTime();

				// move sprite by velocity amount
				this->setX(this->getX() + this->velocity.getX());
				this->setY(this->getY() + this->velocity.getY());
			}
		}
		else {
			// no movement timer -- update at CPU clock speed
			this->setX(this->getX() + this->velocity.getX());
			this->setY(this->getY() + this->velocity.getY());
		}
	}

	void Sprite::animate()
	{
		// update frame based on animdir
		if (frameTimer > 0)
		{
			if (timeGetTime() > (DWORD)(frameStart + frameTimer))
			{
				// reset animation timer
				frameStart = timeGetTime();
				curFrame += animdir;

				// keep frame within bounds
				if (curFrame < 0) curFrame = totalFrame - 1;
				if (curFrame > totalFrame - 1)curFrame = 0;
			}
		}
		else
		{
			// no animation timer -- update at CPU clock speed
			
			curFrame += animdir;

			if (curFrame < 0) curFrame = totalFrame - 1;
			if (curFrame > totalFrame - 1)curFrame = 0;
		}
	}

	void Sprite::draw()
	{
		// calculate source frame location
		int fx = (curFrame % animColumns) * width;
		int fy = (curFrame / animColumns) * height;
		RECT srcRect = { fx,fy,fx + width,fy + height };

		// draw the sprite frame
		this->transform();
		
		g_engine->getSpriteHandler()->Draw(this->image->GetTexture(), &srcRect, NULL, NULL, color);
	}

	void Sprite::transform()
	{
		D3DXMATRIX mat;
		D3DXVECTOR2 scale((float)scaling, (float)scaling);
		D3DXVECTOR2 center((float)(width * scaling) / 2, (float)(height * scaling) / 2);
		D3DXVECTOR2 trans((float)getX(), (float)getY());
		D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, (float)rotation, &trans);
		g_engine->getSpriteHandler()->SetTransform(&mat);
	}
}; // namespace
