/*
 * motherboard_disk_strategy.cpp
 *
 *  Created on: Dec 30, 2024
 *      Author: giedrius-N
 */

#include "motherboard_disk_strategy.hpp"

using namespace std;

namespace license {
namespace hw_identifier {

static constexpr uint32_t FNV_PRIME_32 = 16777619u;
static constexpr uint32_t FNV_OFFSET_BASIS_32 = 2166136261u;

static uint32_t fnv1a_32(const std::string& data) { 
	uint32_t hash = FNV_OFFSET_BASIS_32; 
	for (unsigned char c : data) {
		hash ^= c;
		hash *= FNV_PRIME_32;
	}

	return hash;
}

static array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA> generate_id_by_sn(const std::string& sn) {
	array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA> id_array = {};
	uint32_t hash = fnv1a_32(sn);

	for (size_t i = 0; i < HW_IDENTIFIER_PROPRIETARY_DATA; i++) {
		uint32_t shift = (i % 4) * 8;
		uint8_t byteVal = static_cast<uint8_t>((hash >> shift) & 0xFF);
		id_array[i] = byteVal ^ static_cast<uint8_t>(i);
	}

	return id_array;
}

#ifdef _WIN32
static FUNCTION_RETURN generate_motherboard_pc_id(vector<array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA>> &v_motherboard_id) {	
	hwinfo::MainBoard main_board;
	std::string sn = main_board.serialNumber();

	v_motherboard_id.emplace_back(generate_id_by_sn(sn));

	if (v_motherboard_id.back().empty()) {
		v_motherboard_id.pop_back();
	}

	return v_motherboard_id.size() > 0 ? FUNC_RET_OK : FUNC_RET_NOT_AVAIL;
}
#endif

#ifdef Linux
static FUNCTION_RETURN generate_disk_pc_id(vector<array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA>> &v_disk_id) {
	const vector<hwinfo::Disk> disks = hwinfo::getAllDisks();
	v_disk_id.reserve(disks.size());

	for (size_t i = 0; i < disks.size(); i++) {
		v_disk_id.emplace_back(generate_id_by_sn(disks[i].serialNumber()));
	}

	return v_disk_id.size() > 0 ? FUNC_RET_OK : FUNC_RET_NOT_AVAIL;
}
#endif

LCC_API_HW_IDENTIFICATION_STRATEGY MotherboardDiskStrategy::identification_strategy() const {
	return LCC_API_HW_IDENTIFICATION_STRATEGY::STRATEGY_MOTHERBOARD_DISK;
}

MotherboardDiskStrategy::~MotherboardDiskStrategy() {}

vector<HwIdentifier> MotherboardDiskStrategy::alternative_ids() const {
	vector<HwIdentifier> identifiers;
	vector<array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA>> data;

	FUNCTION_RETURN result = FUNC_RET_NOT_AVAIL;

#ifdef _WIN32
	result = generate_motherboard_pc_id(data);
#elif defined(Linux)
	result = generate_disk_pc_id(data);
#endif
	
	if (result == FUNC_RET_OK) {
		identifiers.reserve(data.size());
		for (auto &it : data) {
			HwIdentifier pc_id;
			pc_id.set_identification_strategy(identification_strategy());
			pc_id.set_data(it);
			identifiers.push_back(pc_id);
		}
	}

	return identifiers;
}

} // hw_identifier
} // license