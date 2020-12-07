#ifndef ALGO_A_STAR_H
#define ALGO_A_STAR_H


#include <utility>
#include <vector>
#include <cmath>
#include <array>


namespace algo::search
{
	typedef std::pair<int, int> node;

	template <typename T>
	using grid = std::vector<std::vector<T>>;


	typedef struct {
		int parent_i, parent_j;
		double f, g, h;
	} cell;


	class AStar {
	public:
		AStar(grid<int> grid_)
		: grid_(grid_), row_(grid_.size()), col_(grid_[0].size())
		{}

		bool search_path(node src, node dest, std::vector<algo::search::node> &path);

	private:
		grid<int> grid_;
		int row_, col_;

		std::vector<node> trace_path(grid<cell> &details, node dest);

		bool
		is_valid(int row, int col)
		{
			return (row >= 0) && (row < row_)
				&& (col >= 0) && (col < col_);
		}

		bool
		is_unblocked(int row, int col)
		{
			return (grid_[row][col] == 1);
		}

		bool
		is_destination(int row, int col, node dest)
		{
			return (row == dest.first)
				&& (col == dest.second);
		}

		double
		approximate_h(int row, int col, node dest)
		{
			return std::sqrt(
				(row - dest.first) * (row - dest.first)
				+ (col - dest.second) * (col - dest.second)
			);
		}
	};
}


#endif
