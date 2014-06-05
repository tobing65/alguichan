#ifndef ALLEGRO5_GUICHAN_GRAPHICS
#define ALLEGRO5_GUICHAN_GRAPHICS

#include "guichan/color.hpp"
#include "guichan/graphics.hpp"

#include <allegro5/allegro5.h>

class Allegro5Graphics : public gcn::Graphics
{
public:
	using Graphics::drawImage;

	Allegro5Graphics();

	Allegro5Graphics(ALLEGRO_BITMAP *target);

	virtual ~Allegro5Graphics();
	virtual void setTarget(ALLEGRO_BITMAP *target);

	virtual ALLEGRO_BITMAP *getTarget();

	const ALLEGRO_COLOR& getAllegroColor() const;

	virtual void drawBitmap(ALLEGRO_BITMAP* bitmap, float dstX, float dstY);

	// Inherited from Graphics

	virtual void _beginDraw();

	virtual void _endDraw();

	virtual bool pushClipArea(gcn::Rectangle area);

	virtual void popClipArea();

	virtual void drawImage(const gcn::Image* image,
								 int srcX,
								 int srcY,
								 int dstX,
								 int dstY,
								 int width,
								 int height);

	virtual void drawPoint(int x, int y);

	virtual void drawLine(int x1, int y1, int x2, int y2);

	virtual void drawRectangle(const gcn::Rectangle& rectangle);

	virtual void fillRectangle(const gcn::Rectangle& rectangle);

	virtual void setColor(const gcn::Color& color);

	virtual const gcn::Color& getColor() const;
protected:
	ALLEGRO_BITMAP *mTarget;
	ALLEGRO_COLOR mAllegroColor;
	gcn::Color mColor;
};

#endif
