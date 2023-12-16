#include "EasyEngineAG2D.h"

namespace EE {
	Script::Script()
	{
		luaState = lua_open();
		luaL_openlibs(luaState);
	}

	Script::Script(std::string scriptFile)
	{
		loadScript(scriptFile);
	}

	Script::~Script()
	{
		lua_close(luaState);
	}

	bool Script::loadScript(std::string scriptFile)
	{
		bool value = true;
		try
		{
			luaL_dofile(luaState, scriptFile.c_str());
		}
		catch (...)
		{
			value = false;
		}
		return value;
	}

	std::string Script::getGlobalString(std::string name)
	{
		std::string value = "";
		try
		{
			lua_getglobal(luaState, name.c_str());
			value = lua_tostring(luaState, -1);
			lua_pop(luaState, 1);
		}
		catch (...)
		{

		}
		return value;
	}

	void Script::setGlobalString(std::string name, std::string value)
	{
		lua_pushstring(luaState, value.c_str());
		lua_setglobal(luaState, name.c_str());
	}

	double Script::getGlobalNumber(std::string name)
	{
		double value = 0.0;
		try
		{
			lua_getglobal(luaState, name.c_str());
			value = lua_tonumber(luaState, -1);
			lua_pop(luaState, 1);
		}
		catch (...)
		{

		}
		return value;
	}

	void Script::setGlobalNumber(std::string name, double value)
	{
		lua_pushnumber(luaState, (int)value);
		lua_setglobal(luaState, name.c_str());
	}

	bool Script::getGlobalBoolean(std::string name)
	{
		bool value = 0;
		try
		{
			lua_getglobal(luaState, name.c_str());
			value = (bool)(int)lua_toboolean(luaState, -1);
			lua_pop(luaState, 1);
		}
		catch (const std::exception&)
		{

		}
		return value;
	}

	void Script::setGlobalBoolean(std::string name, bool value)
	{
		lua_pushboolean(luaState, (int)value);
		lua_setglobal(luaState, name.c_str());
	}

	void Script::runFunction(std::string name)
	{
		// call script function, 0 args , 0 return values
		lua_getglobal(luaState, name.c_str());
		lua_call(luaState, 0, 0);
	}
};
