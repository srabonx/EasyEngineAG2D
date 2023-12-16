#include "EasyEngineAG2D.h"

Sample::Sample() {
	sample = NULL;
	channel = NULL;
}

Sample::~Sample() {
	if (sample != NULL)
	{
		FMOD_Sound_Release(sample);
		sample = NULL;
	}
}

namespace EE {
	Audio::Audio()
	{
		system = NULL;
	}
	Audio::~Audio()
	{
		// Release all samples
		for (smplItr i = samples.begin(); i != samples.end(); ++i)
		{
			(*i) = NULL;
		}
		FMOD_System_Release(system);
	}

	bool Audio::Init()
	{
		if (FMOD_System_Create(&system, FMOD_VERSION) != FMOD_OK)
			return false;
		if (FMOD_System_Init(system, 100, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
			return false;


		return true;
	}

	void Audio::Update()
	{
		FMOD_System_Update(system);
	}

	bool Audio::Load(std::string filename, std::string name)
	{
		if (filename.length() == 0 || name.length() == 0) return false;

		Sample* sample = new Sample();
		sample->setName(name);
		FMOD_RESULT res;
		res = FMOD_System_CreateSound(system,								// FMOD system
									  filename.c_str(),						// Filename
									  FMOD_DEFAULT,							// Default audio
									  NULL,
									  &sample->sample);						// Pointer to sample
		if (res != FMOD_OK)
			return false;

		samples.push_back(sample);

		return true;
	}

	Sample* Audio::Load(std::string filename)
	{
		if (filename.length() == 0) return NULL;

		Sample* sample = new Sample();
		FMOD_RESULT res;
		res = FMOD_System_CreateSound(system,							// FMOD system
									filename.c_str(),					// filename 
									FMOD_DEFAULT,						// default audio
									NULL,
									&sample->sample);					// pointer to sample

		if (res != FMOD_OK)
			sample = NULL;

		return sample;
	}

	bool Audio::Play(std::string name)
	{
		FMOD_RESULT res;
		Sample* sample = FindSample(name);
		if (sample->sample != NULL)
		{
		 // Sample found! play it
			res = FMOD_System_PlaySound(system,
										sample->sample,
										NULL,
										true,
										&sample->channel);
			if (res != FMOD_OK) return false;
			FMOD_Channel_SetLoopCount(sample->channel, -1);
			FMOD_Channel_SetPaused(sample->channel, false);
		}
		return true;
	}

	bool Audio::Play(Sample* sample)
	{
		FMOD_RESULT res;
		if (sample == NULL) return false;
		if (sample->sample == NULL) return false;
		res = FMOD_System_PlaySound(system,
									sample->sample,
									NULL,
									true,
									&sample->channel);
		if (res != FMOD_OK) return false;
		FMOD_Channel_SetLoopCount(sample->channel, -1);
		FMOD_Channel_SetPaused(sample->channel, false);
		return true;
	}

	void Audio::Stop(std::string name)
	{
		if (!IsPlaying(name)) return;
		Sample* sample = FindSample(name);
		if (sample == NULL) return;
		FMOD_Channel_Stop(sample->channel);
	}

	void Audio::StopAll()
	{
		for (smplItr i = samples.begin(); i != samples.end(); ++i)
			FMOD_Channel_Stop((*i)->channel);
	}

	void Audio::StopAllExcept(std::string name)
	{
		for (smplItr i = samples.begin(); i != samples.end(); ++i)
			if ((*i)->getName() != name)
				FMOD_Channel_Stop((*i)->channel);
	}

	bool Audio::IsPlaying(std::string name)
	{
		Sample* samp = FindSample(name);
		if (samp == NULL) return false;
		
		int index;
		FMOD_Channel_GetIndex(samp->channel, &index);

		// FMOD returns 99 if channel is playing , 0 if not
		return (index > 0);
	}

	bool Audio::SampleExist(std::string name)
	{
		for (smplItr i = samples.begin(); i != samples.end(); ++i)
			if ((*i)->getName() == name)
				return true;

		return false;
	}
	Sample* Audio::FindSample(std::string name)
	{
		Sample* sample = NULL;

		for (smplItr i = samples.begin(); i != samples.end(); ++i)
		{
			if ((*i)->getName() == name)
			{
				sample = (*i);
				break;
			}
		}

		return sample;
	}
};
