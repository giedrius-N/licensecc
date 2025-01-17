add_library(hw_identifier OBJECT
	hw_identifier_facade.cpp
	default_strategy.cpp
	ethernet.cpp
	disk_strategy.cpp
	identification_strategy.cpp
	hw_identifier.cpp
	motherboard_disk_strategy.cpp
	motherboard_disk_strategy.hpp
)

if(CODE_COVERAGE AND UNIX)
	  target_compile_options(hw_identifier PUBLIC -O0 -g --coverage)
endif(CODE_COVERAGE AND UNIX)

target_link_libraries(hw_identifier lfreist-hwinfo::hwinfo)