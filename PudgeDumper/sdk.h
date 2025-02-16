#pragma once
#include <Windows.h>
#include <iostream>
#include <cstdint>
#include <string_view>
#include <vector>

constexpr auto PI = 3.1415926;

using ENT_HANDLE = uint32_t;
constexpr uint32_t ENT_HANDLE_MASK = 0x7fff;
constexpr uint32_t NET_ENT_HANDLE_MASK = 0x3fff;
#define NH2IDX(H) ((H) & NET_ENT_HANDLE_MASK) // Network messages have a mask that's two times smaller
#define H2IDX(H) ((H) & ENT_HANDLE_MASK) // Entity handle to entity index
#define HVALID(H) ((H) != 0xFFFFFFFF)

#define u64 unsigned long long


//credits to Liberalist from YouGame
//very handy classes
class Function {
public:
	void* ptr;
	Function(uintptr_t ptr) : ptr((void*)ptr) {

	}
	template<typename ...T>
	void* __fastcall operator()(T... t) {
		return (void*)((u64(__fastcall*)(T...))ptr)(t...);
	}
	template<typename V, typename ...T>
	V __fastcall Execute(T... t) {
		return ((V(__fastcall*)(T...))ptr)(t...);
	}

};
class VClass {
public:
	virtual void dummy_fn() = 0; // so that the classes have a vtable
	template<typename T>
	T Member(int offset, T defaultValue = T{}) {
		if (!offset)
			return defaultValue;
		return *(T*)((uintptr_t)this + offset);
	}

	Function GetVFunc(int index)
	{
		uintptr_t vtable = *((uintptr_t*)(this));
		uintptr_t entry = vtable + sizeof(uintptr_t) * index;
		return Function(*(uintptr_t*)entry);
	}
	template<uint32_t index, typename RET = void*, typename ...T>
	RET CallVFunc(T... t) {
		return GetVFunc(index).Execute<RET>(this, t...);
	}
};

template <class T>
class CUtlVector
{
public:
	uint32_t m_Size;
	T* m_pElements;
	uint32_t m_Capacity;

	T& operator[](int i)
	{
		return m_pElements[i];
	}

	T& at(int i) {
		return m_pElements[i];
	}

	T* begin() {
		return m_pElements;
	}

	T* end() {
		return m_pElements + m_Size;
	}

	[[nodiscard]] std::vector<T> AsStdVector() {
		auto result = std::vector<T>{};
		result.reserve(m_Size);
		for (int i = 0; i < m_Size; i++)
			result.push_back(m_pElements[i]);
		return result;
	}

	int Count() const
	{
		return m_Size;
	}
};

using UtlTsHashHandleT = std::uint64_t;

class CUtlMemoryPool {
public:
	// returns number of allocated blocks
	int BlockSize() const {
		return m_blocks_per_blob_;
	}
	int Count() const {
		return m_block_allocated_size_;
	}
	int PeakCount() const {
		return m_peak_alloc_;
	}
private:
	std::int32_t m_block_size_ = 0; // 0x0558
	std::int32_t m_blocks_per_blob_ = 0; // 0x055C
	std::int32_t m_grow_mode_ = 0; // 0x0560
	std::int32_t m_blocks_allocated_ = 0; // 0x0564
	std::int32_t m_block_allocated_size_ = 0; // 0x0568
	std::int32_t m_peak_alloc_ = 0; // 0x056C
};

template <class T, class Keytype = std::uint64_t>
class CUtlTSHash {
public:
	// Invalid handle.
	static UtlTsHashHandleT InvalidHandle(void) {
		return static_cast<UtlTsHashHandleT>(0);
	}

	// Returns the number of elements in the hash table
	[[nodiscard]] int BlockSize() const {
		return m_entry_memory_.BlockSize();
	}
	[[nodiscard]] int Count() const {
		return m_entry_memory_.Count();
	}

	// Returns elements in the table
	std::vector<T> GetElements(void);
public:
	// Templatized for memory tracking purposes
	template <typename DataT>
	struct HashFixedDataInternalT {
		Keytype m_ui_key;
		HashFixedDataInternalT<DataT>* m_next;
		DataT m_data;
	};

	using HashFixedDataT = HashFixedDataInternalT<T>;

	// Templatized for memory tracking purposes
	template <typename DataT>
	struct HashFixedStructDataInternalT {
		DataT m_data;
		Keytype m_ui_key;
		char pad_0x0020[0x8];
	};

	using HashFixedStructDataT = HashFixedStructDataInternalT<T>;

	struct HashStructDataT {
		char pad_0x0000[0x10]; // 0x0000
		std::array<HashFixedStructDataT, 256> m_list;
	};

	struct HashAllocatedDataT {
	public:
		auto GetList() {
			return m_list_;
		}
	private:
		char pad_0x0000[0x18]; // 0x0000
		std::array<HashFixedDataT, 128> m_list_;
	};

	// Templatized for memory tracking purposes
	template <typename DataT>
	struct HashBucketDataInternalT {
		DataT m_data;
		HashFixedDataInternalT<DataT>* m_next;
		Keytype m_ui_key;
	};

	using HashBucketDataT = HashBucketDataInternalT<T>;

	struct HashUnallocatedDataT {
		HashUnallocatedDataT* m_next_ = nullptr; // 0x0000
		Keytype m_6114; // 0x0008
		Keytype m_ui_key; // 0x0010
		Keytype m_i_unk_1; // 0x0018
		std::array<HashBucketDataT, 256> m_current_block_list; // 0x0020
	};

	struct HashBucketT {
		HashStructDataT* m_struct_data = nullptr;
		void* m_mutex_list = nullptr;
		HashAllocatedDataT* m_allocated_data = nullptr;
		HashUnallocatedDataT* m_unallocated_data = nullptr;
	};

	CUtlMemoryPool m_entry_memory_;
	HashBucketT m_buckets_;
	bool m_needs_commit_ = false;
};

template <class T, class Keytype>
std::vector<T> CUtlTSHash<T, Keytype>::GetElements(void) {
	std::vector<T> list;

	const int n_count = Count();
	auto n_index = 0;
	auto& unallocated_data = m_buckets_.m_unallocated_data;
	for (auto element = unallocated_data; element; element = element->m_next_) {
		for (auto i = 0; i < BlockSize() && i != n_count; i++) {
			list.emplace_back(element->m_current_block_list.at(i).m_data);
			n_index++;

			if (n_index >= n_count)
				break;
		}
	}
	return list;
}
