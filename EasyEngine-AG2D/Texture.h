#include "EasyEngineAG2D.h"

#pragma once

namespace EE {
	class Texture
	{
	private:
		LPDIRECT3DTEXTURE9 texture;
		D3DXIMAGE_INFO info;
	public:
		Texture();
		Texture(const Texture& tx);
		virtual ~Texture();
		bool Load(const std::string& filename, D3DCOLOR transColor = D3DCOLOR_XRGB(255, 0, 255));
		void Release();
		LPDIRECT3DTEXTURE9 GetTexture() { return texture; }
		int getWidth();
		int getHeight();

	};
};


