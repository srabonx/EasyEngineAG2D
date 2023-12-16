// Easy Engine - AdvanceGameEngine2D
// Main source code file

//includes

#include "EasyEngineAG2D.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <list>
#include "winmain.h"

namespace EE {

	Engine::Engine() {
		srand((unsigned int)time(NULL));
		p_maximizeProcessor = false;
		p_frameCount_core = 0;
		p_frameRate_core = 0;
		p_frameCount_real = 0;
		p_frameRate_real = 0;
		p_ambientColor = D3DCOLOR_RGBA(255, 255, 255, 0);
		p_windowHandle = 0;
		p_pauseMode = false;
		p_versionMajor = VERSION_MAJOR;
		p_versionMinor = VERSION_MINOR;
		p_revision = REVISION;
		// Set default values

		this->setAppTitle("Easy Engine");
		this->setScreenWidth(640);
		this->setScreenHeight(480);
		this->setColorDepth(32);
		this->setFullScreen(false);

		// Windows Handle must set later for DirectX

		this->setWindowHandle(0);
	}

	Engine::~Engine(){
		audio->StopAll();
		delete audio;
		delete p_input;
		if (this->p_device) this->p_device->Release();
		if (this->p_d3d) this->p_d3d->Release();
	}

	std::string Engine::getVersionText() {
		std::ostringstream s;

		s << "Easy Engine - AdvanceGameEngine2D " << p_versionMajor << "." << p_versionMinor << "." << p_revision;

		return s.str();
	}

	void Engine::message(std::string message, std::string title) {
		MessageBox(0, message.c_str(), title.c_str(), 0);
	}

	void Engine::fatalerror(std::string message, std::string title) {
		this->message(message, title);
		Shutdown();
	}

	int Engine::Init(int width, int height, int colordepth, bool fullscreen) {

		// Initialize Direct3D
		this->p_d3d = Direct3DCreate9(D3D_SDK_VERSION);

		// Initialize Direct Input
		this->p_input = new Input(this->p_windowHandle);

		// Initialize Entity list
		p_entities = new std::list<Entity*>();

		// Initialize math
		math = new Math();

		// Create audio system
		audio = new Audio();
		if (!audio->Init()) return 0;

		if (this->p_d3d == NULL)
			return 0;

		// Get system desktop color depth
		D3DDISPLAYMODE dm;
		this->p_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);

		// Set configuration option for Direct3D
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = (!fullscreen);
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.BackBufferFormat = dm.Format;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		d3dpp.hDeviceWindow = this->p_windowHandle;

		// Create Direct3D device
		this->p_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->p_windowHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &this->p_device);

		if (this->p_device == NULL)
			return 0;

		// Clear Back buffer to black
		this->ClearScene(D3DCOLOR_XRGB(0, 0, 0));

		// Create Pointer to back buffer
		this->p_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->p_backbuffer);

		// Use ambient lighting and z-buffering
		this->p_device->SetRenderState(D3DRS_ZENABLE, true);
		this->p_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		this->SetAmbient(this->p_ambientColor);

		// Initialize 2D Renderer
		HRESULT result = D3DXCreateSprite(this->p_device, &this->p_sprite_handler);

		if (result != D3D_OK) return 0;



		// Call Game initialization extern function
		if (!game_init(this->getWindowHandle())) return 0;

		// Set Default Material
		SetDefaultMaterial();

		// Convert bury entities into a threaded function
		mutex = PTHREAD_MUTEX_INITIALIZER;
		pthread_t thread_bury_entities;
		int threadid = 1;
		int id = pthread_create(&thread_bury_entities, NULL, thread_function_bury_entities, (void*)&threadid);

		return 1;
	}

	void Engine::SetDefaultMaterial() {
		D3DMATERIAL9 mat;
		memset(&mat, 0, sizeof(mat));
		mat.Diffuse.r = 1.0f;
		mat.Diffuse.g = 1.0f;
		mat.Diffuse.b = 1.0f;
		mat.Diffuse.a = 1.0f;
		p_device->SetMaterial(&mat);
	}

	void Engine::ClearScene(D3DCOLOR color) {
		this->p_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
	}

	void Engine::SetAmbient(D3DCOLOR colorvalue) {
		this->p_ambientColor = colorvalue;
		this->p_device->SetRenderState(D3DRS_AMBIENT, this->p_ambientColor);
	}

	void Engine::SetIdentity()
	{
		D3DXMATRIX matrixWorld;
		D3DXMatrixIdentity(&matrixWorld);

		HRESULT result = g_engine->getDevice()->SetTransform(D3DTS_WORLD, &matrixWorld);

		if (result != D3D_OK)
			MessageBox(NULL, "Error Setting Identity!", "Error", MB_OK);
		
	}

	int Engine::RenderStart() {
		if (!this->p_device) return 0;
		if (this->p_device->BeginScene() != D3D_OK) return 0;
		return 1;
	}

	int Engine::RenderStop() {
		if (!this->p_device) return 0;
		if (this->p_device->EndScene() != D3D_OK) return 0;
		if (this->p_device->Present(NULL, NULL, NULL, NULL) != D3D_OK) return 0;
		return 1;
	}

	int Engine::Render2D_Start()
	{	
		if (p_sprite_handler->Begin(D3DXSPRITE_ALPHABLEND) != D3D_OK)
			return 0;
		else
			return 1;
	}

	int Engine::Render2D_Stop()
	{
		p_sprite_handler->End();
		return 1;
	}

	int Engine::Release()
	{
		return 0;
	}

	void Engine::UpdateKeyboard()
	{
		static char old_keys[256];
		for (int i = 0; i < 255; i++)
		{
			// Check for key presses
			if (p_input->GetKeyState(i) & 0x80)
			{
				game_keyPress(i);
				old_keys[i] = p_input->GetKeyState(i);
			}
			// Check for key release
			else if (old_keys[i] & 0x80)
			{
				game_keyRelease(i);
				old_keys[i] = p_input->GetKeyState(i);
			}
		}
	}

	void Engine::UpdateMouse()
	{
		static int oldPosX = 0;
		static int oldposY = 0;
		int deltaX = p_input->GetDeltaX();
		int deltaY = p_input->GetDeltaY();

		// Check mouse button 1-3
		for (int i = 0; i < 4; i++)
			if (p_input->GetMouseButton(i))
				game_mouseButton(i);

		// Check mouse position
		if (p_input->GetPosX() != oldPosX || p_input->GetPosY() != oldposY)
		{
			game_mouseMove(p_input->GetPosX(), p_input->GetPosY());
			oldPosX = p_input->GetPosX();
			oldposY = p_input->GetPosY();
		}

		// check mouse motion
		if (deltaX != 0 || deltaY)
			game_mouseMotion(deltaX, deltaY);

		// check mouse Wheel
		int wheel = p_input->GetDeltaWheel();
		if (wheel != 0)
			game_mouseWheel(wheel);
	}

	void Engine::Shutdown() {
		gameover = true;
	}

	void Engine::Update() {

		// Calculate Core framerate
		p_frameCount_core++;
		if (p_coreTimer.stopwatch(999)) {
			p_frameRate_core = p_frameCount_core;
			p_frameCount_core = 0;
		}

		//fast update with no timing
		// Mutex lock to prevent thread from accessing unwanted data
		pthread_mutex_lock(&mutex);
		game_update();
		pthread_mutex_unlock(&mutex);

		// Update Entities
		pthread_mutex_lock(&mutex);
		if (!p_pauseMode)
			UpdateEntities();
		pthread_mutex_unlock(&mutex);

		// Perform global collision testing
		if (!p_pauseMode && collisionTimer.stopwatch(50))
		{
			pthread_mutex_lock(&mutex);
			TestForCollisions();
			pthread_mutex_unlock(&mutex);
		}
		//update with 60fps timing
		if (!timedUpdate.stopwatch(14)) {
			if (!this->getMaximizeProcessor()) {
				Sleep(1);
			}
		}
		else {
			// Calculate Real framerate
			p_frameCount_real++;
			if (p_realTimer.stopwatch(999)) {
				p_frameRate_real = p_frameCount_real;
				p_frameCount_real = 0;
			}

			// update input device
			p_input->Update();
			this->UpdateKeyboard();
			this->UpdateMouse();

			// update audio system
			audio->Update();

			// Begin Rendering
			this->RenderStart();

			// Allow game to render 3D
			game_render3d();

			// Render 3D entities
			pthread_mutex_lock(&mutex);
			if (!p_pauseMode)
				Draw3DEntities();
			pthread_mutex_unlock(&mutex);

			// set identity, return to origin
			//SetIdentity();
			// 2D Rendering
			Render2D_Start();
			
			pthread_mutex_lock(&mutex);
			if (!p_pauseMode)
				Draw2DEntities();
			pthread_mutex_unlock(&mutex);

			pthread_mutex_lock(&mutex);
			game_render2d();
			pthread_mutex_unlock(&mutex);

			// Render 2D entities
			Render2D_Stop();
			
			// Done Rendering
			this->RenderStop();

		}
		// Remove dead entities from the list
		//BuryEntities();
	}

	void Engine::Close() {
		game_end();
	}

	void Engine::UpdateEntities()
	{
		std::list<Entity*>::iterator iter;
		Entity* entity;
		iter = p_entities->begin();
		while (iter != p_entities->end())
		{
			// point local sprite to object in the list
			entity = *iter;

			// Is this entity Alive?
			if (entity->getAlive())
			{
				// move/animate entity
				entity->move();
				entity->animate();

				// tell game this entity has been updated
				game_entityUpdate(entity);

				// see if this entity will auto expire

				if (entity->getLifeTime() > 0)
				{
					if (entity->lifeTimeExpired()) {
						entity->setAlive(false);
					}
				}
			}
			++iter;
			if (gameover) break;
		}
	}

	void Engine::Draw3DEntities()
	{
		Entity* entity;
		std::list<Entity*>::iterator iter = p_entities->begin();
		while (iter != p_entities->end())
		{
			// Temporary pointer
			entity = *iter;

			// is this a 3D entity?
			if (entity->getRenderType() == Render3D)
			{
				// is this entity in use?
				if (entity->getAlive() && entity->getVisible())
				{
					entity->draw();
					game_entityRender(entity);
				}
			}
			++iter;
			if (gameover) break;
		}
	}

	void Engine::Draw2DEntities()
	{
		Entity* entity;
		std::list<Entity*>::iterator iter = p_entities->begin();
		while (iter != p_entities->end())
		{
			// temporary pointer
			entity = *iter;

			// is this a 2D entity?
			if (entity->getRenderType() == Render2D)
			{
				// is this entity in use?
				if (entity->getAlive() && entity->getVisible())
				{
					entity->draw();
					game_entityRender(entity);
				}
			}
			++iter;
			if (gameover) break;
		}

	}

	// MOVED TO	thread_function_bury_entities

// 	void Engine::BuryEntities()
// 	{
// 		std::list<Entity*>::iterator iter = p_entities->begin();
// 		while (iter != p_entities->end())
// 		{
// 			if ((*iter)->getAlive() == false) {
// 				iter = p_entities->erase(iter);
// 			}
// 			else
// 				iter++;
// 		}
// 	}

	void Engine::addEntity(Entity* entity)
	{
		static int id = 0;
		entity->setID(id);
		p_entities->push_back(entity);
		id++;
	}

	Entity* Engine::findEntity(int objectType)
	{
		std::list<Entity*>::iterator iter = p_entities->begin();
		while (iter != p_entities->end())
		{
			if ((*iter)->getAlive() == true && (*iter)->getObjectType() == objectType)
				return *iter;
			else
				++iter;
			if (gameover) break;
		}
		return NULL;
	}

	Entity* Engine::findEntity(std::string name)
	{
		std::list<Entity*>::iterator iter = p_entities->begin();
		while (iter != p_entities->end())
		{
			if ((*iter)->getAlive() == true && (*iter)->getName() == name)
				return *iter;
			else
				++iter;
			if (gameover) break;
		}
		return NULL;
	}

	int Engine::getEntityCount(int objectType)
	{
		int total = 0;
		std::list<Entity*>::iterator i = p_entities->begin();
		if (*i) {
			while (i != p_entities->end())
			{
				if ((*i)->getAlive() && (*i)->getObjectType() == objectType)
					total++;
				else
					++i;

				if (gameover) break;
			}
		}
		return total;
	}

	bool Engine::collision(Sprite* sprite1, Sprite* sprite2)
	{
		switch (sprite1->getCollisionMethod())
		{
		case COLLISION_RECT:
			return collisionBR(sprite1, sprite2);
			break;
		case COLLISION_DIST:
			return collisionD(sprite1, sprite2);
			break;
		case COLLISION_NONE:
		default:
			return false;
		}
	}

	bool Engine::collisionBR(Sprite* sprite1, Sprite* sprite2)
	{
		bool ret = false;

		Rect* ra = new Rect(
			sprite1->getX(),
			sprite1->getY(),
			sprite1->getX() + sprite1->getWidth() * sprite1->getScale(),
			sprite1->getY() + sprite1->getHeight() * sprite1->getScale());

		Rect* rb = new Rect(
			sprite2->getX(),
			sprite2->getY(),
			sprite2->getX() + sprite2->getWidth() * sprite2->getScale(),
			sprite2->getY() + sprite2->getHeight() * sprite2->getScale());

		//are any of sprite b's corners intersecting sprite a?
		if (ra->isInside(rb->getLeft(), rb->getTop()) ||
			ra->isInside(rb->getRight(), rb->getTop()) ||
			ra->isInside(rb->getLeft(), rb->getBottom()) ||
			ra->isInside(rb->getRight(), rb->getBottom()))
			ret = true;

		delete ra;
		delete rb;
		return ret;
	}

	bool Engine::collisionD(Sprite* sprite1, Sprite* sprite2)
	{
		double radius1, radius2;

		// calculate radius 1
		if (sprite1->getWidth() > sprite1->getHeight())
			radius1 = (sprite1->getWidth() * sprite1->getScale()) / 2;
		else
			radius1 = (sprite1->getHeight() * sprite1->getScale()) / 2;

		// point = center of sprite1
		double x1 = sprite1->getX() + radius1;
		double y1 = sprite1->getY() + radius1;
		Vector3 vector1(x1, y1, 0.0f);

		// calculate radius2
		if (sprite2->getWidth() > sprite2->getHeight())
			radius2 = (sprite2->getWidth() * sprite2->getScale()) / 2;
		else
			radius2 = (sprite2->getHeight() * sprite2->getScale()) / 2;

		// point = center of sprite2
		double x2 = sprite2->getX() + radius2;
		double y2 = sprite2->getY() + radius2;
		Vector3 vector2(x2, y2, 0.0f);

		// calculate distance
		double dist = vector1.Distance(vector2);

		// Return distance comparison
		return (dist < radius1 + radius2);
	}

	void Engine::TestForCollisions()
	{
		std::list<Entity*>::iterator first;
		std::list<Entity*>::iterator second;
		Sprite* sprite1;
		Sprite* sprite2;

		first = p_entities->begin();
		while (first != p_entities->end())
		{
			//we only care about sprite collisions
			if ((*first)->getRenderType() == Render2D)
			{
				//point local sprite to sprite contained in the list
				sprite1 = (Sprite*)*first;

				if (sprite1->getAlive() && sprite1->getVisible() && sprite1->isCollidable())
				{
					//test all other entities for collision
					second = p_entities->begin();
					while (second != p_entities->end())
					{
						//point local sprite to sprite contained in the list
						sprite2 = (Sprite*)*second;

						//if other entity is active and not same as first entity...
						if (sprite2->getAlive() && sprite2->getVisible() && sprite2->isCollidable() && sprite1 != sprite2)
						{
							//test for collision
							if (collision(sprite1, sprite2)) {

								//notify game of collision
								game_entityCollision(sprite1, sprite2);
							}

						}
						//go to the next sprite in the list
						second++;
					}

				}
				//go to the next sprite in the list
				first++;
			}//render2d
			if (gameover) break;
		} //while

	}

	void* thread_function_bury_entities(void* data)
	{
		static Timer timer;
		std::list<Entity*>::iterator iter;

		while (!gameover)
		{
			if (timer.stopwatch(2000))
			{
				pthread_mutex_lock(&g_engine->mutex);

				// Iterate through entity list
				iter = g_engine->getEntityList()->begin();
				while (iter != g_engine->getEntityList()->end())
				{
					if (*iter)
					{
						if ((*iter)->getAlive() == false)
						{
							delete (*iter);
							iter = g_engine->getEntityList()->erase(iter);
						}
						else
						{
							iter++;
							if (gameover) break;
						}
					}
				}
				if (gameover) break;
				pthread_mutex_unlock(&g_engine->mutex);
			}// if
		}// gameover

		pthread_exit(NULL);
		return NULL;
	}

};	// Namespace