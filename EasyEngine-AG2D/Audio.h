#include "EasyEngineAG2D.h"
#pragma once

class Sample {
private:
	std::string name;
public:
	FMOD_SOUND* sample;
	FMOD_CHANNEL* channel;
	Sample(void);
	~Sample(void);
	std::string getName() { return name; }
	void setName(std::string nm) { name = nm; }
};

namespace EE {

	class Audio
	{
	private:
		FMOD_SYSTEM* system;
		typedef std::vector<Sample*> Samples;
		typedef std::vector<Sample*>::iterator smplItr;
		Samples samples;
	public:
		Audio();
		~Audio();
		FMOD_SYSTEM* getSystem() { return system; }
		bool Init();
		void Update();				// Must be called once per frame
		bool Load(std::string filename,std::string name);
		Sample* Load(std::string filename);
		bool Play(std::string name);
		bool Play(Sample* sample);
		void Stop(std::string name);
		void StopAll();
		void StopAllExcept(std::string name);
		bool IsPlaying(std::string name);
		bool SampleExist(std::string name);
		Sample* FindSample(std::string name);
	};

};

