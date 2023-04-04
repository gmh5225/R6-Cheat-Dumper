#ifndef _UTIl_UTIL_HPP_
#define _UTIL_UTIL_HPP_

#include <iostream>

namespace util {
	uint32_t get_pid_from_window(const char* window_name);
	uint32_t get_pid_from_class(const char* window_class);
	uint32_t get_pid_from_file(const char target[]);
	bool is_game_open(const char* window_title, const char* window_class, const char window_file[]);
}

#endif