#include <stdexcept>
#include <cstring>

#include "bigint.h"


#define assert(x) if (!(x)) throw std::invalid_argument(#x)


static void
algorithm_a(int n, const uint32_t *u, const uint32_t *v, uint32_t *w)
{
	bool carry, carry_a, carry_b;
	uint32_t sum_a, sum_b;
	int j;

	carry = false;

	for (j = 0; j < n; j++) {
		sum_a = u[j] + carry;
		carry_a = (sum_a < u[j]);

		sum_b = sum_a + v[j];
		carry_b = (sum_b < sum_a);

		w[j] = sum_b;
		assert(carry_a + carry_b <= 1);
		carry = carry_a + carry_b;
	}

	w[j] = carry;
}


static int
cmp(int u_len, const uint32_t *u, int v_len, const uint32_t *v)
{
	int i;

	if (u_len != v_len) {
		return u_len < v_len ? -1 : 1;
	}

	for (i = u_len - 1; i >= 0; i--) {
		if (u[i] != v[i]) {
			return u[i] < v[i] ? -1 : 1;
		}
	}

	return 0;
}


static void
algorithm_s(int n, const uint32_t *u, const uint32_t *v, uint32_t *w)
{
	bool borrow, borrow_a, borrow_b;
	uint32_t diff_a, diff_b;
	int j;

	assert(cmp(n, u, n, v) >= 0 && "Subtraction result would be negative!");

	borrow = false;

	for (j = 0; j < n; j++) {
		diff_a = u[j] - borrow;
		borrow_a = (diff_a > u[j]);

		diff_b = diff_a - v[j];
		borrow_b = (diff_b > diff_a);

		w[j] = diff_b;
		assert(borrow_a + borrow_b <= 1);
		borrow = borrow_a + borrow_b;
	}

	assert(!borrow && "Nothing to borrow from!");
}


static void
mul_32_by_32(uint32_t x, uint32_t y, uint32_t *z_hi, uint32_t *z_lo)
{
	uint64_t prod;

	prod = (uint64_t)x * y;

	*z_hi = (uint32_t)(prod >> 32);
	*z_lo = (uint32_t)prod;
}


static void
algorithm_m(int m, int n, const uint32_t *u, const uint32_t *v, uint32_t *w)
{
	int i, j;
	uint32_t k, hi_prod, lo_prod;
	bool carry_a, carry_b;

	for (i = 0; i < m; i++) {
		w[i] = 0;
	}

	for (j = 0; j < n; j++) {
		if (v[j] == 0) {
			w[j + m] = 0;
			continue;
		}

		k = 0;
		for (i = 0; i < m; i++) {
			mul_32_by_32(u[i], v[j], &hi_prod, &lo_prod);

			lo_prod += k;
			carry_a = (lo_prod < k);

			w[i + j] += lo_prod;
			carry_b = (w[i + j] < lo_prod);

			k = hi_prod + carry_a + carry_b;
			assert(k >= hi_prod && "k cannot overflow");
		}

		w[j + m] = k;
	}
}


static void
div_32_by_16(uint16_t u_hi, uint16_t u_lo, uint16_t v, uint16_t *q, uint16_t *r)
{
	uint32_t u = ((uint32_t)u_hi << 16) | u_lo;

	assert(v > 0 && "Division by zero!");
	assert(u / v <= UINT16_MAX && "Division overflow!");

	*q = (uint16_t)(u / v);
	*r = (uint16_t)(u % v);
}


static void
short_division(int n, uint16_t *u, uint16_t v, uint16_t *q, uint16_t *r)
{
	uint16_t k;
	int i;

	assert(v > 0 && "Division by zero!");
	assert(n > 0 && "Dividing empty number!");

	k = 0;
	for (i = n - 1; i >= 0; i--) {
		div_32_by_16(k, u[i], v, &q[i], &k);
	}
	*r = k;
}


static int
leading_zeros(uint16_t x)
{
	int n;

	assert(x != 0);

	n = 0;
	while (x <= UINT16_MAX / 2) {
		x <<= 1;
		n++;
	}

	return n;
}


static void
shift_left(int n, uint16_t *u, int m)
{
	uint16_t k, t;
	int i;

	assert(m > 0);
	assert(m < 16);

	k = 0;
	for (i = 0; i < n; i++) {
		t = u[i] >> (16 - m);
		u[i] = (u[i] << m) | k;
		k = t;
	}

	assert(k == 0 && "Leftover carry!");
}


static void
shift_right(int n, uint16_t *u, int m)
{
	uint16_t k, t;
	int i;

	assert(m > 0);
	assert(m < 16);

	k = 0;
	for (i = n - 1; i >= 0; i--) {
		t = u[i] << (16 - m);
		u[i] = (u[i] >> m) | k;
		k = t;
	}

	assert(k == 0 && "Leftover carry!");
}


static void
algorithm_d(int m, int n, uint16_t *u, uint16_t *v, uint16_t *q)
{
	int shift;
	int j, i;
	uint32_t qhat, rhat, p, t;
	uint16_t k, k2, d;

	assert(n > 0 && "v must be greater than zero!");
	assert(v[n - 1] != 0 && "v must not have leading zeros!");

	if (n == 1) {
		short_division(m + n, u, v[0], q, &u[0]);
		return;
	}

	u[m + n] = 0;
	shift = leading_zeros(v[n-1]);
	if (shift) {
			shift_left(n, v, shift);
			shift_left(m + n + 1, u, shift);
	}

	for (j = m; j >= 0; j--) {
		t = ((uint32_t)u[j + n] << 16) | u[j + n - 1];
		qhat = t / v[n - 1];
		rhat = t % v[n - 1];

		while (true) {
			assert(n >= 2);
			if (qhat > UINT16_MAX || qhat * v[n - 2] > ((rhat << 16) | u[j + n - 2])) {
				qhat--;
				rhat += v[n - 1];
				if (rhat <= UINT16_MAX) {
					continue;
				}
			}
			break;
		}

		k = 0;
		for (i = 0; i <= n; i++) {
			p = qhat * (i == n ? 0 : v[i]);
			k2 = (p >> 16);

			d = u[j + i] - (uint16_t)p;
			k2 += (d > u[j + i]);

			u[j + i] = d - k;
			k2 += (u[j + i] > d);

			k = k2;
		}

		q[j] = qhat;
		if (k != 0) {
			q[j]--;
			k = 0;
			for (i = 0; i < n; i++) {
				t = u[j + i] + v[i] + k;
				u[j + i] = (uint16_t)t;
				k = t >> 16;
			}
			u[j + n] += k;
		}
	}

	if (shift) {
		shift_right(n, u, shift);
	}
}


static void
u32_to_u16(int n, const uint32_t *u32, uint16_t *u16)
{
	int i;

	for (i = 0; i < n; i++) {
		u16[i * 2 + 0] = (uint16_t)u32[i];
		u16[i * 2 + 1] = (uint16_t)(u32[i] >> 16);
	}
}


static void
u16_to_u32(int n, const uint16_t *u16, uint32_t *u32)
{
	int i;

	assert((n % 2) == 0 && "Expected n to be even!");

	for (i = 0; i < n; i += 2) {
		u32[i / 2] = u16[i] | ((uint32_t)u16[i + 1] << 16);
	}
}


static void
algorithm_d_wrapper(int m, int n, const uint32_t *u, const uint32_t *v, uint32_t *q, uint32_t *r)
{
	uint16_t u16[(m + n) * 2 + 1];
	uint16_t v16[n * 2];
	uint16_t q16[(m + 1) * 2];

	assert(n > 0 && "Division by zero!");
	assert(v[n - 1] != 0 && "v has leading zero!");

	u32_to_u16(m + n, u, u16);
	u32_to_u16(n, v, v16);

	bool v_zero = (v16[n * 2 - 1] == 0);

	algorithm_d(m * 2 + v_zero, n * 2 - v_zero, u16, v16, q16);

	if (v_zero)
		u16[n * 2 - 1] = 0;
	else
		q16[(m + 1) * 2 - 1] = 0;

	u16_to_u32((m + 1) * 2, q16, q);
	u16_to_u32(n * 2, u16, r);
}


static void
multiply_add(uint32_t *u, int *m, uint32_t x, uint32_t y)
{
	int i;
	uint32_t k, hi_prod, lo_prod;

	k = y;

	for (i = 0; i < *m; i++) {
		mul_32_by_32(u[i], x, &hi_prod, &lo_prod);
		lo_prod += k;
		k = hi_prod + (lo_prod < k);
		u[i] = lo_prod;
	}

	if (k) {
		u[*m] = k;
		(*m)++;
	}
}


static void
bigint_from_string(int n, const char *str, int *u_len, uint32_t *u)
{
	uint32_t chunk;
	int i;

	static const uint32_t pow10s[] = {
		1000000000,
		10,
		100,
		1000,
		10000,
		100000,
		1000000,
		10000000,
		100000000
	};

	*u_len = 0;
	chunk = 0;

	for (i = 1; i <= n; i++) {
		assert(*str >= '0' && *str <= '9');
		chunk = chunk * 10 + *str++ - '0';

		if (i % 9 == 0 || i == n) {
			multiply_add(u, u_len, pow10s[i % 9], chunk);
			chunk = 0;
		}
	}
}


static void
bigint_to_string(int n, const uint32_t *u, char *str)
{
	uint16_t v[n * 2];
	uint16_t k;
	char *s, t;
	int i;

	u32_to_u16(n, u, v);
	n *= 2;

	while (n && v[n - 1] == 0) n--;

	if (n == 0) {
		str[0] = '0';
		str[1] = '\0';
		return;
	}

	s = str;
	while (n != 0) {
		short_division(n, v, 10000, v, &k);
		while (n && v[n - 1] == 0) {
			n--;
		}

		for (i = 0; (n != 0 && i < 4) || k; i++) {
			*s++ = '0' + (k % 10);
			k /= 10;
		}
	}

	*s-- = '\0';
	while (str < s) {
		t = *str;
		*str++ = *s;
		*s-- = t;
	}
}


static algo::bigint::BigInt
bigint_add(int x_len, const uint32_t *x, int y_len, const uint32_t *y)
{
	if (x_len < y_len) {
		return bigint_add(y_len, y, x_len, x);
	}

	int w_len = x_len + 1;
	uint32_t w[w_len];
	int i;

	assert(x_len >= y_len);

	for (i = 0; i < y_len; i++) {
		w[i] = y[i];
	}
	for (i = y_len; i < x_len; i++) {
		w[i] = 0;
	}

	algorithm_a(x_len, x, w, w);

	return algo::bigint::BigInt(std::vector<uint32_t>(w, w + w_len), false);
}


static algo::bigint::BigInt
bigint_sub(int x_len, const uint32_t *x, int y_len, const uint32_t *y)
{
	if (cmp(x_len, x, y_len, y) < 0)
		return algo::bigint::BigInt(bigint_sub(y_len, y, x_len, x).internal_repr(), true);

	uint32_t w[x_len];
	int i;

	assert(x_len >= y_len);

	for (i = 0; i < y_len; i++) {
		w[i] = y[i];
	}
	for (i = y_len; i < x_len; i++) {
		w[i] = 0;
	}

	algorithm_s(x_len, x, w, w);

	return algo::bigint::BigInt(std::vector<uint32_t>(w, w + x_len), false);
}


static algo::bigint::BigInt
bigint_divrem(int x_len, const uint32_t *x, int y_len, const uint32_t *y, bool remainder)
{
	uint32_t q[x_len - y_len + 1];
	uint32_t r[y_len];

	assert(x_len >= y_len);

	algorithm_d_wrapper(x_len - y_len, y_len, x, y, q, r);

	if (remainder) {
		return algo::bigint::BigInt(std::vector<uint32_t>(r, r + y_len), false);
	}

	return algo::bigint::BigInt(std::vector<uint32_t>(q, q + (x_len - y_len + 1)), false);
}


std::vector<uint32_t>
algo::bigint::BigInt::from_string(const char *string)
{
	int n = std::strlen(string);

	uint32_t u[n / 9 + 1];
	bool negative = false;
	int u_length;

	assert(n > 0 && "Empty string is not a valid number.");

	if (string[0] == '-') {
		assert(n > 1 && "Just '-' is not a valid number.");
		negative = true;
		string++;
		n--;
	}

	bigint_from_string(n, string, &u_length, u);

	return std::vector<uint32_t>(u, u + u_length);
}


std::string
algo::bigint::BigInt::to_string()
{
	char buff[repr.size() + 1];

	uint32_t arr[repr.size()];

	for (int i = 0; i < repr.size(); i++) {
		arr[i] = repr[i];
	}

	if (negative) {
		buff[0] = '-';
		bigint_to_string(repr.size(), arr, buff + 1);
	} else {
		bigint_to_string(repr.size(), arr, buff);
	}

	return std::string(buff);
}


algo::bigint::BigInt
algo::bigint::BigInt::operator+(const BigInt &op)
{
	uint32_t x[repr.size()];
	uint32_t y[op.repr.size()];

	for (int i = 0; i < op.repr.size(); i++) {
		y[i] = op.repr[i];
	}

	for (int i = 0; i < repr.size(); i++) {
		x[i] = repr[i];
	}

	if (negative && op.negative) {
		return algo::bigint::BigInt(bigint_add(repr.size(), x, op.repr.size(), y).internal_repr(), true);
	}

	if (negative) {
		assert(!op.negative);
		return bigint_sub( op.repr.size(), y, repr.size(), x);
	}

	if (op.negative) {
		assert(!negative);
		return bigint_sub(repr.size(), x, op.repr.size(), y);
	}

	assert(!negative && !op.negative);

	return bigint_add(repr.size(), x, op.repr.size(), y);
}


algo::bigint::BigInt
algo::bigint::BigInt::operator-(const BigInt &op)
{
	uint32_t x[repr.size()];
	uint32_t y[op.repr.size()];

	for (int i = 0; i < op.repr.size(); i++) {
		y[i] = op.repr[i];
	}

	for (int i = 0; i < repr.size(); i++) {
		x[i] = repr[i];
	}

	if (negative && op.negative) {
		return bigint_sub(op.repr.size(), y, repr.size(), x);
	}

	if (negative) {
		assert(!op.negative);
		return algo::bigint::BigInt(bigint_add(repr.size(), x, op.repr.size(), y).internal_repr(), true);
	}

	if (op.negative) {
		assert(!negative);
		return bigint_add(repr.size(), x, op.repr.size(), y);
	}

	assert(!negative && !op.negative);

	return bigint_sub(repr.size(), x, op.repr.size(), y);
}


algo::bigint::BigInt
algo::bigint::BigInt::operator*(const BigInt &op)
{
	uint32_t x[repr.size()];
	uint32_t y[op.repr.size()];

	for (int i = 0; i < op.repr.size(); i++) {
		y[i] = op.repr[i];
	}

	for (int i = 0; i < repr.size(); i++) {
		x[i] = repr[i];
	}

	uint32_t w[repr.size() + op.repr.size()];

	algorithm_m(repr.size(), op.repr.size(), x, y, w);

	return algo::bigint::BigInt(
		std::vector<uint32_t>(w, w + (repr.size() + op.repr.size())),
		negative ^ op.negative
	);
}


algo::bigint::BigInt
algo::bigint::BigInt::operator/(const BigInt &op)
{
	uint32_t x[repr.size()];
	uint32_t y[op.repr.size()];

	for (int i = 0; i < op.repr.size(); i++) {
		y[i] = op.repr[i];
	}

	for (int i = 0; i < repr.size(); i++) {
		x[i] = repr[i];
	}

	if (repr.size() < op.repr.size()) {
		throw std::invalid_argument("Invalid division");
	}

	return algo::bigint::BigInt(
		bigint_divrem(repr.size(), x, op.repr.size(), y, false).internal_repr(),
		negative ^ op.negative
	);
}


algo::bigint::BigInt
algo::bigint::BigInt::operator%(const BigInt &op)
{
	uint32_t x[repr.size()];
	uint32_t y[op.repr.size()];

	for (int i = 0; i < op.repr.size(); i++) {
		y[i] = op.repr[i];
	}

	for (int i = 0; i < repr.size(); i++) {
		x[i] = repr[i];
	}

	if (repr.size() < op.repr.size()) {
		return algo::bigint::BigInt(repr, negative);
	} else {
		return algo::bigint::BigInt(
			bigint_divrem(repr.size(), x, op.repr.size(), y, true).internal_repr(),
			negative
		);
	}
}


bool
algo::bigint::BigInt::operator==(const BigInt &op)
{
	uint32_t buff1[repr.size()];
	uint32_t buff2[op.repr.size()];

	for (int i = 0; i < op.repr.size(); i++) {
		buff2[i] = op.repr[i];
	}

	for (int i = 0; i < repr.size(); i++) {
		buff1[i] = repr[i];
	}

	auto diff = cmp(repr.size(), buff1, op.repr.size(), buff2);
	return diff == 0;
}
