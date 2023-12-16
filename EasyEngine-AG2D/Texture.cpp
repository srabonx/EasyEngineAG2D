#include "EasyEngineAG2D.h"

namespace EE {

	Texture::Texture()
	{
		texture = NULL;
		ZeroMemory(&info, sizeof(info));
	}

	Texture::Texture(const Texture& tx)
	{
		*this = tx;
	}
	Texture::~Texture()
	{
		Release();
	}
	bool Texture::Load(const std::string& filename, D3DCOLOR transColor)
	{
		const char* filePath = filename.c_str();
		// Get Width and height from bitmap file
		HRESULT result;
		result = D3DXGetImageInfoFromFile(filePath, &info);
		if (FAILED(result))
			return false;

		// Create the new texture by loading a bitmap image file
		result = D3DXCreateTextureFromFileEx(
			g_engine->getDevice(),							// Direct3D device object
			filePath,										// bitmap filename
			info.Width,										// bitmap image width
			info.Height,									// bitmap image height
			1,												// mip-map levels (1 for no chain)
			D3DPOOL_DEFAULT,								// the type of surface (standard)
			D3DFMT_UNKNOWN,									// surface format (unknown)
			D3DPOOL_DEFAULT,								// memory class for the texture
			D3DX_DEFAULT,									// image filter
			D3DX_DEFAULT,									// mip filter
			transColor,										// color key for transparency
			&info,											// bitmap file info. (from loaded file)
			NULL,											// color palette
			&texture										// destination texture
		);

		if (FAILED(result))
			return false;

		return true;
	}
	int Texture::getWidth()
	{
		return info.Width;
	}
	int Texture::getHeight()
	{
		return info.Height;
	}

	void Texture::Release()
	{
		if (texture != NULL)
			texture->Release();
	}
};