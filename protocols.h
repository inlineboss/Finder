#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_

#include <string>
#include <cstddef>
#include <boost/filesystem.hpp>
#include <unordered_set>
#include <boost/unordered_set.hpp>
#include <boost/container/vector.hpp>

namespace protocols {

struct regulation {

	boost::container::vector<boost::filesystem::path> includes;
	boost::unordered_set<boost::filesystem::path> excludes = {""};

	std::size_t depth = 0;

	std::size_t min_size_file = 1;
	std::size_t max_size_file = 1;

	std::size_t size_memory_block = 1;

	std::vector<std::__cxx11::string> mask = {""};

	enum class hasher_type {
		crc32,
		md5
	} hasher ;
};

} //namespace protocols

#endif /* PROTOCOLS_H_ */
