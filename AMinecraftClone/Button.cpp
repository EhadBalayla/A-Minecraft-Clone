#include "Button.h"


Button::Button(UIUVQuad offUV, UIUVQuad normalUV, UIUVQuad hoveredUV) : Widget(normalUV){
	m_OffUV = offUV;
	m_NormalUV = normalUV;
	m_HoveredUV = hoveredUV;


}

void Button::UpdateWidget() {
	
}