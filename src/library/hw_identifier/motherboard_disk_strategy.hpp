/*
 * motherboard_disk_strategy.hpp
 *
 *  Created on: Dec 30, 2024
 *      Author: giedrius-N
 */

#ifndef SRC_LIBRARY_PC_IDENTIFIER_MOTHERBOARD_DISK_STRATEGY_HPP_
#define SRC_LIBRARY_PC_IDENTIFIER_MOTHERBOARD_DISK_STRATEGY_HPP_

#include "identification_strategy.hpp"
#include "hwinfo/hwinfo.h"
#include <vector>
#include <array>

namespace license {
namespace hw_identifier {

class MotherboardDiskStrategy : public IdentificationStrategy {
public:
	inline MotherboardDiskStrategy(){};
	virtual ~MotherboardDiskStrategy();
	virtual LCC_API_HW_IDENTIFICATION_STRATEGY identification_strategy() const;
	virtual std::vector<HwIdentifier> alternative_ids() const;
};

} // namespace hw_identifier
} // namespace license

#endif /* SRC_LIBRARY_PC_IDENTIFIER_MOTHERBOARD_DISK_STRATEGY_HPP_ */
