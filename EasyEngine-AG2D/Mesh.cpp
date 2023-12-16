#include "EasyEngineAG2D.h"
namespace EE {

	Mesh::Mesh(void) : Entity(Render3D)
	{
		p_mesh = 0;
		p_materials = 0;
		p_d3dxMaterials = 0;
		p_matbuffer = 0;
		p_materialCount = 0;
		p_textures = 0;
		p_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		p_velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		p_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		p_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	Mesh::~Mesh(void)
	{
		if (p_materials != NULL)delete[]p_materials;

		// remove textures from memory
		if (p_textures != NULL)
		{
			for (DWORD i = 0; i < p_materialCount; i++) {
				if (p_textures[i] != NULL)
					p_textures[i]->Release();
			}
			delete[] p_textures;
		}

		if (p_mesh != NULL) p_mesh->Release();
	}

	int Mesh::getFaceCount() { return this->p_mesh->GetNumFaces(); }

	int Mesh::getVertexCount() { return this->p_mesh->GetNumVertices(); }

	bool Mesh::Load(const char* filename)
	{
		HRESULT result;

		// load mesh from specified filepath
		result = D3DXLoadMeshFromX(
			filename,				// filepath
			D3DXMESH_SYSTEMMEM,		// Mesh option
			g_engine->getDevice(),	// Direct3D device
			NULL,					// adjacency buffer
			&p_matbuffer,			// material buffer
			NULL,					// special effects buffer
			&p_materialCount,		// number of materials
			&p_mesh					// resulting mesh
			);

		if (result != D3D_OK)
			return false;

		// extract material properties and texture names from material buffer
		p_d3dxMaterials = (LPD3DXMATERIAL)p_matbuffer->GetBufferPointer();
		p_materials = new D3DMATERIAL9[p_materialCount];
		p_textures = new LPDIRECT3DTEXTURE9[p_materialCount];

		// create the materials and textures
		for (DWORD i = 0; i < p_materialCount; i++) {

			// grab the material
			p_materials[i] = p_d3dxMaterials[i].MatD3D;

			// set ambient color for material
			p_materials[i].Ambient = p_materials[i].Diffuse;
		//	p_materials[i].Emissive = p_materials[i].Diffuse;
			p_materials[i].Power = 0.5f;
		//	p_materials[i].Specular = p_materials[i].Diffuse;

			p_textures[i] = NULL;
			if (p_d3dxMaterials[i].pTextureFilename != NULL &&
				lstrlen(p_d3dxMaterials[i].pTextureFilename) > 0)
			{
				// load texture file specified in .x file
				result = D3DXCreateTextureFromFile(g_engine->getDevice(), p_d3dxMaterials[i].pTextureFilename, &p_textures[i]);

				if (result != D3D_OK)
					return false;
			}
		}
			// done using material buffer
		p_matbuffer->Release();

		return true;
	}

	void Mesh::createSphere(float radius, int slices, int stacks)
	{
		D3DXCreateSphere(g_engine->getDevice(), radius, slices, stacks, &p_mesh, NULL);
	}

	void Mesh::createCube(float width, float height, float depth)
	{
		D3DXCreateBox(g_engine->getDevice(), width, height, depth, &p_mesh, NULL);
	}

	void Mesh::draw()
	{
		Transform();
		if (p_materialCount == 0)
			p_mesh->DrawSubset(0);
		else
		{
			// Draw each mesh subset
			for (DWORD i = 0; i < p_materialCount; ++i)
			{
				// Set material and texture for this subset
				g_engine->getDevice()->SetMaterial(&p_materials[i]);

				if (p_textures[i])
				{
					if (p_textures[i]->GetType() == D3DRTYPE_TEXTURE)
					{
						D3DSURFACE_DESC desc;
						p_textures[i]->GetLevelDesc(0,&desc);
						if (desc.Width > 0)
							g_engine->getDevice()->SetTexture(0, p_textures[i]);
					}
				}

				// Draw mesh subset
				p_mesh->DrawSubset(i);
			}
		}
	}

	void Mesh::move()
	{
		Update();
		//Transform();
	}

	void Mesh::animate()
	{

	}

	void Mesh::Transform()
	{
		// Set rotation transform
		float x = D3DXToRadian(p_rotation.x);
		float y = D3DXToRadian(p_rotation.y);
		float z = D3DXToRadian(p_rotation.z);
		D3DXMatrixRotationYawPitchRoll(&p_materialRotate, x, y, z);

		// Set Scaling matrix
		D3DXMatrixScaling(&p_materialScale, p_scale.x, p_scale.y, p_scale.z);

		// Set Translation matrix
		D3DXMatrixTranslation(&p_materialTranslate, p_position.x, p_position.y, p_position.z);

		// Transform the mesh
		p_materialWorld = p_materialRotate * p_materialScale * p_materialTranslate;
		g_engine->getDevice()->SetTransform(D3DTS_WORLD, &p_materialWorld);
	}

	void Mesh::Rotate(D3DXVECTOR3 rotation)
	{
		Rotate(rotation.x, rotation.y, rotation.z);
	}

	void Mesh::Rotate(float x, float y, float z)
	{
		p_rotation.x += x;
		p_rotation.y += y;
		p_rotation.z += z;
	}

	void Mesh::SetPosition(const D3DXVECTOR3& aPosition)
	{
		p_position.x = aPosition.x;
		p_position.y = aPosition.y;
		p_position.z = aPosition.z;
	}

	void Mesh::SetPosition(float aPositionX, float aPositionY, float aPositionZ)
	{
		p_position.x = aPositionX;
		p_position.y = aPositionY;
		p_position.z = aPositionZ;
	}

	void Mesh::SetVelocity(const D3DXVECTOR3& aVelocity)
	{
		p_velocity = aVelocity;
	}

	void Mesh::SetVelocity(float aVelocityX, float aVelocityY, float aVelocityZ)
	{
		p_velocity.x = aVelocityX;
		p_velocity.y = aVelocityY;
		p_velocity.z = aVelocityZ;
	}

	void Mesh::SetRotation(const D3DXVECTOR3& aRotation)
	{
		p_rotation = aRotation;
	}

	void Mesh::SetRotation(float aRotationX, float aRotationY, float aRotationZ)
	{
		p_rotation.x = aRotationX;
		p_rotation.y = aRotationY;
		p_rotation.z = aRotationZ;
	}

	void Mesh::SetScale(const D3DXVECTOR3& aScale)
	{
		p_scale = aScale;
	}

	void Mesh::SetScale(float aScaleX, float aScaleY, float aScaleZ)
	{
		p_scale.x = aScaleX;
		p_scale.y = aScaleY;
		p_scale.z = aScaleZ;
	}

	void Mesh::Update()
	{
		p_position.x += p_velocity.x;
		p_position.y += p_velocity.y;
		p_position.z += p_velocity.z;
	}

	void Mesh::LimitBoundary(float left, float right, float top, float bottom, float back, float front)
	{
		if (p_position.x < left || p_position.x > right)
			p_velocity.x *= -1;
		if (p_position.y < bottom || p_position.y > top)
			p_velocity.y *= -1;
		if (p_position.z<front || p_position.z >back)
			p_velocity.z *= -1;
	}

}; // namespace