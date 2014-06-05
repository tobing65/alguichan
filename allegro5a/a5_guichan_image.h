#ifndef ALLEGRO5_GUICHAN_IMAGE
#define ALLEGRO5_GUICHAN_IMAGE

#include <guichan/color.hpp>
#include <guichan/image.hpp>

#include <allegro5/allegro5.h>

class Allegro5Image : public gcn::Image
{
public:
	Allegro5Image(ALLEGRO_BITMAP* bitmap, bool autoFree);

	virtual ~Allegro5Image();
	virtual ALLEGRO_BITMAP* getBitmap() const;

	// Inherited from Image

	virtual void free();

	virtual int getWidth() const;

	virtual int getHeight() const;

	virtual gcn::Color getPixel(int x, int y);

	virtual void putPixel(int x, int y, const gcn::Color& color);

	virtual void convertToDisplayFormat();

protected:
	ALLEGRO_BITMAP* mBitmap;
	bool mAutoFree;
};

#endif
