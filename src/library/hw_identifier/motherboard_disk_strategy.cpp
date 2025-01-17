/*
 * motherboard_disk_strategy.cpp
 *
 *  Created on: Dec 30, 2024
 *      Author: giedrius-N
 */

#include "motherboard_disk_strategy.hpp"
#include "hwinfo/hwinfo.h"
#include <algorithm>

using namespace std;

namespace license {
namespace hw_identifier {

static array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA> generate_id_by_motherboard_sn(const std::string& sn) {
	array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA> id_array = {};

	size_t size_to_copy = min(HW_IDENTIFIER_PROPRIETARY_DATA, sn.size());

	memcpy(id_array.data(), sn.data(), size_to_copy);

	return id_array;
}

static FUNCTION_RETURN generate_disk_pc_id(vector<array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA>> &v_disk_id) {
	return FUNC_RET_OK;
}

static FUNCTION_RETURN generate_motherboard_pc_id(vector<array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA>> &v_motherboard_id) {
	hwinfo::MainBoard main_board;
	std::string sn = main_board.serialNumber();

	array<uint8_t, HW_IDENTIFIER_PROPRIETARY_DATA> motherboard_id = generate_id_by_motherboard_sn(sn);

	if (!motherboard_id.empty()) {
		v_motherboard_id.push_back(motherboard_id);
	}

	return v_motherboard_id.size() > 0 ? FUNC_RET_OK : FUNC_RET_NOT_AVAIL;
}

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
#endif

#ifdef linux



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