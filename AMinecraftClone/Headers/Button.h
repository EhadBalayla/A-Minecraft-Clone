#pragma once
#include "Widget.h"
class Button : public Widget
{
public:
	Button(UIUVQuad offUV, UIUVQuad normalUV, UIUVQuad hoveredUV);

	void UpdateWidget() override;

	void(*Callback)();

protected:

	//the uvs for the different buttons wether they are pressed or not
	UIUVQuad m_OffUV;
	UIUVQuad m_NormalUV;
	UIUVQuad m_HoveredUV;

private:

	void UpdateButtonUVs(UIUVQuad uvs);


	bool IsHovered = false;
	bool FirstClick = false;
};

