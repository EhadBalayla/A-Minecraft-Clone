#pragma once
#include <string>

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();


	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline unsigned int getID() const { return m_TextureID; }
private:
	unsigned int m_TextureID;
	int m_Width, m_Height, m_Channels;
};

