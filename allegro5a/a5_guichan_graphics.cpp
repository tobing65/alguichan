#include "a5_guichan_graphics.h"
#include "a5_guichan_image.h"

#include "guichan/rectangle.hpp"
#include "guichan/exception.hpp"
#include "guichan/cliprectangle.hpp"
#include "guichan/color.hpp"

#include <allegro5/allegro_primitives.h>
#include <math.h>

#ifdef ALLEGRO_MSVC
   #define hypotf(x, y) _hypotf((x), (y))
#endif

using namespace gcn;

Allegro5Graphics::Allegro5Graphics()
{
	mTarget = al_get_backbuffer(al_get_current_display());
}

Allegro5Graphics::Allegro5Graphics(ALLEGRO_BITMAP *target)
{
	mTarget = target;
}

Allegro5Graphics::~Allegro5Graphics()
{

}

void Allegro5Graphics::setTarget(ALLEGRO_BITMAP *target)
{
	
}

ALLEGRO_BITMAP *Allegro5Graphics::getTarget()
{
	return mTarget;
}

void Allegro5Graphics::_beginDraw()
{
	if(mTarget == NULL)
	{
		throw GCN_EXCEPTION("Target BITMAP is null, set it with setTarget first.");
	}

	//push a clip area the size of the target bitmap
	pushClipArea(Rectangle(0, 0, al_get_bitmap_width(mTarget), al_get_bitmap_height(mTarget)));
}

void Allegro5Graphics::_endDraw()
{
	//pop the clip area pushed in _beginDraw
	popClipArea();
}

bool Allegro5Graphics::pushClipArea(Rectangle area)
{
	bool result = Graphics::pushClipArea(area);

	const ClipRectangle& cr = mClipStack.top();

	ALLEGRO_BITMAP* old_target = al_get_target_bitmap();
	al_set_target_bitmap(mTarget);
	al_set_clipping_rectangle(cr.x, cr.y, cr.width, cr.height);
	al_set_target_bitmap(old_target);

	return result;
}

void Allegro5Graphics::popClipArea()
{
	Graphics::popClipArea();

	if (mClipStack.empty())
	{
		return;
	}

	const ClipRectangle& cr = mClipStack.top();

	ALLEGRO_BITMAP* old_target = al_get_target_bitmap();
	al_set_target_bitmap(mTarget);
	al_set_clipping_rectangle(cr.x, cr.y, cr.width, cr.height);
	al_set_target_bitmap(old_target);
}

void Allegro5Graphics::drawImage(const Image* image,
						int srcX,
						int srcY,
						int dstX,
						int dstY,
						int width,
						int height)
{
	if (mClipStack.empty())
	{
		throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion "
						  "outside of _beginDraw() and _endDraw()?");
	}

	const int xOffset = mClipStack.top().xOffset;
	const int yOffset = mClipStack.top().yOffset;

	const Allegro5Image* srcImage = dynamic_cast<const Allegro5Image*>(image);

	if (srcImage == NULL)
	{
		throw GCN_EXCEPTION("Trying to draw an image of unknown format, must be an Allegro5Image.");
	}

	al_draw_bitmap_region(srcImage->getBitmap(),
	   srcX,
	   srcY,
	   width,
	   height,
	   dstX + xOffset,
	   dstY + yOffset,
	   0
	);
}

void Allegro5Graphics::drawPoint(int x, int y)
{
	if (mClipStack.empty())
	{
		throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion "
						  "outside of _beginDraw() and _endDraw()?");
	}

	const int xOffset = mClipStack.top().xOffset;
	const int yOffset = mClipStack.top().yOffset;

	al_draw_pixel(x + xOffset + 0.5f, y + yOffset + 0.5f, mAllegroColor);
}

//A function to draw a line with A4 semantics, which is what GuiChan expects
void a4_line(float x1, float y1, float x2, float y2, ALLEGRO_COLOR color)
{
	ALLEGRO_VERTEX vtx[4];

	float len = hypotf(x2 - x1, y2 - y1);

	if (len == 0)
	{
		al_draw_pixel(x1 + 0.5f, y1 + 0.5f, color);
		return;
	}

	float tx = 0.5f * (y2 - y1) / len;
	float ty = 0.5f * -(x2 - x1) / len;
	float lx = 0.5f * (x2 - x1) / len;
	float ly = 0.5f * (y2 - y1) / len;
	   
	vtx[0].x = 0.5f + x1 + tx - lx; vtx[0].y = 0.5f + y1 + ty - ly;
	vtx[1].x = 0.5f + x1 - tx - lx; vtx[1].y = 0.5f + y1 - ty - ly;
	vtx[2].x = 0.5f + x2 - tx + lx; vtx[2].y = 0.5f + y2 - ty + ly;
	vtx[3].x = 0.5f + x2 + tx + lx; vtx[3].y = 0.5f + y2 + ty + ly;

	for (int ii = 0; ii < 4; ii++)
	{
		vtx[ii].color = color;
		vtx[ii].z = 0;
	}

	al_draw_prim(vtx, 0, 0, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
}

void Allegro5Graphics::drawLine(int x1, int y1, int x2, int y2)
{
	if (mClipStack.empty())
	{
		throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion "
						  "outside of _beginDraw() and _endDraw()?");
	}

	const int xOffset = mClipStack.top().xOffset;
	const int yOffset = mClipStack.top().yOffset;

	a4_line(x1 + xOffset, y1 + yOffset, x2 + xOffset, y2 + yOffset, mAllegroColor);
}

void Allegro5Graphics::drawRectangle(const Rectangle& rectangle)
{
	if (mClipStack.empty())
	{
		throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion " 
						  "outside of _beginDraw() and _endDraw()?");
	}

	const int xOffset = mClipStack.top().xOffset;
	const int yOffset = mClipStack.top().yOffset;

	al_draw_rectangle(
	   float(rectangle.x + xOffset) + 0.5,
	   float(rectangle.y + yOffset) + 0.5,
	   float(rectangle.x + rectangle.width - 1 + xOffset) + 0.5,
	   float(rectangle.y + rectangle.height - 1 + yOffset) + 0.5,
	   mAllegroColor, 1);
}

void Allegro5Graphics::fillRectangle(const Rectangle& rectangle)
{
	if (mClipStack.empty())
	{
		throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion " 
						  "outside of _beginDraw() and _endDraw()?");
	}

	const int xOffset = mClipStack.top().xOffset;
	const int yOffset = mClipStack.top().yOffset;

	al_draw_filled_rectangle(
	   rectangle.x + xOffset,
	   rectangle.y + yOffset,
	   rectangle.x + rectangle.width + xOffset,
	   rectangle.y + rectangle.height + yOffset,
	   mAllegroColor);
}

void Allegro5Graphics::setColor(const Color& color)
{
	mColor = color;
	mAllegroColor = al_map_rgba(color.r, color.g, color.b, color.a);
}

const Color& Allegro5Graphics::getColor() const
{
	return mColor;
}

const ALLEGRO_COLOR& Allegro5Graphics::getAllegroColor() const
{
	return mAllegroColor;
}

void Allegro5Graphics::drawBitmap(ALLEGRO_BITMAP* bitmap, float dstX, float dstY)
{
	const int xOffset = mClipStack.top().xOffset;
	const int yOffset = mClipStack.top().yOffset;

	al_draw_bitmap(bitmap, dstX + xOffset, dstY + yOffset, 0);
}
