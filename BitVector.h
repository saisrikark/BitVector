#ifndef __BIT_VECTOR_H__
#define __BIT_VECTOR_H__
#define SIZE 32
#define SIZE_D 4
#define DATATYPE unsigned int

#include <iostream>
#include <string>
#include <vector>
#include <limits.h>
#include <cmath>

using namespace std;

// BitVector is a class that encapsulates a sequence of bits of arbitrary length.
// Note that the bits are counted starting from the least significant position,
// i.e. from the "right".
class BitVector {
public:
    // Construct a BitVector of the specified size.
    BitVector(size_t nbits = 0);

    // Copy constructor
    BitVector(const BitVector &bv);

    // Construct a BitVector from a string representation of zeroes and ones.
    BitVector(const string &bv_str);

    // Destructor
    ~BitVector(void);

    // Return the string representation of this BitVector. The most significant
    // bits appear before the least significant bits in the returned string.
    string to_string(void) const;

    // Set the bit at the specified index position.
    void set(size_t pos);

    // Set the bits at the specified index positions in the range [low, high],
    // both inclusive.
    void set(size_t low, size_t high);

    // Set all the bits in this BitVector.
    void set(void);

    // Clear the bit at the specified index position.
    void clear(size_t pos);

    // Clear the bits at the specified index positions in the range [low, high],
    // both inclusive.
    void clear(size_t low, size_t high);

    // Clear all the bits in this BitVector.
    void clear(void);

    // Flip the bit at the specified index position.
    void flip(size_t pos);

    // Flip the bits at the specified index positions in the range [low, high],
    // both inclusive.
    void flip(size_t low, size_t high);

    // Flip all the bits in this BitVector.
    void flip(void);

    // Return 0 or 1 based on the bit at the specified index position.
    int get(size_t pos) const;

    // Return the total number of bits in this BitVector.
    size_t size(void) const;

    // Return the number of 1-bits in this BitVector.
    size_t count(void) const;

    // Return true if any bit is set in this BitVector, false otherwise.
    bool any(void) const;

    // Return true if none of the bits are set in this BitVector,
    // false otherwise.
    bool none(void) const;

    // Return true if all the bits are set in this BitVector, false otherwise.
    bool all(void) const;

    // Subscript operator which retrieves the bit at the specified index
    // position; e.g.: to retrieve the bit at index 10:
    // BitVector bv;
    // ...
    // int bit = bv[10];
    int operator[](size_t pos) const;

    // Bitwise left shift operator: shift this BitVector to the left by the
    // specified number of positions and return the result as a new BitVector
    // object.
    BitVector operator<<(size_t pos) const;
    
    // Bitwise right shift operator: shift this BitVector to the right by the
    // specified number of positions and return the result as a new BitVector
    // object.
    BitVector operator>>(size_t pos) const;

    // Equality operator: return true if this BitVector is identical to the
    // specified BitVector, false otherwise.
    bool operator==(const BitVector &bv) const;

    // Inequality operator: return true if this BitVector is not identical to
    // the specified BitVector, false otherwise.
    bool operator!=(const BitVector &bv) const;

    // Copy assignment operator; this makes it possible to assign one BitVector
    // to another:
    // BitVector bv1, bv2;
    // ...
    // bv1 = bv2;
    BitVector operator=(const BitVector &bv);

    // Bitwise left shift operator: Shift this BitVector to the left in-place by
    // the specified number of positions.
    BitVector operator<<=(size_t pos);

    // Bitwise right shift operator: Shift this BitVector to the right in-place
    // by the specified number of positions.
    BitVector operator>>=(size_t pos);

    // Bitwise AND operator: AND this BitVector with the specified BitVector,
    // and store the result in this BitVector.
    // The two BitVectors should be of the same size.
    BitVector operator&=(const BitVector &bv);

    // Bitwise OR operator: OR this BitVector with the specified BitVector,
    // and store the result in this BitVector.
    // The two BitVectors should be of the same size.
    BitVector operator|=(const BitVector &bv);

    // Bitwise XOR operator: XOR this BitVector with the specified BitVector,
    // and store the result in this BitVector.
    // The two BitVectors should be of the same size.
    BitVector operator^=(const BitVector &bv);

    // Return the ones' complement of this BitVector as a new BitVector object.
    BitVector operator~(void) const;

    // Returns the vector bitvec
    BitVector ret_bitvec (int size, std::vector<DATATYPE> &ret_vec);

    /***** TODO: ANY PUBLIC HELPER FUNCTION DECLARATIONS GO HERE *****/


private:
    /***** TODO: PRIVATE FIELDS AND MEMBER FUNCTION DECLARATIONS GO HERE *****/
    size_t seq_size; // Number of bits being represented
    unsigned int vec_size; // Number of block/segments required
    DATATYPE *bitvec;
};


// Output stream insertion operator
// e.g. we should be able to write code like this to print a BitVector:
// BitVector bv;
// ...
// cout << bv;
ostream &operator<<(ostream &os, const BitVector &bv);

// Input stream insertion operator
// e.g. we should be able to write code like this to input a BitVector:
// BitVector bv;
// cin >> bv;
istream &operator>>(istream &is, BitVector &bv);

// Bitwise AND operator: AND the two specified BitVectors, and return the
// result as a new BitVector object.
// The two BitVectors should be of the same size.
BitVector operator&(const BitVector &bv1, const BitVector &bv2);

// Bitwise OR operator: OR the two specified BitVectors, and return the
// result as a new BitVector object.
// The two BitVectors should be of the same size.
BitVector operator|(const BitVector &bv1, const BitVector &bv2);

// Bitwise XOR operator: XOR the two specified BitVectors, and return the
// result as a new BitVector object.
// The two BitVectors should be of the same size.
BitVector operator^(const BitVector &bv1, const BitVector &bv2);


#endif  /* __BIT_VECTOR_H__ */
