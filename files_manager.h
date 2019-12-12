#ifndef FILES_MANAGER_H_
#define FILES_MANAGER_H_

#include <iterator>

#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include "terminal_options.h"

template <typename PathsType>
class FilesManager {
public:

	class Paths {
	private:
			boost::filesystem::recursive_directory_iterator dir;
			boost::filesystem::recursive_directory_iterator start;
			boost::filesystem::recursive_directory_iterator end;

			bool is_file = false;
			bool is_dir = false;
			bool is_end = false;
			std::size_t lvl = 0;
			std::string name = {""};
			std::size_t size = 0;

			void getInfo () {
				is_dir = boost::filesystem::is_directory(dir->status());
				is_file = boost::filesystem::is_regular_file(dir->status());
				lvl = dir.level();
				name = dir->path().filename().string();
				size = boost::filesystem::file_size(*dir);
			}

	public:
		Paths(boost::filesystem::path path):
			dir( path ), start(dir) {
		}

		inline bool isDir() {
			return is_dir;
		}

		inline bool isFile() {
			return is_file;
		}
		inline bool isEnd() {
			return is_end;
		}

		inline std::size_t getSize () {
			return size;
		}
		inline std::size_t getLvl() {
			return lvl;
		}

		inline std::string getName() {
			return name;
		}

		template <typename List>
		bool equalityMask(const List& list) {

			return std::find_if(list.being(), list.end(), [&name](const auto& object){
					return boost::regex_match(name, object);
			});

		}

		void operator++ () {
			if (dir != end) {
				dir++;

				getInfo();
				is_end = false;
			} else {
				is_end = true;
			}
		}

		void operator-- () {
			if (dir != start) {
				dir--;

				getInfo();
				is_end = false;
			} else {
				is_end = true;
			}
		}


	};

	using paths_storage = boost::container::vector<boost::filesystem::path>;

	FilesManager(const PathsType& paths): paths(paths){};

	const PathsType& paths;

	template <typename MaskType>
	bool find (std::size_t depth = 0, std::size_t min_size = 1, const MaskType& masks) {
		for (auto It : paths) {

			Paths path(It);

			while ( path.getLvl() != depth ) {

				if ( path.isFile() && path.getSize() >= min_size ) {

					storage.push_back(It);

				} else if ( path.isDir() ) {



				}

			}
		}
		return true;
	}


    std::size_t size() const { return storage.count() ; }

    value& operator[](std::size_t index)
    {
        assert(index < storage.count());
        return storage[index];
    }

    const value& operator[](std::size_t index) const
    {
        assert(index < storage.count());
        return storage[index];
    }

    template <typename T>
    T begin()
    {
        return storage.begin();
    }

    template <typename T>
    T end()
    {
        return  storage.end();
    }

private:

    paths_storage storage;
};



#endif /* FILES_MANAGER_H_ */
