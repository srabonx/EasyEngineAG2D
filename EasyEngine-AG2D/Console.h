#include "EasyEngineAG2D.h"
#pragma once

namespace EE {
	class Console
	{
	private:
		bool showing;
		Sprite* panel;
		Font* font;
		int currentLine;
		std::vector<std::string> textLines;
		std::vector < std::string > ::iterator iter;

	public:
		Console();
		virtual ~Console();
		bool init();
		void draw();
		void clear();
		void print(std::string text, int line = -1);
		
		bool isShowing() { return showing; }
		void show() { showing = true; }
		void hide() { showing = false; }
		void setShowing(bool value) { showing = value; }
	};
}

