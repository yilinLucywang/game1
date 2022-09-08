#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>


//Lucy's include begin
//Yilin's change 
#include <list>
#include <cmath>
#include <random>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cassert>

PlayMode::PlayMode() {
	//TODO:
	// you *must* use an asset pipeline of some sort to generate tiles.
	// don't hardcode them like this!
	// or, at least, if you do hardcode them like this,
	//  make yourself a script that spits out the code that you paste in here
	//   and check that script into your repository.

	//Also, *don't* use these tiles in your game:

	{ //use tiles 0-16 as some weird dot pattern thing:
		std::array< uint8_t, 8*8 > distance;
		for (uint32_t y = 0; y < 8; ++y) {
			for (uint32_t x = 0; x < 8; ++x) {
				float d = glm::length(glm::vec2((x + 0.5f) - 4.0f, (y + 0.5f) - 4.0f));
				d /= glm::length(glm::vec2(4.0f, 4.0f));
				distance[x+8*y] = uint8_t(std::max(0,std::min(255,int32_t( 255.0f * d ))));
			}
		}
		for (uint32_t index = 0; index < 16; ++index) {
			PPU466::Tile tile;
			uint8_t t = uint8_t((255 * index) / 16);
			for (uint32_t y = 0; y < 8; ++y) {
				uint8_t bit0 = 0;
				uint8_t bit1 = 0;
				for (uint32_t x = 0; x < 8; ++x) {
					uint8_t d = distance[x+8*y];
					if (d > t) {
						bit0 |= (1 << x);
					} else {
						bit1 |= (1 << x);
					}
				}
				tile.bit0[y] = bit0;
				tile.bit1[y] = bit1;
			}
			ppu.tile_table[index] = tile;
		}
	}

	std::string line;
	std::ifstream infile("thefile.txt");
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::cout << "printing?"; 
		std::cout << line;
	}

	//rocket
	ppu.tile_table[32].bit0 = {
		0b01100110,
		0b11111111,
		0b01111110,
		0b00011000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
	};
	ppu.tile_table[32].bit1 = {
		0b01100110,
		0b11011011,
		0b01011010,
		0b00100100,
		0b01111110,
		0b01111110,
		0b00111100,
		0b00011000,
	};
	//asteroid
	ppu.tile_table[33].bit0 = {
		0b00001000,
		0b00011000,
		0b01011010,
		0b11101101,
		0b01010110,
		0b00111010,
		0b00101100,
		0b00010000,
	};
	ppu.tile_table[33].bit1 = {
		0b11100111,
		0b11000011,
		0b10000001,
		0b00000000,
		0b00000000,
		0b10000001,
		0b11000011,
		0b11100111,
	};

	//rocket palette
	ppu.palette_table[6] = {
		glm::u8vec4(0x00,0x00,0x00,0x00),
		glm::u8vec4(0xf8,0x83,0x09,0xff),
		glm::u8vec4(0xf7,0xf1,0xf1,0xff),
		glm::u8vec4(0xf8,0x4e,0x09,0xff),
	};
	//asteroid palette
	ppu.palette_table[7] = {
		glm::u8vec4(0xf7,0x62,0x09,0xff),
		glm::u8vec4(0xf7,0x93,0x00,0xff),
		glm::u8vec4(0x00,0x00,0x00,0x00),
		glm::u8vec4(0x00,0x00,0x00,0x00),
	};

	alive = true;
}

PlayMode::~PlayMode() {
}

//Yilin's change begin
//subtract score when the 
void PlayMode::check_collision(){
	for(int i = 0; i < asteroid_positions.size(); i++){
		float dist_x = std::fabs(asteroid_positions[i].x - player_at.x); 
		float dist_y = std::fabs(asteroid_positions[i].y - player_at.y); 
		if(dist_x < x_pad_asteroid && dist_y < y_pad_asteroid){
			alive = false; 
		}
	}
}



void PlayMode::move_asteroid(float elapsed){
	std::mt19937 generator (123);
	std::uniform_real_distribution<double> dis(0.0, 1.0);//source: https://stackoverflow.com/questions/22923551/generating-number-0-1-using-mersenne-twister-c
	//1. update the position of 3 asteroids; 
	//2. change the x speed  and direction if hit left wall or the right wall; 
	//3. change y speed if hit the top or the buttom wall; 
	for(int i = 0; i < asteroid_positions.size(); i++){
		float new_x = asteroid_positions[i].x + asteroid_x_speed[i] * elapsed; 
		float new_y = asteroid_positions[i].y + asteroid_y_speed[i] * elapsed;
		//update the position first; then change the speed
		asteroid_positions[i].x = new_x; 
		asteroid_positions[i].y = new_y; 
		float new_speed_val = dis(generator) * 70.0f;; 
		if(new_x < 10){
			asteroid_x_speed[i] = fabs(new_speed_val);
		}
		if(new_x > 246){
			asteroid_x_speed[i] = (-1.0f) * fabs(new_speed_val);
		}
		if(new_y < 10){
			asteroid_y_speed[i] = fabs(new_speed_val);
		}
		if(new_y > 230){
			asteroid_y_speed[i] = (-1.0f) * fabs(new_speed_val);
		}
	}
}
//Yilin's change end


bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}
	}

	return false;
}

bool PlayMode::update(float elapsed) {

	//slowly rotates through [0,1):
	// (will be used to set background color)
	background_fade += elapsed / 10.0f;
	background_fade -= std::floor(background_fade);


	//TODO: just for debug
	std::mt19937 generator (123);
	std::uniform_real_distribution<double> dis(0.0, 1.0);
	constexpr float PlayerSpeed = 50.0f;
	if (left.pressed) player_at.x -= PlayerSpeed * elapsed;
	if (right.pressed) player_at.x += PlayerSpeed * elapsed;
	if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	if (up.pressed) player_at.y += PlayerSpeed * elapsed;


	//Yilin's change begin
	move_asteroid(elapsed);
	score += 0.2f;
	//Yilin's change end

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;

	check_collision();
	return alive;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

	//background color will be some hsv-like fade:
	// ppu.background_color = glm::u8vec4(
	// 	std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 0.0f / 3.0f) ) ) ))),
	// 	std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 1.0f / 3.0f) ) ) ))),
	// 	std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 2.0f / 3.0f) ) ) ))),
	// 	0xff
	// );

	// //tilemap gets recomputed every frame as some weird plasma thing:
	// //NOTE: don't do this in your game! actually make a map or something :-)
	// for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y) {
	// 	for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x) {
	// 		//TODO: make weird plasma thing
	// 		ppu.background[x+PPU466::BackgroundWidth*y] = ((x+y)%16);
	// 	}
	// }

	//background scroll:
	// ppu.background_position.x = int32_t(-0.5f * player_at.x);
	// ppu.background_position.y = int32_t(-0.5f * player_at.y);

	//player sprite:
	ppu.sprites[0].x = int8_t(player_at.x);
	ppu.sprites[0].y = int8_t(player_at.y);
	ppu.sprites[0].index = 32;
	ppu.sprites[0].attributes = 6;

	for(uint32_t i = 0; i < asteroid_positions.size(); i++){
		ppu.sprites[i + 1].x = int8_t(asteroid_positions[i].x);
		ppu.sprites[i + 1].y = int8_t(asteroid_positions[i].y);
		ppu.sprites[i + 1].index = 33;
		ppu.sprites[i + 1].attributes = 7;
	}

	//--- actually draw ---
	ppu.draw(drawable_size);
}
