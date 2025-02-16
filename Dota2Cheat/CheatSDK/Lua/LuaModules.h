#pragma once
#include <sol/sol.hpp>
#include <filesystem>
#include <ShlObj_core.h>


using directory_iterator = std::filesystem::directory_iterator;

namespace Lua {


	// Calls a function in every module that has it
	template<typename ...Args>
	inline void CallModuleFunc(const std::string& funcName, Args&&... args) {
		ctx.lua["Modules"]["Core"]["CallModuleFunc"](funcName, std::forward<Args>(args)...);
	}

	inline void CallModuleFunc(const std::string& funcName) {
		ctx.lua["Modules"]["Core"]["CallModuleFunc"](funcName);
	}

	// Loads and executes scripts from the cheat's folder in C:\Users\%USER%\Documents\Dota2Cheat\scripts
	inline void LoadScriptFiles(sol::state& lua) {

		lua.create_named_table("Modules");
		auto scriptsPath = ctx.cheatFolderPath + R"(\scripts)";
		if (!std::filesystem::exists(scriptsPath))
			std::cout << scriptsPath << " not found! No scripts for you\n";

		for (auto& file : directory_iterator(scriptsPath)) {
			auto path = file.path();
			if (path.string().substr(path.string().size() - 3) == "lua")
				lua.load_file(path.string())();
		}

		CallModuleFunc("OnLoaded");
	}
}