// nimber - GF[2^2^k] as described by J.H. Conway
// David Eppstein, Columbia University, 19 Mar 1988
//
// All multiplicative operations take time (gates) O(n^(log 3) log n)
// where n=2^k.  But this is not good enough to compete with implementations
// of GF[2^n] based on irreducible polynomials and FFT multiplication,
// which take time (gates) O(n polylog(n)) -- the hard part is reducing
// the 2n-bit result of a multiply to n bits, which can be done in log n
// stages, each stage using a multiply to half the number of excess bits.


#include "nimber.h"


// high_part and low_part - split nimber into half sets of bits
// combine - put the halves back together again

static inline proto_nimber exp(int level) { return 1 << level; }
static inline proto_nimber mask(int level) { return exp(exp(level)) - 1; }

static inline proto_nimber
high_part(proto_nimber x, int level) { return x >> exp(level); }

static inline proto_nimber
low_part(proto_nimber x, int level) { return x & mask(level); }

static inline proto_nimber
combine(proto_nimber x, proto_nimber y, int level)
{
    return (x << exp(level)) ^ y; // ^ not + so compiler has more opt chances
}



// level() - first level at which high_part is zero

static int
level(proto_nimber x)
{
    for (int i = 0; high_part(x,i) != 0; i++) ;
    return i;
}




// half_mult - multiply by exp(exp(level) - 1)
// 
// This is faster than general multiplication because of the simple
// structure of the multiplicand -- its high_part is the same structure
// at the next level down, and its low_part is zero.
//
// Time T(n) = O(n) + 3 T(n/2) = O(n^(log 3)).
//
// Can be done in O(n) bit vector ops by combining precomputed results
// for each bit, but that would be O(n^2) bit ops.

static proto_nimber
half_mult(proto_nimber x, int level)
{
    if (--level < 0) return x;
    proto_nimber high = high_part(x,level);
    proto_nimber low = low_part(x,level);
    return combine(half_mult(high ^ low, level),
		   half_mult(half_mult(high, level), level),
		   level);
}




// square - sped up from general multiply because (a+b)^2 = a^2 + b^2
// Time T(n) = O(n^(log 3)) + 2 T(n/2) = O(n^(log 3)).

static proto_nimber
nim_square(proto_nimber x, int level)
{
    if (--level < 0) return x;
    proto_nimber high = high_part(x, level);
    proto_nimber low = low_part(x, level);
    proto_nimber high_square = nim_square(high, level);
    return combine(high_square,
		   half_mult(high_square, level) ^ nim_square(low, level),
		   level);
}

nimber
nimber::square()
{
    return nimber(nim_square(value(), level(value())));
}




// square root
// 
// Correctness can be seen by comparing this code to that for square().
// Note that all nimbers have square roots (proof: this code).
// Time T(n) = O(n^(log 3)), same as square().

static proto_nimber
nim_sqrt(proto_nimber x, int level)
{

    if (--level < 0) return x;
    proto_nimber high = high_part(x, level);
    proto_nimber low = low_part(x, level);
    return combine(nim_sqrt(high, level),
		  nim_sqrt(half_mult(high,level) ^ low, level),
		  level);
}

nimber
nimber::sqrt()
{
    return nimber(nim_sqrt(value(), level(value())));
}




// multiplication of nimbers
//
// (ah,al)(bh,bl)
//     = (ah,0)(bh,0) + (ah,0)(0,bl) + (0,al)(bh,0) + (0,al)(0,bl)
//     = (ah bh, half_mult(ah bh)) + (ah bl,0) + (al bh,0) + (0,al bl)
//     = (ah bh + ah bl + al bh, half_mult(ah bh) + al bl)
//     = ((ah + al)(bh + bl) + al bl, half_mult(ah bh) + al bl)
//
// Time T(n) = O(n^(log 3)) + 3 T(n/2) = O(n^(log 3) log n).

static proto_nimber
nim_times(proto_nimber x, proto_nimber y, int level)
{
    if (--level < 0) return x & y;
	// single bit multiplication is Boolean conjunction

    proto_nimber x_high = high_part(x, level);
    proto_nimber x_low = low_part(x, level);
    proto_nimber y_high = high_part(y, level);
    proto_nimber y_low = low_part(y, level);

    proto_nimber low_mult = nim_times(x_low, y_low, level);
    return combine(nim_times(x_high ^ x_low, y_high ^ y_low, level) ^ low_mult,
		   half_mult(nim_times(x_high,y_high,level),level) ^ low_mult,
		   level);
}

nimber
nimber::operator* (nimber that)
{
    return nimber(nim_times(value(), that.value(),
			    level(value() | that.value())));
}





// multiplicative inverse
//
// If we treat nimbers as column vectors of their high and low parts, then
// the product (ah,al) (bh,bl) can be found as the matrix multiplication
//
//         ( ah + al         ah )  ( bh )
//         ( half_mult(ah)   al )  ( bl )
//
// Multiplication forms a group, so the matrix is non-singular.  The inverse
// of (ah,al) is found by inverting the matrix, then multiplying by (0,1).
// When (ah,al) == 0 we return zero for lack of a better value.
// 
// Time T(n) = O(n^(log 3) log n) + T(n/2) = O(n^(log 3) log n).

static proto_nimber nim_inverse(proto_nimber x, int level)
{
    if (--level < 0) return x;

    proto_nimber high = high_part(x, level);
    proto_nimber low = low_part(x, level);

    proto_nimber ad = nim_times(high ^ low, low, level);
    proto_nimber bc = half_mult(nim_square(high, level), level);
    proto_nimber inv_det = nim_inverse(ad ^ bc, level);
    return combine(nim_times(high, inv_det, level),
		   nim_times(high ^ low, inv_det, level),
		   level);
}

nimber
nimber::inverse()
{
    return nimber(nim_inverse(value(), level(value())));
}
