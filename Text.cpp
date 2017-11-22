#include "Text.hpp"

void Text::SetText(const std::string& str)
{
	static AXColour black(0, 0, 0);
	SetText(str, black);
}

void Text::SetText(const std::string& str, AXColour& colour)
{
	if(str != m_string)
	{
		m_string = str;
		m_texture.reset(m_font->bakeTexture(m_string, colour));
	}
}