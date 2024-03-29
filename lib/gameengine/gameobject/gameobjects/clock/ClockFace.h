#ifndef CLOCKFACE_H_
#define CLOCKFACE_H_

#include "../../DynamicGameObject.h"

class ClockFace : public DynamicGameObject
{
public:
	ClockFace(SpriteSheet* spriteSheet, SpriteSheet* font, Renderer* renderer, int x, int y);
	~ClockFace();

	// game object update logic goes here - eg.: move game object
	void handleEvents(BitFlag* events) override;
	void update(int frameTime) override;
	void update(int frameTime, int hours, int minutes);
	void render() override;
	virtual int getWidth() override
	{
		return _spriteSheet->spriteWidth() * 3;
	}
	virtual int getHeight() override
	{
		return _spriteSheet->spriteHeight() * 3;
	}

private:
	void _renderBackground(int x, int y);
	void _renderDigit(int x, int y, int digit);
	SpriteSheet* _font;
	int _digitHours = 0;
	int _digitMinutes = 0;
	bool _bumpEvent = false;
	float _yInternal = 0;
};

#endif