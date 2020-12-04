#include <stack>
#include <stdexcept>
#include <cstring>
#include <cfloat>
#include <set>

#include "a_star.h"


bool
algo::search::a_star::search_path(node src, node dest, std::vector<algo::search::node> &path)
{
	if (!is_valid(src.first, src.second))
		throw std::invalid_argument("Invalid source node.");

	if (!is_valid(dest.first, dest.second))
		throw std::invalid_argument("Invalid destination node.");

	if (!is_unblocked(src.first, src.second)
		|| !is_unblocked(dest.first, dest.second))
		throw std::invalid_argument("Blocked node.");

	if (is_destination(src.first, src.second, dest))
		throw std::invalid_argument("Alredy at destination.");

	bool closed[col_][row_];
	std::memset(closed, false, sizeof(closed));

	algo::search::cell details[col_][row_];

	int i, j;
	for (i = 0; i < row_; i++) {
		for (j = 0; j < col_; j++) {
			details[i][j].f = FLT_MAX;
			details[i][j].g = FLT_MAX;
			details[i][j].h = FLT_MAX;
			details[i][j].parent_i = -1;
			details[i][j].parent_j = -1;
		}
	}

	i = src.first;
	j = src.second;
	details[i][j].f = 0.0;
	details[i][j].g = 0.0;
	details[i][j].h = 0.0;
	details[i][j].parent_i = i;
	details[i][j].parent_j = j;

	std::set<std::pair<double, algo::search::node>> open;
	open.insert(std::make_pair(0.0, std::make_pair(i, j)));

	bool found = false;

	while (!open.empty()) {
		std::pair<double, algo::search::node> p = *open.begin();
		open.erase(open.begin());

		i = p.second.first;
		j = p.second.second;
		closed[i][j] = true;

		double g, h, f;

		if (is_valid(i - 1, j)) {
			if (is_destination(i - 1, j, dest)) {
				details[i - 1][j].parent_i = i;
				details[i - 1][j].parent_j = j;

				algo::search::grid<cell> vec;
				for (int i = 0; i < row_; i++) {
					std::vector<cell> elem;
					for (int j = 0; j < col_; j++) {
						elem.push_back(details[i][j]);
					}
					vec.push_back(elem);
				}

				found = true;
				path = trace_path(vec, dest);
				break;

			} else if (!closed[i - 1][j] && is_unblocked(i - 1, j)) {
				g = details[i][j].g + 1.0;
				h = approximate_h (i - 1, j, dest);
				f = g + h;

				if (details[i - 1][j].f == FLT_MAX || details[i - 1][j].f > f) {
					open.insert(std::make_pair(f, std::make_pair(i - 1, j)));
					details[i - 1][j].f = f;
					details[i - 1][j].g = g;
					details[i - 1][j].h = h;
					details[i - 1][j].parent_i = i;
					details[i - 1][j].parent_j = j;
				}
			}
		}

		if (is_valid(i + 1, j)) {
			if (is_destination(i + 1, j, dest)) {
				details[i + 1][j].parent_i = i;
				details[i + 1][j].parent_j = j;

				algo::search::grid<cell> vec;
				for (int i = 0; i < row_; i++) {
					std::vector<cell> elem;
					for (int j = 0; j < col_; j++) {
						elem.push_back(details[i][j]);
					}
					vec.push_back(elem);
				}

				found = true;
				path = trace_path(vec, dest);
				break;

			} else if (!closed[i + 1][j] && is_unblocked(i + 1, j)) {
				g = details[i][j].g + 1.0;
				h = approximate_h(i + 1, j, dest);
				f = g + h;

				if (details[i + 1][j].f == FLT_MAX || details[i + 1][j].f > f) {
					open.insert(std::make_pair (f, std::make_pair (i + 1, j)));
					details[i + 1][j].f = f;
					details[i + 1][j].g = g;
					details[i + 1][j].h = h;
					details[i + 1][j].parent_i = i;
					details[i + 1][j].parent_j = j;
				}
			}
		}

		if (is_valid (i, j + 1)) {
			if (is_destination(i, j + 1, dest)) {
				details[i][j + 1].parent_i = i;
				details[i][j + 1].parent_j = j;

				algo::search::grid<cell> vec;
				for (int i = 0; i < row_; i++) {
					std::vector<cell> elem;
					for (int j = 0; j < col_; j++) {
						elem.push_back(details[i][j]);
					}
					vec.push_back(elem);
				}

				found = true;
				path = trace_path(vec, dest);
				break;

			} else if (!closed[i][j + 1] && is_unblocked (i, j + 1)) {
				g = details[i][j].g + 1.0;
				h = approximate_h(i, j + 1, dest);
				f = g + h;

				if (details[i][j + 1].f == FLT_MAX || details[i][j + 1].f > f) {
					open.insert(std::make_pair(f, std::make_pair (i, j + 1)));
					details[i][j + 1].f = f;
					details[i][j + 1].g = g;
					details[i][j + 1].h = h;
					details[i][j + 1].parent_i = i;
					details[i][j + 1].parent_j = j;
				}
			}
		}

		if (is_valid(i, j - 1)) {
			if (is_destination(i, j - 1, dest)) {
				details[i][j - 1].parent_i = i;
				details[i][j - 1].parent_j = j;

				algo::search::grid<cell> vec;
				for (int i = 0; i < row_; i++) {
					std::vector<cell> elem;
					for (int j = 0; j < col_; j++) {
						elem.push_back(details[i][j]);
					}
					vec.push_back(elem);
				}

				found = true;
				path = trace_path(vec, dest);
				break;

			} else if (!closed[i][j - 1] && is_unblocked(i, j - 1)) {
				g = details[i][j].g + 1.0;
				h = approximate_h(i, j - 1, dest);
				f = g + h;

				if (details[i][j - 1].f == FLT_MAX || details[i][j - 1].f > f) {
					open.insert(std::make_pair (f, std::make_pair (i, j - 1)));
					details[i][j - 1].f = f;
					details[i][j - 1].g = g;
					details[i][j - 1].h = h;
					details[i][j - 1].parent_i = i;
					details[i][j - 1].parent_j = j;
				}
			}
		}

		if (is_valid(i - 1, j + 1)) {
			if (is_destination(i - 1, j + 1, dest)) {
				details[i - 1][j + 1].parent_i = i;
				details[i - 1][j + 1].parent_j = j;

				algo::search::grid<cell> vec;
				for (int i = 0; i < row_; i++) {
					std::vector<cell> elem;
					for (int j = 0; j < col_; j++) {
						elem.push_back(details[i][j]);
					}
					vec.push_back(elem);
				}

				found = true;
				path = trace_path(vec, dest);
				break;

			} else if (!closed[i - 1][j + 1] && is_unblocked(i - 1, j + 1)) {
				g = details[i][j].g + 1.414;
				h = approximate_h(i - 1, j + 1, dest);
				f = g + h;

				if (details[i - 1][j + 1].f == FLT_MAX || details[i - 1][j + 1].f > f) {
					open.insert(std::make_pair (f, std::make_pair(i - 1, j + 1)));
					details[i - 1][j + 1].f = f;
					details[i - 1][j + 1].g = g;
					details[i - 1][j + 1].h = h;
					details[i - 1][j + 1].parent_i = i;
					details[i - 1][j + 1].parent_j = j;
				}
			}
		}

		if (is_valid (i - 1, j - 1)) {
			if (is_destination (i - 1, j - 1, dest)) {
				details[i - 1][j - 1].parent_i = i;
				details[i - 1][j - 1].parent_j = j;

				algo::search::grid<cell> vec;
				for (int i = 0; i < row_; i++) {
					std::vector<cell> elem;
					for (int j = 0; j < col_; j++) {
						elem.push_back(details[i][j]);
					}
					vec.push_back(elem);
				}

				found = true;
				path = trace_path(vec, dest);
				break;

			} else if (!closed[i - 1][j - 1] && is_unblocked(i - 1, j - 1)) {
				g = details[i][j].g + 1.414;
				h = approximate_h(i - 1, j - 1, dest);
				f = g + h;

				if (details[i - 1][j - 1].f == FLT_MAX || details[i - 1][j - 1].f > f) {
					open.insert(std::make_pair (f, std::make_pair (i - 1, j - 1)));
					details[i - 1][j - 1].f = f;
					details[i - 1][j - 1].g = g;
					details[i - 1][j - 1].h = h;
					details[i - 1][j - 1].parent_i = i;
					details[i - 1][j - 1].parent_j = j;
				}
			}
		}

		if (is_valid(i + 1, j + 1) )
		{
			if (is_destination(i + 1, j + 1, dest) )
			{
				details[i + 1][j + 1].parent_i = i;
				details[i + 1][j + 1].parent_j = j;

				algo::search::grid<cell> vec;
				for (int i = 0; i < row_; i++) {
					std::vector<cell> elem;
					for (int j = 0; j < col_; j++) {
						elem.push_back(details[i][j]);
					}
					vec.push_back(elem);
				}

				found = true;
				path = trace_path(vec, dest);
				break; trace_path(vec, dest);

			} else if (!closed[i + 1][j + 1] && is_unblocked(i + 1, j + 1)) {
				g = details[i][j].g + 1.414;
				h = approximate_h(i + 1, j + 1, dest);
				f = g + h;

				if (details[i + 1][j + 1].f == FLT_MAX || details[i + 1][j + 1].f > f) {
					open.insert(std::make_pair(f, std::make_pair (i + 1, j + 1)));
					details[i + 1][j + 1].f = f;
					details[i + 1][j + 1].g = g;
					details[i + 1][j + 1].h = h;
					details[i + 1][j + 1].parent_i = i;
					details[i + 1][j + 1].parent_j = j;
				}
			}
		}

		if (is_valid (i + 1, j - 1)) {
			if (is_destination(i + 1, j - 1, dest)) {
				details[i + 1][j - 1].parent_i = i;
				details[i + 1][j - 1].parent_j = j;

				algo::search::grid<cell> vec;
				for (int i = 0; i < row_; i++) {
					std::vector<cell> elem;
					for (int j = 0; j < col_; j++) {
						elem.push_back(details[i][j]);
					}
					vec.push_back(elem);
				}

				found = true;
				path = trace_path(vec, dest);
				break;

			} else if (!closed[i + 1][j - 1] && is_unblocked(i + 1, j - 1) ) {
				g = details[i][j].g + 1.414;
				h = approximate_h(i + 1, j - 1, dest);
				f = g + h;

				if (details[i + 1][j - 1].f == FLT_MAX || details[i + 1][j - 1].f > f) {
					open.insert(std::make_pair(f, std::make_pair(i + 1, j - 1)));
					details[i + 1][j - 1].f = f;
					details[i + 1][j - 1].g = g;
					details[i + 1][j - 1].h = h;
					details[i + 1][j - 1].parent_i = i;
					details[i + 1][j - 1].parent_j = j;
				}
			}
		}
	}

	return found;
}


std::vector<algo::search::node>
algo::search::a_star::trace_path(algo::search::grid<algo::search::cell> details,
	algo::search::node dest)
{
	int row = dest.first;
	int col = dest.second;

	std::stack<algo::search::node> path;

	while (!(details[row][col].parent_i == row
		&& details[row][col].parent_j == col)) {

		path.push(std::make_pair(row, col));
		int temp_row = details[row][col].parent_i;
		int temp_col = details[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	path.push(std::make_pair(row, col));

	std::vector<algo::search::node> vec;

	while (!path.empty()) {
		vec.push_back(path.top());
		path.pop();
	}

	return vec;
}
