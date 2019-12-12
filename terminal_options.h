#ifndef TERMINAL_OPTIONS_H_
#define TERMINAL_OPTIONS_H_

#include <boost/program_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <vector>

#include "protocols.h"

class TerminalOptions {
private:
	boost::program_options::variables_map vm;
	boost::program_options::options_description desc = {"Allowed options"};
public:

	TerminalOptions( int argc, const char * argv[] ) {

		set_settings_key();

		try {

		boost::program_options::store (
				boost::program_options::parse_command_line(
						argc, argv, desc), vm);

		} catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            std::cout << desc << std::endl;
            is_faild = true;
            return;
		}

		fill_options();

	}

	protocols::regulation keys;

	bool is_faild = false;
private:

	inline bool is_dir(const std::string& str) {

		using namespace boost::filesystem;

		 return is_directory( status(path(str)));

	}

	template<typename Out, typename In>
	void check_dirs (Out insert, const In& includes) {
		using namespace boost::filesystem;

		if(includes.empty()) {
			return;
		}

		 for (auto It : includes) {

			 if( is_dir(It) ){

				 insert(canonical(path(It)));

			 } else {

				 std::cout << It << " - не является дирректорией -> исключен из поиска" << std::endl;

			 }

		 }
	}

	void fill_options() {

		try {

			if(vm.count("help")) {
				std::cout << desc << std::endl;
			}


			if(vm.count("depth")) {
				keys.depth = vm["depth"].as<std::size_t>();
			}


			if(vm.count("include")) {

				 check_dirs ([&](const boost::filesystem::path& dir){

					 keys.includes.push_back(dir);

					}, vm["include"].as<std::vector<std::__cxx11::string>>() );

			}
			if(vm.count("exclude")) {

				 check_dirs ([&](const boost::filesystem::path& dir){

					 keys.excludes.insert(dir);

					}, vm["include"].as<std::vector<std::__cxx11::string>>() );

			}
			if(vm.count("mask")) {

				keys.mask = vm["mask"].as<std::vector<std::__cxx11::string>>();

			}
			if(vm.count("blocksize")) {

				 auto Key = vm["blocksize"].as<std::__cxx11::string>();
				 (void) Key;
//				 keys.size_memory_block =

			}
			if(vm.count("algorithm")) {

				if (std::string ch_algo = vm["algorithm"].as<std::__cxx11::string>();
						std::string{"MD5"} == ch_algo) {

					keys.hasher = protocols::regulation::hasher_type::md5;

				} else if (std::string{"CRC32"} == ch_algo) {

					keys.hasher = protocols::regulation::hasher_type::crc32;

				} else {
					std::cout << "Не верно указан алогритм (CRC32, MD5)" << std::endl;
				}
			}

		} catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            std::cout << desc << std::endl;
            is_faild = true;
            return;
        }


	}

	void set_settings_key() {
	    desc.add_options()
	            ("help", "Этот экран")

	            ("include",
	            		boost::program_options::value<std::vector<std::__cxx11::string>>(),
	             "Директории для поиска")

	            ("exclude",
	            		boost::program_options::value<std::vector<std::__cxx11::string>>(),
	             "Директории для исключения из поиска")

	            ("depth",
	            		boost::program_options::value<std::size_t>()->default_value(0),
	             "Глубина сканирования (0 - только указанная директория)")

	            ("minsize",
	            		boost::program_options::value<std::size_t>()->default_value(keys.min_size_file),
	             "Минимальный размер файла, по умолчанию проверяются все файлы больше 1 байта")

	            ("mask",
	            		boost::program_options::value<std::vector<std::__cxx11::string>>(),
	             "Маски имен файлов разрешенных для сравнения (не зависят от регистра)")

	            ("blocksize", boost::program_options::value<std::__cxx11::string>()->default_value("B"),
	             "Размер блока, которым производится чтения файлов (B, K, M, G)")

	            ("algorithm", boost::program_options::value<std::string>()->default_value("MD5"),
	             "Алгоритм хэширования (CRC32 || MD5)")

	            ("debug", "Отладочная информация");
	}

};



#endif /* TERMINAL_OPTIONS_H_ */
