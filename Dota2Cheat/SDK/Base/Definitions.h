#pragma once
#include <cstdint>

using ENT_HANDLE = uint32_t;
constexpr uint32_t ENT_HANDLE_MASK = 0x7fff;
constexpr uint32_t NET_ENT_HANDLE_MASK = 0x3fff;
constexpr uint32_t INVALID_HANDLE = 0xFFFFFFFF;

#define H2IDX(H) ((H) & ENT_HANDLE_MASK) // Entity handle to entity index
#define NH2IDX(H) ((H) & NET_ENT_HANDLE_MASK) // Network messages have a mask that's two times smaller
#define HVALID(H) ((H) != INVALID_HANDLE)

// McDota technologies
#define CASE_STRING( x ) case static_cast<int>( x ) : return #x
#define CASE_STD_STRING( x ) case static_cast<int>( x ) : return std::string(#x)

// Macros for dota classes
#define GETTER(type, name, offset) type name() { return Member<type>(offset); }			// regular getter
#define IGETTER(type, name, offset) type* name() { return MemberInline<type>(offset); }  // inline field
#define FIELD(type, name, offset) type& name() { return Field<type>(offset); }           // like GETTER, but returns a reference
#define VGETTER(type, name, index) type name() { return CallVFunc<index, type>(); }     // vfunc without arguments