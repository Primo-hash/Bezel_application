#include <engine/engine.h>

//#include "pacman/include/layers/sandbox-layer.h"
#include "pacman/include/characters/pacman.h"
#include "pacman/include/inanimate-objects/map.h"
#include "pacman/include/layers/game-layer.h"

class PacmanGame : public engine::AppFrame {
public:
	PacmanGame() : AppFrame() {
		//pushLayer(NEW ExampleLayer());
		pushLayer(NEW GameLayer);
	}


	PacmanGame(engine::WindowSpecs specs) : AppFrame(specs) {
		setAppIcon("assets/textures/pacman3.png");	// Window icon

		pushLayer(NEW GameLayer);
	}


	~PacmanGame() {

	}
};

/*
	Returns a new application sandbox defined and executed engine side.
*/
engine::u_Ptr<engine::AppFrame> engine::createApp() {
	auto windowSpecs = engine::WindowSpecs("Pacman", 1600, 900);

	return engine::m_UPtr<PacmanGame>(windowSpecs);
}