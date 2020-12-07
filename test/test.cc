#include <iostream>
#include "../src/hash/hash.h"
#include "../src/search/a_star.h"
#include "../src/bigint/bigint.h"


int
main(int argc, char **argv)
{
	//hash
	uint8_t octects[] = {'a', 'b', 'c'};

	std::cout << algo::hash::murmur3(octects, 3) << std::endl;
	std::cout << algo::hash::fnv0_32(octects, 3) << std::endl;
	std::cout << algo::hash::fnv1_64(octects, 3) << std::endl;
	std::cout << algo::hash::fnv1a_32(octects, 3) << std::endl;
	std::cout << algo::hash::sdbm(octects, 3) << std::endl;
	std::cout << algo::hash::djb2(octects, 3) << std::endl;
	std::cout << algo::hash::crc32b(octects, 3) << std::endl;
	std::cout << algo::hash::xcrc32(octects, 3) << std::endl;

	std::cout << std::endl << std::endl;


	//a star algorithm
	algo::search::grid<int> gr {
		{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
		{ 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
		{ 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
		{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
		{ 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 }
	};

	auto a_star = algo::search::AStar(gr);

	algo::search::node src = std::make_pair(gr.size() - 1, gr[0].size() - 1);
	algo::search::node dest = std::make_pair(0, 0);

	std::vector<algo::search::node> path;

	bool found = false;

	try {
		found = a_star.search_path(src, dest, path);
	} catch(std::invalid_argument &e) {
		std::cout << e.what() << std::endl;
	}

	if (!found)
		std::cout << "Can't find a path" << std::endl;
	else {
		std::cout << "Nodes" << std::endl;
		for (auto cell : path)
			std::cout << "y" << cell.first << " -- x" << cell.second << std::endl;

		std::cout << std::endl << std::endl;

		for (int i = 0; i < gr[0].size() - 1; i++) {
			for (int j = 0; j < gr.size(); j++) {
				for (auto cell : path) {
					if (cell.first == j && cell.second == i) {
						std::cout << "\033[31;1mx\033[0m";
						goto ex1;
					}
				}

				std::cout << "\033[2mo\033[0m";
				ex1: continue;
			}
			std::cout << std::endl;
		}

		std::cout << std::endl << std::endl;

		for (int i = gr.size(); i >= 0; i--) {
			for (int j = 0; j < gr[0].size() - 1; j++) {
				for (auto cell : path) {
					if (cell.first == j && cell.second == i) {
						std::cout << "\033[31;1mx\033[0m";
						goto ex2;
					}
				}

				std::cout << "\033[2mo\033[0m";
				ex2: continue;
			}
			std::cout << std::endl;
		}

		std::cout << std::endl << std::endl;

		for (int i = gr.size(); i >= 0; i--) {
			for (int j = 0; j < gr[0].size() - 1; j++) {
				for (auto cell : path) {
					if (cell.first == j && cell.second == i) {
						std::cout << "\033[31;1mx\033[0m";
						goto ex3;
					}
				}

				std::cout << "\033[2mo\033[0m";
				ex3: continue;
			}
			std::cout << std::endl;
		}

		std::cout << std::endl << std::endl;

		for (int i = 0; i < gr.size(); i++) {
			for (int j = 0; j < gr[0].size(); j++) {
				for (auto cell : path) {
					if (cell.first == i && cell.second == j) {
						std::cout << "\033[31;1mx\033[0m";
						goto ex4;
					}
				}

				std::cout << "\033[2mo\033[0m";
				ex4: continue;
			}

			std::cout << std::endl;
		}

	}

	std::cout << std::endl << std::endl;


	//bigint
	auto int1 = algo::bigint::BigInt("100000000000");
	auto int2 = algo::bigint::BigInt("100000000");

	if (int1 == int2) std::cout << "Equal" << std::endl;
	else std::cout << "Not equal" << std::endl;

	std::cout << (int1 + int2).to_string() << std::endl;

	std::cout << (int1 - int2).to_string() << std::endl;

	std::cout << (int1 * int2).to_string() << std::endl;

	std::cout << (int1 / int2).to_string() << std::endl;

	std::cout << (int1 % int2).to_string() << std::endl;

	return 0;
}
