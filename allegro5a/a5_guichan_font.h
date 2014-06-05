#ifndef ALLEGRO5_GUICHAN_FONT
#define ALLEGRO5_GUICHAN_FONT

#include <allegro5/allegro5.h>

#include "guichan/font.hpp"
#include "guichan/platform.hpp"

struct ALLEGRO_FONT;

class Allegro5Font : public gcn::Font
{
public:
	Allegro5Font(ALLEGRO_FONT* font);
	Allegro5Font(const std::string& filename, int size);

	~Allegro5Font();

	// Inherited from Font

	virtual int getWidth(const std::string& text) const;

	virtual int getHeight() const;

	virtual void drawString(gcn::Graphics* graphics, const std::string& text, int x, int y);
protected:
	bool mAutoFree;
	ALLEGRO_FONT* mAllegroFont;
};


#endif
