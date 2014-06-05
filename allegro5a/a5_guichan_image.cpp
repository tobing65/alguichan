#include "a5_guichan_image.h"
#include "guichan/exception.hpp"

using namespace gcn;

Allegro5Image::Allegro5Image(ALLEGRO_BITMAP* bitmap, bool autoFree)
{
	mAutoFree = autoFree;
	mBitmap = bitmap;
}

Allegro5Image::~Allegro5Image()
{
	if (mAutoFree)
	{
		free();
	}
}

ALLEGRO_BITMAP* Allegro5Image::getBitmap() const
{
	return mBitmap;
}

void Allegro5Image::free()
{
	al_destroy_bitmap(mBitmap);
}

int Allegro5Image::getWidth() const
{
	if (mBitmap == NULL)
	{
		throw GCN_EXCEPTION("Trying to get the width of a non loaded image.");
	}

	return al_get_bitmap_width(mBitmap);
}

int Allegro5Image::getHeight() const
{
	if (mBitmap == NULL)
	{
		GCN_EXCEPTION("Trying to get the height of a non loaded image.");
	}

	return al_get_bitmap_height(mBitmap);
}

Color Allegro5Image::getPixel(int x, int y)
{
	if (mBitmap == NULL)
	{
		throw GCN_EXCEPTION("Trying to get a pixel from a non loaded image.");
	}

	ALLEGRO_COLOR c = al_get_pixel(mBitmap, x, y);
	
	unsigned char r,g,b,a;
	al_unmap_rgba(c, &r, &g, &b, &a);

	return Color(r, g, b, a);
}

void Allegro5Image::putPixel(int x, int y, const Color& color)
{
	if (mBitmap == NULL)
	{
		throw GCN_EXCEPTION("Trying to put a pixel in a non loaded image.");
	}

	ALLEGRO_COLOR c = al_map_rgba(color.r, color.g, color.b, color.a);
	
	ALLEGRO_BITMAP* old_target = al_get_target_bitmap();
	al_set_target_bitmap(mBitmap);
	al_put_pixel(x, y, c);
	al_set_target_bitmap(old_target);
}

void Allegro5Image::convertToDisplayFormat()
{
	if (mBitmap == NULL)
	{
		GCN_EXCEPTION("Trying to convert a non loaded image to display format.");
	}

	int old_flags = al_get_new_bitmap_flags();
	int old_format = al_get_new_bitmap_format();

	al_set_new_bitmap_flags(0);
	al_set_new_bitmap_format(0);

	ALLEGRO_BITMAP *bmp = al_clone_bitmap(mBitmap);

	al_set_new_bitmap_flags(old_flags);
	al_set_new_bitmap_format(old_format);

	al_destroy_bitmap(mBitmap);
	mBitmap = bmp;
}
