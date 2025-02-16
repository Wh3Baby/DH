#pragma once
#include <vector>
#include <iostream>

// Game Events
// Since McDota's times second virtual destructors were removed

class CGameEvent
{
public:
	virtual void DESTROY() = 0;

	virtual const char* GetName(void) = 0;
	virtual int GetID(void) = 0;

	virtual bool IsReliable(void) = 0;
	virtual bool IsLocal(void) = 0;
	virtual bool IsEmpty(char const* keyname = nullptr) = 0;

	virtual bool GetBool(char const* keyname = nullptr, bool defaultValue = false) = 0;
	virtual int GetInt(char const* keyname = nullptr, int defaultValue = 0) = 0;
	virtual uint64_t GetUint64(char const* keyname = nullptr, uint64_t DefaultValue = 0) = 0;
	virtual float GetFloat(char const* keyname = nullptr, float defaultValue = 0.0f) = 0;
	virtual const char* GetString(char const* keyname = nullptr, char const* defaultValue = nullptr) = 0;
	virtual const void* GetPtr(char const* keyname = nullptr) = 0;

private:
	virtual void unk0() = 0;
	virtual void unk1() = 0;
	virtual void unk2() = 0;
	virtual void unk3() = 0;
	virtual void unk4() = 0;
	virtual void unk5() = 0;
	virtual void unk6() = 0;
	virtual void unk7() = 0;

public:
	virtual void SetBool(char const* keyname, bool value) = 0;
	virtual void SetInt(char const* keyname, int value) = 0;
	virtual void SetUint64(char const* keyname, uint64_t value) = 0;
	virtual void SetFloat(char const* keyname, float value) = 0;
	virtual void SetString(char const* keyname, char const* value) = 0;
	virtual void SetPtr(char const* keyname, void const* value) = 0;

	// Last vfunc, easier to rebuild than put up another 5 of unk()
	void* GetDataKeys() {
		return *(void**)((uintptr_t)this + 0x10);
	};

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CGameEvent>("CGameEvent");
		type["SetBool"] = &CGameEvent::SetBool;
		type["SetInt"] = &CGameEvent::SetInt;
		type["SetUint64"] = &CGameEvent::SetUint64;
		type["SetFloat"] = &CGameEvent::SetFloat;
		type["SetString"] = &CGameEvent::SetString;
		type["SetPtr"] = &CGameEvent::SetPtr;
		type["GetName"] = &CGameEvent::GetName;
		type["GetID"] = &CGameEvent::GetID;

		type["IsReliable"] = &CGameEvent::IsReliable;
		type["IsLocal"] = &CGameEvent::IsLocal;
		type["IsEmpty"] = &CGameEvent::IsEmpty;

		type["GetBool"] = &CGameEvent::GetBool;
		type["GetInt"] = &CGameEvent::GetInt;
		type["GetUint64"] = &CGameEvent::GetUint64;
		type["GetFloat"] = &CGameEvent::GetFloat;
		type["GetString"] = &CGameEvent::GetString;
		type["GetPtr"] = &CGameEvent::GetPtr;
	}
};

class IGameEventListener2
{
	virtual void DESTROY() = 0;

	virtual void FireGameEvent(CGameEvent* event) = 0;
};

class CGameEventManager
{
public:
	virtual void DESTROY() = 0;
	virtual int LoadEventsFromFile(const char* filename) = 0;
	virtual void Reset(void) = 0;
	virtual bool AddListener(IGameEventListener2* listener, const char* eventName, bool serverSide = false) = 0;
	virtual bool FindListener(IGameEventListener2* listener, const char* eventName) = 0;
	virtual void RemoveListener(IGameEventListener2* listener) = 0;
};
