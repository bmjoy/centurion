#ifndef ENGINE_H
#define ENGINE_H

#include <gui>

using namespace std;
using namespace glm;

namespace engine {

	class Engine {
	public:
		static void Init();
		static void Reset() { reset = true; }
		static void setEnvironment(string s) { environment = s; }
		static string getEnvironment() { return environment; }
		static int launch();
		~Engine();

	private:
		Engine();
		// objects 
		static gui::SimpleText text;
		static string environment;

		// fps
		static double currentTime, lastTime, finalTime;
		static int nbFrames, Fps, Mpfs;
		static bool reset;
	
		// Private methods
		static void resetKeyCodes();
		static void fps();
		static void fps_sleep();
		static void read_data();
		static void handleGlobalKeys();
	};
};

#endif