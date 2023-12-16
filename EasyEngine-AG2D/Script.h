#include "EasyEngineAG2D.h"
#pragma once

namespace EE {

	class Script
	{
	private:
		lua_State* luaState;

	public:
		Script();
		Script(std::string scriptFile);
		virtual ~Script();

		bool loadScript(std::string scriptFile);
		
		std::string getGlobalString(std::string name);
		void setGlobalString(std::string name,std::string value);

		double getGlobalNumber(std:: string name);
		void setGlobalNumber(std::string name, double value);

		bool getGlobalBoolean(std::string name);
		void setGlobalBoolean(std::string name, bool value);

		void runFunction(std::string name);

	};// class

}// namespace

