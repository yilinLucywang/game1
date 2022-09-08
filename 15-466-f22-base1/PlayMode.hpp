#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

//Lucy's include
//Yilin's change 
#include <list>
#include <cmath>
#include <random>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual bool update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//some weird background animation:
	float background_fade = 0.0f;

	//player position:
	glm::vec2 player_at = glm::vec2(0.0f);

	//----- drawing handled by PPU466 -----

	//Yilin's change begin here
	void move_asteroid(float elapsed);
	void check_collision();
	int max_num_asteroid = 10;
	std::vector<float> asteroid_x_speed{30.0f, -30.0f, 30.0f, 30.0f, -30.0f, 30.0f, -30.0f, 30.0f, 30.0f, -30.0f};
	std::vector<float> asteroid_y_speed{-30.0f, 30.0f, -30.0f, -30.0f, -30.0f, 30.0f, -30.0f, 30.0f, 30.0f, -30.0f};
	std::vector<glm::vec2> asteroid_positions{glm::vec2(10.0f), glm::vec2(20.0f), glm::vec2(30.0f), glm::vec2(40.0f), glm::vec2(50.0f), glm::vec2(60.0f), glm::vec2(70.0f), glm::vec2(80.0f),glm::vec2(90.0f), glm::vec2(100.0f)};
	int x_pad_asteroid = 2;
	int y_pad_asteroid = 2; 
	// ScreenWidth = 256,
	// ScreenHeight = 240
	int screen_width = 256; 
	int screen_height = 240;
	float score = 0.0f;
	//Yilin's change end

	PPU466 ppu;
	public: 
		bool alive = true; 
};
