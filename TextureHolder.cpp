#include "TextureHolder.h"
#include "assert.h"

using namespace sf;
using namespace std;

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

sf::Texture& TextureHolder::GetTexture(std::string const& fileName) {
	// Get a reference to m_Textures using static instance
	auto& m = m_s_Instance->m_Textures;

	// Try and find the texture
	auto keyValuePair = m.find(fileName);


	if (keyValuePair != m.end()) {
		// Found texture return it
		return keyValuePair->second;
	}
	else
	{
		// File name not found
		// Create a new file
		auto& texture = m[fileName];

		texture.loadFromFile(fileName);

		return texture;
	}
}