#include "EasyEngineAG2D.h"
#pragma once

namespace EE {

	class Font:public Sprite
	{
	private:
		int widths[256];
	public:
		Font();
		virtual ~Font(void) {}
		void Print(int x, int y, std::string text, int color = 0xFFFFFFFF);
		int getCharWidth() { return this->width; }
		int getCharHeight() { return this->height; }
		void setCharWidth(int width) { this->width = width; }
		void setCharHeight(int height) { this->height = height; }
		void setCharSize(int width, int height)
		{
			setCharWidth(width);
			setCharHeight(height);
		}
		bool loadWidthData(std::string filename);
	};// Class

}; // namespace

