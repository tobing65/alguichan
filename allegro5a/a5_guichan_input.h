#ifndef ALLEGRO5_GUICHAN_FONT
#define ALLEGRO5_GUICHAN_FONT

#include "guichan/input.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/mouseinput.hpp"

struct ALLEGRO_EVENT_QUEUE;

class Allegro5Input : public gcn::Input
{
public:
	Allegro5Input();
	virtual ~Allegro5Input();


	// Inherited from Input

	virtual bool isKeyQueueEmpty();

	virtual gcn::KeyInput dequeueKeyInput();

	virtual bool isMouseQueueEmpty();

	virtual gcn::MouseInput dequeueMouseInput();

	virtual void _pollInput() { };

protected:
	gcn::Key convertToKey(int keycode, int unicode);
	bool isNumericPad(int keycode);

	ALLEGRO_EVENT_QUEUE* mKeyQueue;
	ALLEGRO_EVENT_QUEUE* mMouseQueue;
};


#endif
