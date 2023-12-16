#include "EasyEngineAG2D.h"

namespace EE {
	Font::Font():Sprite()
	{
		// Set character widths to default
		memset(&widths, 0, sizeof(widths));
	}

	void Font::Print(int x, int y, std::string text, int color)
	{
		float fx = (float)x;
		float fy = (float)y;

		// Set font color
		this->setColor(color);

		// Draw each character of the string
		for (unsigned int i = 0; i < text.length(); i++)
		{
			int frame = (int)text[i];
			this->setCurrentFrame(frame);
			this->setX(fx);
			this->setY(fy);
			this->draw();

			// use proportional width if available
			if (widths[frame] == 0)widths[frame] = this->width;
			fx += widths[frame] * this->scaling;
		}
	}

	bool Font::loadWidthData(std::string filename)
	{
		unsigned char buffer[512];
		//open font width data file
		std::ifstream infile;
		infile.open(filename.c_str(), std::ios::binary);
		if (!infile) return false;

		// Read 512 byte. 2 byte per character
		infile.read((char*)(&buffer), 512);
		if (infile.bad()) return false;
		infile.close();

		// convert raw data to proportional width data
		for (int i = 0; i < 256; i++)
			widths[i] = (int)buffer[i * 2];


		return true;
	}
};
