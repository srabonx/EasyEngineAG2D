// Easy Engine - AdvanceGameEngine2D 
// Main Header File
// Author - Ahmed Srabon		Date: 29-Nov-2023

#ifndef _EASYENGINEAG2D_H
#define _EASYENGINEAG2D_H 1

#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <dinput.h>
#include <fmod.h>
#include <vector>
#include <math.h>
#include <list>
#include <fstream>
#include <sstream>
#include <pthread.h>
//#include <dxerr9.h>
#include "Timer.h"
#include "Camera.h"
#include "Entity.h"
#include "Mesh.h"
#include "Light.h"
#include "Texture.h"
#include "Vector3.h"
#include "Sprite.h"
#include "ParticleEmitter.h"
#include "Input.h"
#include "Audio.h"
#include "Font.h"
#include "Rect.h"
#include "Console.h"
#include "Math.h"
#include "lua.hpp"
#include "Script.h"




#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define REVISION 0

//External variable and functions

extern bool gameover;
extern bool game_preload();
extern bool game_init(HWND);
extern void game_update();
extern void game_end();
extern void game_render3d();
extern void game_render2d();

extern void game_entityUpdate(EE::Entity*);
extern void game_entityRender(EE::Entity*);

// Events
extern void game_keyPress(int key);
extern void game_keyRelease(int key);
extern void game_mouseButton(int button);
extern void game_mouseMotion(int x, int y);
extern void game_mouseMove(int x, int y);
extern void game_mouseWheel(int wheel);

extern void game_entityCollision(EE::Entity*, EE::Entity*);

namespace EE {

	//*** THREAD STUFF
	// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	void* thread_function_bury_entities(void* data);

	class Engine {
	private:
		int p_versionMajor, p_versionMinor, p_revision;
		HWND p_windowHandle;
		LPDIRECT3D9 p_d3d;
		LPDIRECT3DDEVICE9 p_device;
		LPDIRECT3DSURFACE9 p_backbuffer;
		LPD3DXSPRITE p_sprite_handler;
		std::string p_apptitle;
		bool p_fullscreen;
		int p_screenwidth;
		int p_screenheight;
		int p_colordepth;
		bool p_pauseMode;
		D3DCOLOR p_ambientColor;
		bool p_maximizeProcessor;
		Timer timedUpdate;
		Timer p_coreTimer;
		long p_frameCount_core;
		long p_frameRate_core;
		Timer p_realTimer;
		long p_frameCount_real;
		long p_frameRate_real;
		Timer collisionTimer;
		
		//class objects
		Input* p_input;
		std::list<Entity*>* p_entities;

	private:
		void UpdateEntities();
		void Draw3DEntities();
		void Draw2DEntities();
		void BuryEntities();

		bool collisionBR(Sprite* sprite1, Sprite* sprite2);
		bool collisionD(Sprite* sprite1, Sprite* sprite2);
		void TestForCollisions();

	public:
		Engine();
		virtual ~Engine();
		int Init(int width, int height, int colordepth, bool fullscreen);
		void Close();
		void Update();
		void message(std::string message, std::string title = "Easy Engine");
		void fatalerror(std::string message, std::string title = "FATAL ERROR");
		void Shutdown();
		void ClearScene(D3DCOLOR color);
		void SetDefaultMaterial();
		void SetAmbient(D3DCOLOR colorvalue);
		void SetIdentity();
		int RenderStart();
		int RenderStop();
		int Render2D_Start();
		int Render2D_Stop();
		int Release();
		void UpdateKeyboard();
		void UpdateMouse();
		bool collision(Sprite* sprite1, Sprite* sprite2);

//		Accessor/mutator functions expose the private variables

		bool isPaused() { return this->p_pauseMode; }
		void setPaused(bool value) { this->p_pauseMode = value; }
		LPDIRECT3DDEVICE9 getDevice() { return this->p_device; }
		LPDIRECT3DSURFACE9 getBackBuffer() { return this->p_backbuffer; }
		LPD3DXSPRITE getSpriteHandler() { return this -> p_sprite_handler; }
		void setWindowHandle(HWND hwnd) { this->p_windowHandle = hwnd; }
		HWND getWindowHandle() { return this->p_windowHandle; }
		std::string getAppTitle() { return this->p_apptitle; }
		void setAppTitle(std::string value) { this->p_apptitle = value; }
		int getVersionMajor() { return this->p_versionMajor; }
		int getVersionMinor() { return this->p_versionMinor; }
		int getRevision() { return this->p_revision; }
		std::string getVersionText();
		long getFrameRateCore() { return this->p_frameRate_core; }
		long getFrameRateReal() { return this->p_frameRate_real; }
		int getScreenWidth() { return this->p_screenwidth; }
		void setScreenWidth(int value) { this->p_screenwidth = value; }
		int getScreenHeight() { return this->p_screenheight; }
		void setScreenHeight(int value) { this->p_screenheight = value; }
		int getColorDepth() { return this->p_colordepth; }
		void setColorDepth(int value) { this->p_colordepth = value; }
		bool getFullScreen() { return this->p_fullscreen; }
		void setFullScreen(bool value) { this->p_fullscreen = value; }
		bool getMaximizeProcessor() { return this->p_maximizeProcessor; }
		void setMaximizeProcessor(bool value) { this->p_maximizeProcessor = value; }
		

						// PUBLIC OBJECTS
		// Simplified public Audio object
		Audio* audio;

		// Math object
		Math* math;

		// Entity List on Head
		std::list<Entity*>* getEntityList() { return p_entities; }
		long getEntityCount() { return (long)p_entities->size(); }
		void addEntity(Entity* entity);
		Entity* findEntity(int objectType);
		Entity* findEntity(std::string name);
		int getEntityCount(int objectType);

		// Thread Mutex.. expose to public to simplify code
		pthread_mutex_t mutex;


	}; //class
}; //namespace

// Global Engine object. (visible everywhere!)

extern EE::Engine* g_engine;


#endif
