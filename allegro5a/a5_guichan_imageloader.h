#ifndef ALLEGRO5_GUICHAN_IMAGELOADER
#define ALLEGRO5_GUICHAN_IMAGELOADER

#include <allegro5/allegro5.h>

#include "guichan/image.hpp"
#include "guichan/imageloader.hpp"

class Allegro5ImageLoader : public gcn::ImageLoader
{
public:
	// Inherited from ImageLoader
	virtual gcn::Image* load(const std::string& filename, bool convertToDisplayFormat = true);
protected:
	virtual ALLEGRO_BITMAP* loadBitmap(const std::string& filename);
};

#endif
