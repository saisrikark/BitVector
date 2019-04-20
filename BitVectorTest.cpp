/*
 * Test suite for the BitVector class
 */
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "BitVector.h"

using namespace std;

static const size_t s_sizes[] = {
       0,   1,   2,
       3,   4,   5,
       7,   8,   9,
      15,  16,  17,
      31,  32,  33,
      63,  64,  65,
     127, 128, 129,
     191, 192, 193,
     255, 256, 257,
    1000
};
static const size_t s_nsizes = sizeof(s_sizes) / sizeof(s_sizes[0]);


/***** BEGIN helper string routines *****/

static string get_random_bit_string(size_t nbits) {
    string str;
    srand(time(0));
    for (int i = 0; i < (int)nbits; i++) {
        str += (rand() < RAND_MAX / 2) ? '0' : '1';
    }
    return str;
}

static size_t count_ones(const string &str) {
    size_t count = 0;

    for (string::const_iterator it = str.begin(); it != str.end(); it++)
        if (*it == '1')
            count++;

    return count;
}

// Return true if any character in the bit string str is '1'.
static bool any(const string &str) {
    for (string::const_iterator it = str.begin(); it != str.end(); it++)
        if (*it == '1')
            return true;

    return false;
}

// Return true if none of the characters in the bit string str is '1'.
static bool none(const string &str) {
    return !any(str);
}

// Return true if all of the characters in the bit string str are '1'.
static bool all(const string &str) {
    for (string::const_iterator it = str.begin(); it != str.end(); it++)
        if (*it == '0')
            return false;

    return true;
}

// "Shift" left the bit string str, filling zeroes from the right.
static string shift_left(const string &str, size_t n) {
    return str.substr(n) + string(n, '0');
}

// "Shift" right the bit string str, filling zeroes from the left.
static string shift_right(const string &str, size_t n) {
    size_t sz = str.size();
    return string(n, '0') + str.substr(0, sz - n);
}

// AND the two bit strings.
static string bitwise_and(const string &str1, const string &str2) {
    size_t sz = str1.size();
    string sand;
    sand.reserve(sz);
    for (int i = 0; i < (int)sz; i++) {
        sand += '0' + ((str1[i] - '0') & (str2[i] - '0'));
    }
    return sand;
}

// OR the two bit strings.
static string bitwise_or(const string &str1, const string &str2) {
    size_t sz = str1.size();
    string sor;
    sor.reserve(sz);
    for (int i = 0; i < (int)sz; i++) {
        sor += '0' + ((str1[i] - '0') | (str2[i] - '0'));
    }
    return sor;
}

// XOR the two bit strings.
static string bitwise_xor(const string &str1, const string &str2) {
    size_t sz = str1.size();
    string sxor;
    sxor.reserve(sz);
    for (int i = 0; i < (int)sz; i++) {
        sxor += '0' + ((str1[i] - '0') ^ (str2[i] - '0'));
    }
    return sxor;
}

// Invert the bit string.
static string bitwise_not(const string &str) {
    size_t sz = str.size();
    string snot;
    snot.reserve(sz);
    for (int i = 0; i < (int)sz; i++) {
        snot += '0' + ('1' - str[i]);
    }
    return snot;
}

/***** END helper string routines *****/

#define ASSERT_EQUAL(lhs, rhs) do {                                     \
    if ((lhs) != (rhs)) {                                               \
        cerr << __FILE__ << ":" << __LINE__ << ": " <<                  \
            __PRETTY_FUNCTION__ << ": Assertion `" << #lhs << " == " << \
            #rhs << "' failed." << endl;                                \
        cerr << "(" << #lhs << " = " << (lhs) << ", " << #rhs <<        \
            " = " << (rhs) << ")" << endl;                              \
        abort();                                                        \
    }                                                                   \
} while (0)

static void test_string_ctor(size_t nbits) {
    string str = get_random_bit_string(nbits);
    BitVector bv(str);
    ASSERT_EQUAL(bv.to_string(), str);
}

static void test_copy_ctor(size_t nbits) {
    string str = get_random_bit_string(nbits);
    BitVector bv1(str);
    BitVector bv2(bv1);
    ASSERT_EQUAL(bv1, bv2);
}

static void test_ctors(void) {
    int i;

    for (i = 0; i < (int)s_nsizes; i++){
        test_string_ctor(s_sizes[i]);
        }
    for (i = 0; i < (int)s_nsizes; i++)
        test_copy_ctor(s_sizes[i]);
}

static void test_get(const BitVector &bv, const string &str) {
    size_t sz = bv.size();
    int i;

    for (i = 0; i < (int)sz; i++) {
        ASSERT_EQUAL(bv.get(i), str[sz - 1 - i] - '0');
        ASSERT_EQUAL(bv[i], str[sz - 1 - i] - '0');
    }
}

static void test_set_pos(BitVector &bv, size_t pos) {
    size_t sz = bv.size();

    // Set the bit
    bv.set(pos);
    // NOTE: pos is counted from the end of the string.
    ASSERT_EQUAL(bv.to_string()[sz - 1 - pos], '1');
}

static void test_set_pos(BitVector &bv, size_t lo, size_t hi) {
    size_t sz = bv.size();
    // Set the bits
    bv.set(lo, hi);
    // NOTE: lo and hi are counted from the end of the string.
    ASSERT_EQUAL(bv.to_string().substr(sz - 1 - hi, hi - lo + 1), string(hi - lo + 1, '1'));
}

static void test_set(size_t sz) {
    BitVector bv(sz);
    string str;
    int i, j;
    
    // Set all bits
    bv.set();
    for (i = 0; i < (int)sz; i++) {
        ASSERT_EQUAL(bv.get(i), 1);
    }
    
    bv.clear();
    /*
    // Set bit at given position
    str = bv.to_string();
    bv.set(sz);        // Invalid position: should not change bv!
    ASSERT_EQUAL(bv.to_string(), str);

    for (i = 0; i < (int)sz; i++) {
        if ((i % 8 == 0) || (i % 8 == 1) || (i % 8 == 7))
            test_set_pos(bv, i);
    }
    */
    bv.clear();
        
    // Set bits in a range
    for (i = 0; i < (int)sz; i++) {
        if ((i % 8 == 0) || (i % 8 == 1) || (i % 8 == 7)) {
            for (j = i; j < (int)sz; j++) {
                if ((j % 8 == 0) || (j % 8 == 1) || (j % 8 == 7))
                    test_set_pos(bv, i, j);
            }
            bv.clear();
        }
    }
}

static void test_clear_pos(BitVector &bv, size_t pos) {
    size_t sz = bv.size();

    // Clear the bit
    bv.clear(pos);
    // NOTE: pos is counted from the end of the string.
    //ASSERT_EQUAL(bv.to_string()[sz - 1 - pos], '0');
}

static void test_clear_pos(BitVector &bv, size_t lo, size_t hi) {
    size_t sz = bv.size();

    // Clear the bits
    bv.clear(lo, hi);
    // NOTE: lo and hi are counted from the end of the string.

    ASSERT_EQUAL(bv.to_string().substr(sz - 1 - hi, hi - lo + 1), string(hi - lo + 1, '0'));
}

static void test_clear(size_t sz) {
    BitVector bv(sz);
    string str;
    int i, j;

    // Clear all bits
    bv.clear();
    for (i = 0; i < (int)sz; i++) {
        ASSERT_EQUAL(bv.get(i), 0);
    }
    bv.set();
    
    // Clear bit at given position
    str = bv.to_string();
    bv.clear(sz);        // Invalid position: should not change bv!
    ASSERT_EQUAL(bv.to_string(), str);

    for (i = 0; i < (int)sz; i++) {
        if ((i % 8 == 0) || (i % 8 == 1) || (i % 8 == 7))
            test_clear_pos(bv, i);
    }

    bv.set();

    // Clear bits in a range
    for (i = 0; i < (int)sz; i++) {
        if ((i % 8 == 0) || (i % 8 == 1) || (i % 8 == 7)) {
            for (j = i; j < (int)sz; j++) {
                if ((j % 8 == 0) || (j % 8 == 1) || (j % 8 == 7))
                    test_clear_pos(bv, i, j);
            }
            bv.set();
        }
    }
}
// NOTE: pos is counted from the end of the string.
static string &flip_bit(string &str, size_t pos) {
    size_t sz = str.size();
    if (str[sz - 1 - pos] == '0')
        str[sz - 1 - pos] = '1';
    else
        str[sz - 1 - pos] = '0';
    return str;
}

// NOTE: lo and hi are counted from the end of the string.
static string &flip_bits(string &str, size_t lo, size_t hi) {
    for (size_t pos = lo; pos <= hi; pos++)
        flip_bit(str, pos);
    return str;
}

static void test_flip_pos(BitVector &bv, const string &str, size_t pos) {
    // Flip the bit
    bv.flip(pos);
    string tstr = bv.to_string();
    ASSERT_EQUAL(flip_bit(tstr, pos), str);

    // Flip it back
    bv.flip(pos);
    ASSERT_EQUAL(bv.to_string(), str);
}

static void test_flip_pos(BitVector &bv, const string &str, size_t lo, size_t hi) {
    // Flip the bits
    bv.flip(lo, hi);
    string tstr = bv.to_string();
    ASSERT_EQUAL(flip_bits(tstr, lo, hi), str);

    // Flip 'em back
    bv.flip(lo, hi);
    ASSERT_EQUAL(bv.to_string(), str);
}

static void test_flip(BitVector &bv, const string &str) {
    size_t sz = bv.size();
    int i, j;

    // Flip all bits
    bv.flip();
    for (i = 0; i < (int)sz; i++) {
        ASSERT_EQUAL(bv.get(i), '1' - str[sz - 1 - i]);
    }
    bv.flip();
    ASSERT_EQUAL(bv.to_string(), str);

    // Flip bit at given position
    bv.flip(sz);        // Invalid position: should not change bv!
    ASSERT_EQUAL(bv.to_string(), str);

    for (i = 0; i < (int)sz; i++) {
        if ((i % 8 == 0) || (i % 8 == 1) || (i % 8 == 7))
            test_flip_pos(bv, str, i);
    }

    // Flip bits in a range
    for (i = 0; i < (int)sz; i++) {
        if ((i % 8 == 0) || (i % 8 == 1) || (i % 8 == 7)) {
            for (j = i; j < (int)sz; j++) {
                if ((j % 8 == 0) || (j % 8 == 1) || (j % 8 == 7))
                    test_flip_pos(bv, str, i, j);
            }
        }
    }
}

static void test_count(const BitVector &bv, const string &str) {
    size_t count = bv.count();
    ASSERT_EQUAL(count, count_ones(str));
}

static void test_predicates(size_t sz) {
    string s0(sz, '0'); // String of all zeroes
    BitVector bv0(s0);

    string s1(sz, '1'); // String of all ones
    BitVector bv1(s1);

    string s2;
    s2.reserve(sz);
    int bit = 1;
    for (int i = 0; i < (int)sz; i++) {
        s2 += '0' + bit;
        bit = !bit;
    }
    BitVector bv2(s2);

    ASSERT_EQUAL(bv0.any(), false);
    ASSERT_EQUAL((sz == 0) || bv1.any(), true);
    ASSERT_EQUAL(bv2.any(), any(s2));

    ASSERT_EQUAL(bv0.none(), true);
    ASSERT_EQUAL((sz == 0) || !bv1.none(), true);
    ASSERT_EQUAL(bv2.none(), none(s2));

    ASSERT_EQUAL((sz == 0) || !bv0.all(), true);
    ASSERT_EQUAL(bv1.all(), true);
    ASSERT_EQUAL(bv2.all(), all(s2));
}

static void test_eq(const BitVector &bv, const string &str) {
    BitVector tbv1(str);
    ASSERT_EQUAL(bv, tbv1);
    BitVector tbv2(bv.size() + 1);
    assert(bv != tbv2);
}

static void test_assign(const BitVector &bv, const string &str) {
    BitVector tbv;

    tbv = bv;
    ASSERT_EQUAL(tbv.to_string(), str);
}

static void test_shift(const BitVector &bv, const string &str) {
    size_t sz = bv.size();
    size_t i;

    for (i = 0; i <= sz; i++) {
        if ((i == sz) || (i % 8 == 0) || (i % 8 == 1) || (i % 8 == 7)) {
            string strl = shift_left(str, i);

            // operator<<=
            BitVector tbvl(bv);
            tbvl <<= i;
            ASSERT_EQUAL(tbvl.to_string(), strl);

            // operator<<
            ASSERT_EQUAL((bv << i).to_string(), strl);
            string strr = shift_right(str, i);

            // operator>>=
            BitVector tbvr(bv);
            tbvr >>= i;
            ASSERT_EQUAL(tbvr.to_string(), strr);

            // operator>>
            ASSERT_EQUAL((bv >> i).to_string(), strr);
        }
    }
}

static void test_logical(const BitVector &bv, const string &str) {
    size_t sz = bv.size();
    string tstr = get_random_bit_string(sz);
    BitVector tbv(tstr);

    // operator&=
    string str_and = bitwise_and(str, tstr);
    BitVector tbv_and(bv);
    tbv_and &= tbv;
    ASSERT_EQUAL(tbv_and.to_string(), str_and);

    // operator&
    ASSERT_EQUAL((bv & tbv).to_string(), str_and);


    // operator|=
    string str_or = bitwise_or(str, tstr);
    BitVector tbv_or(bv);
    tbv_or |= tbv;
    ASSERT_EQUAL(tbv_or.to_string(), str_or);

    // operator|
    ASSERT_EQUAL((bv | tbv).to_string(), str_or);


    // operator^=
    string str_xor = bitwise_xor(str, tstr);
    BitVector tbv_xor(bv);
    tbv_xor ^= tbv;
    ASSERT_EQUAL(tbv_xor.to_string(), str_xor);

    // operator^
    ASSERT_EQUAL((bv ^ tbv).to_string(), str_xor);


    // operator~
    string str_not = bitwise_not(str);
    BitVector tbv_not(~bv);
    ASSERT_EQUAL(tbv_not.to_string(), str_not);

    BitVector tbv_not_not(~tbv_not);
    ASSERT_EQUAL(tbv_not_not.to_string(), str);
}

static void test_stream(const string &str) {
    BitVector bv;
    istringstream iss(str);
    iss >> bv;
    ASSERT_EQUAL(bv.to_string(), str);

    ostringstream oss;
    oss << bv;
    ASSERT_EQUAL(oss.str(), str);
}

static void test_operators(const BitVector &bv, const string &str) {
    test_eq(bv, str);
    test_assign(bv, str);
    test_shift(bv, str);
    test_logical(bv, str);
    test_stream(str);
}

static void test_elem_access(void) {
    string str;
    int i;

    for (i = 0; i < (int)s_nsizes; i++) {
        string str = get_random_bit_string(s_sizes[i]);
        BitVector bv(str);

        test_get(bv, str);
        test_set(s_sizes[i]);
        test_clear(s_sizes[i]);
        //test_flip(bv, str);
        test_count(bv, str);
        test_predicates(s_sizes[i]);
        //test_operators(bv, str);
    }
}

int main() {
    test_ctors();
    test_elem_access();

    cout << "ALL OK" << endl;

    return 0;
}
