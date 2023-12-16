#include "EasyEngineAG2D.h"
#pragma once


namespace EE {

	class Mesh : public Entity
	{
	private:
		LPD3DXMESH p_mesh;

		D3DMATERIAL9* p_materials;
		LPD3DXMATERIAL p_d3dxMaterials;
		LPD3DXBUFFER p_matbuffer;
		DWORD p_materialCount;

		LPDIRECT3DTEXTURE9* p_textures;

		D3DXVECTOR3 p_position;
		D3DXVECTOR3 p_velocity;
		D3DXVECTOR3 p_rotation;
		D3DXVECTOR3 p_scale;

		D3DXMATRIX p_materialWorld;
		D3DXMATRIX p_materialTranslate;
		D3DXMATRIX p_materialRotate;
		D3DXMATRIX p_materialScale;

	public:
		Mesh(void);
		virtual ~Mesh(void);

		int getFaceCount();
		int getVertexCount();

		void createSphere(float radius = 0.1f, int slices = 10, int stacks = 10);
		void createCube(float width = 1.0f, float height = 1.0f, float depth = 1.0f);

		bool Load(const char* filename);

		void Rotate(D3DXVECTOR3 rotation);
		void Rotate(float x, float y, float z);

		void SetPosition(const D3DXVECTOR3& aPosition);
		void SetPosition(float aPositionX, float aPositionY, float aPositionZ);
		D3DXVECTOR3 GetPosition() { return p_position; }

		void SetVelocity(const D3DXVECTOR3& aVelocity);
		void SetVelocity(float aVelocityX, float aVelocityY, float aVelocityZ);
		D3DXVECTOR3 GetVelocity() { return p_velocity; }

		void SetRotation(const D3DXVECTOR3& aRotation);
		void SetRotation(float aRotationX, float aRotationY, float aRotationZ);
		D3DXVECTOR3 GetRotation() { return p_rotation; }

		void SetScale(const D3DXVECTOR3& aScale);
		void SetScale(float aScaleX, float aScaleY, float aScaleZ);
		D3DXVECTOR3 GetScale() { return p_scale; }

		void LimitBoundary(float left, float right, float top, float bottom, float back, float front);

		void move();
		void animate();
		void draw();
		void Transform();
		void Update();

	};

}

