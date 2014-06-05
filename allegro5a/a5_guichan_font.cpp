#include "a5_guichan_font.h"
#include "a5_guichan_graphics.h"

#include <allegro5/allegro_font.h>

#include "guichan/exception.hpp"

using namespace gcn;

Allegro5Font::Allegro5Font(ALLEGRO_FONT* font)
		: mAutoFree(false),
		  mAllegroFont(font)
{
	if (font == NULL)
	{
		throw GCN_EXCEPTION("Allegro font is not usable. Have you forgotten to load it?");
	}
}

Allegro5Font::Allegro5Font(const std::string& filename, int size)
		: mAutoFree(true),
		  mAllegroFont(al_load_font(filename.c_str(), size, 0))
{
	if (mAllegroFont == NULL)
	{
		throw GCN_EXCEPTION("Unable to load Allegro font from file.");
	}
}

Allegro5Font::~Allegro5Font()
{
	if (mAutoFree)
	{
		if (mAllegroFont != NULL)
		{
			al_destroy_font(mAllegroFont);
		}

		mAllegroFont = NULL;
	}
}

int Allegro5Font::getWidth(const std::string& text) const
{
	return al_get_text_width(mAllegroFont, text.c_str());
}

int Allegro5Font::getHeight() const
{
	return al_get_font_line_height(mAllegroFont);
}

void Allegro5Font::drawString(gcn::Graphics* graphics, const std::string& text, int x, int y)
{
	Allegro5Graphics* const allegroGraphics = dynamic_cast<Allegro5Graphics*>(graphics);
	if (!allegroGraphics)
	{
		throw GCN_EXCEPTION("Graphics is not of type AllegroGraphics");
	}
	
	const gcn::ClipRectangle& rec = graphics->getCurrentClipArea();
	
	al_draw_text(mAllegroFont, allegroGraphics->getAllegroColor(), float(x + rec.x), float(y + rec.y), 0, text.c_str());
}
