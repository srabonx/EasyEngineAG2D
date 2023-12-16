#include "EasyEngineAG2D.h"

namespace EE
{
	Console::Console()
	{
		showing = false;
		currentLine = 0;
		clear();
	}

	Console::~Console()
	{
		delete font;
		delete panel;
	}

	bool Console::init()
	{
		// Load game panel image
		panel = new Sprite();
		if (!panel->loadImage("panel.tga"))return false;
		double scale = g_engine->getScreenWidth() / 640.0f;
		panel->setScale(scale);
		panel->setColor(0x99FFFFFF);

		// Load the font
		font = new Font();
		if (!font->loadImage("system12.tga")) return false;
		font->setColumns(16);
		font->setCharSize(14, 16);
		if (!font->loadWidthData("system12.dat")) return false;

		return true;
	}

	void Console::draw()
	{
		int x = 5, y = 0;

		// ** bug fix
		if (!showing) return;
		
		// Draw panel background
		panel->draw();

		//*** Modification
		// Draw Text lines
		for (unsigned int i = 0; i < textLines.size(); i++)
		{
			font->Print(x, y * 14, textLines[i], 0xFF000000);
			y += 1;

			if (y > 26)
			{
				if(x>10) break;
				x = g_engine->getScreenWidth() / 2 + 5;
				y = 0;
			}
		}

	}

	void Console::clear()
	{
		for (int i = 0; i < 55; i++)
			textLines.push_back("");
	}

	void Console::print(std::string text, int line)
	{
		if (line > -1)currentLine = line;
		textLines[currentLine] = text;
		if (currentLine++ > 52) currentLine = 0;
	}

};
