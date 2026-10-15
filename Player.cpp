#include "player.h"

Player::Player(){
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	// Associate a texture with player sprite
	m_Texture.loadFromFile("graphics\player.png");
	m_Sprite.setTexture(m_Texture);
}