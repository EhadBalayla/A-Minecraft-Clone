#pragma once
#include "Widget.h"

class Texture;
class ImageDisplayer : public Widget {
public:

	Texture* tex = nullptr;
	UIUVQuad uv;

	void Render();
};