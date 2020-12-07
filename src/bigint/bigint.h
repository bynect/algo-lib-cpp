#ifndef ALGO_BIGINT_H
#define ALGO_BIGINT_H


#include <vector>
#include <cstdint>
#include <string>


namespace algo::bigint
{
	class BigInt {
	public:
		BigInt(std::string &string)
		: repr(from_string(string))
		{}

		BigInt(const char *c_string)
		: repr(from_string(c_string))
		{}

		BigInt(char *c_string)
		: repr(from_string(c_string))
		{}

		BigInt(const std::vector<uint32_t> &array, bool negative)
		: repr(array), negative(negative)
		{}

		BigInt()
		{}

		BigInt operator+(const BigInt &op);
		BigInt operator-(const BigInt &op);
		BigInt operator*(const BigInt &op);
		BigInt operator/(const BigInt &op);
		BigInt operator%(const BigInt &op);
		bool operator==(const BigInt &op);


		std::string to_string();

		bool
		empty()
		{
			return repr.size() == 0;
		}

		const std::vector<uint32_t>
		internal_repr()
		{
			return repr;
		}

	private:
		std::vector<uint32_t> repr;
		bool negative;

		std::vector<uint32_t> from_string(const char *c_string);

		std::vector<uint32_t>
		from_string(std::string &string)
		{
			return from_string(string.c_str());
		}
	};
}


#endif
