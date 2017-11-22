#pragma once
#include <memory>
#include <Axilya/AXMain.h>
class Text{
public:
	Text(std::shared_ptr<AXFont> font) : m_font (font){}
	void SetText(const std::string& str);
	void SetText(const std::string& str, AXColour& colour);

	int getWidth() { if(m_texture){return m_texture->getWidth();} return 0;}
	int getHeight() { if(m_texture){return m_texture->getHeight();} return 0;}
	const AXTexture* GetTexture() {return m_texture.get();}
private:
	std::shared_ptr<AXFont> m_font = nullptr;
	std::unique_ptr<AXTexture> m_texture;
	std::string m_string;
};