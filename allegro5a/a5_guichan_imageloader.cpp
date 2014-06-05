#include "a5_guichan_imageloader.h"
#include "a5_guichan_image.h"

#include <allegro5/allegro_image.h>

#include "guichan/exception.hpp"

using namespace gcn;

Image* Allegro5ImageLoader::load(const std::string& filename, bool convertToDisplayFormat)
{
	ALLEGRO_BITMAP *bmp = loadBitmap(filename);

	if (bmp == NULL)
	{
		throw GCN_EXCEPTION(std::string("Unable to load: ") + filename);
	}

	al_convert_mask_to_alpha(bmp, al_map_rgb(255, 0, 255));

	Image *image = new Allegro5Image(bmp, true);

	if (convertToDisplayFormat)
	{
		image->convertToDisplayFormat();
	}

	return image;
}

ALLEGRO_BITMAP* Allegro5ImageLoader::loadBitmap(const std::string& filename)
{
	return al_load_bitmap(filename.c_str());
}

