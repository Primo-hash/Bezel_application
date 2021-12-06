/*
	The layer.h header is used along with a container stack class to create a
	layer stack system.
*/
#pragma once
#include "time.h"
#include "events/event.h"

namespace engine {

	/*
		The Layer class is part of the layer stack system.
		By layering the draw execution on the application window the developer has better control
		over modularising and ordering draw calls. More details below.

		The class itsself is an interface to allow the application side to define the content of the
		below functions.
	*/
	class Layer {
	public:
		Layer() = default;
		virtual ~Layer();

		virtual void onAttach() {}				// Commands to be executed on attachment to stack 
		virtual void onDetach() {}				// Commands to be executed on detachment
		virtual void onUpdate(Time ts) {}		// Commands to be executed every game loop cycle
		virtual void onEvent(Event& event) {}	// Event specific functions to register events for this layer
	};


	/*
		The layer stack system implemented for the engine in more detail.

		By layering the draw execution on the application window the application can:
			* Turn on and off draw clusters based on layer
			* Ordering based on which layer gets to draw first
			* Ordering based on which layer receives events first.
			* Integrating overlay draw priority (UI, menu and so on)

		Ordering layers allows us to draw overlays last, and to give priority to
		game/app content. Ordering which layer handles events first, gives us the
		ability to give priority to overlays when events happen.
		An example would be clicking on a button in the GUI of an fps shooter.
		By giving the overlay priority the button will execute while the player doesn't
		shoot randomly in the game because the overlay blocks it by tagging the event as handled.
	*/
}