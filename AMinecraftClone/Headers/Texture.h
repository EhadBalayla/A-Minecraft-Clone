#pragma once

class Texture
{
public:
	void LoadTexture(const char* path);
	void UnloadTexture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline unsigned int getID() const { return m_TextureID; }
private:
	unsigned int m_TextureID;
	int m_Width, m_Height, m_Channels;
};