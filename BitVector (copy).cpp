#include "BitVector.h"

BitVector::BitVector(size_t nbits){
    if(nbits == 0){
        // Assigning only 1 segment
        seq_size = SIZE;
        bitvec.reserve(1);
        *bitvec.begin() = 0;
    }
    else{
        seq_size = nbits;
        int s = nbits/SIZE;
        bitvec.reserve(s);
        for(auto it = bitvec.begin();
            it != bitvec.end(); ++it){
            *it = 0;
        }
    }
    return ;
}

BitVector::BitVector(const BitVector &bv){
    size_t n = bv.bitvec.size();
    seq_size = bv.seq_size;
    bitvec.reserve(n);
    for(auto it = bv.bitvec.begin(); it != bv.bitvec.end(); ++it){
        bitvec.push_back(*it);
    }
    return ;
}

BitVector::BitVector(const string &bv_str){
    size_t s = bv_str.size();
    int res = ceil(bv_str.size()/float(SIZE));
    bitvec.reserve(res);
    cout << "INSIDE CONSTRUCTOR String is: " << bv_str << " SIZE " << res <<endl;
    seq_size = s;
    if(s == 0)
        return ;
    else{
        DATATYPE temp, k, t;
        temp = 0;
        k = 0;

        /*for(auto it = bv_str.end(); it != bv_str.begin(); --it, ++k){
            //Keep traversing the string from behind and each 8 characters,
            //update it to the bitvector by using insert at the beginning
            if(k == SIZE){
                k = 0;
                bitvec.insert(bitvec.begin(),temp);
                temp = 0;
            }
            t = 0;//pow(2,k) * (DATATYPE)(*it-'0');;
            temp += t;
            cout << *it << " " << temp;
        }*/
        for(int it = bv_str.size()-1; it >= 0; --it, ++k){
            //Keep traversing the string from behind and each 8 characters,
            //update it to the bitvector by using insert at the beginning
            if(k == SIZE){
                k = 0;
                bitvec.insert(bitvec.begin(),temp);
                temp = 0;
            }
            t = pow(2,k) * (DATATYPE)(bv_str[it]-'0');
            temp += t;
            cout << t << " " << temp;
        }
        cout << endl;
    }
}

BitVector::~BitVector(void){
    bitvec.clear();
    seq_size = 0;
    return ;
}

string BitVector::to_string(void) const{
    std::string r;
    r.reserve(seq_size);
    int s = seq_size;
    // Some code to copy the bitvec to the string with reserved length
    // Just iterate from the end and check if the s variable is not 0
    // Just check if each bit is set or not accordingly update to the final striing
    // NOTE: pos is counted from the end of the string.
    for(int i = 0; i < seq_size; ++i){
        if(get(i))
            r.insert(r.begin(),'1');
        else
            r.insert(r.begin(),'0');
    }
    return r;
}

void BitVector::set(size_t pos){
    // First get the index in the bitvector
    // Next get the index in the unsigned int
    // Now set by using a variable left shifted the required number of time
    // Have to make this constant time
    if(pos > seq_size-1)
        return;
    else{
        int blk = pos/SIZE;
        int ind = pos - (blk*SIZE);
        unsigned int temp_or = 1;
        unsigned int fin_or = temp_or << ind;
        bitvec[bitvec.size()- blk -1] |= fin_or; 
    }
    return ;
}

void BitVector::set(size_t low, size_t high){
    if((low > high)
        || (low > seq_size-1)
        || (high > seq_size-1)
    )
    // Just iterate through all those positions and set
    for( int i = low; i <= high; ++i){
        set(i);
    }
    return ;
}

void BitVector::set(void){
    // Start from beginning
    // Get the index where we should start
    // Generate an integer with all ones from that position
    // For all other unsigned ints or with unsigned int max
    if(seq_size == 0)
        return;
    else{
        auto it = bitvec.begin();
        // Here is the code to set the bits for the first block
        // For the rest it's east just assign it to the largest unsigned int
        //CHECK the below it's mostly wrong
        set(((seq_size-1)/SIZE)*SIZE, seq_size-1);
        if(bitvec.size() > 1)
        {
            ++it;
            unsigned int temp = UINT32_MAX;
            for(; it != bitvec.end(); ++it){
                *it = temp;
            }            
        }
    }
    return ;
}

void BitVector::clear(size_t pos){
    // Finding the index is same as above
    // Instead of all 0's and a 1
    // Use all 1's and a 0
    if(pos > seq_size-1)
        return;
    else{
        int blk = pos/SIZE;
        int ind = pos - (blk*SIZE);
        unsigned int temp_and = UINT32_MAX - 1;
        unsigned int fin_and = (temp_and << ind) | (temp_and >> (SIZE - ind));
        bitvec[bitvec.size()- blk -1] &= fin_and;
    }
    return ;
}

void BitVector::clear(size_t low, size_t high){
    // Finding the index is same as above
    // Instead of all 0's and a 1
    // Use all 1's and a 0
    if((low > high)
        || (low > seq_size-1)
        || (high > seq_size-1)
    )
    // Just iterate through all those positions and set
    for( int i = low; i <= high; ++i){
        clear(i);
    }
    return ;
}

void BitVector::clear(void){
    //Just set everything to a 0
    for(auto it = bitvec.begin(); it != bitvec.end(); ++it){
        *it = 0;
    }
}

void BitVector::flip(size_t pos){
    // Just do XOR with 1 shifted pos times
    // XOR : Even number of 1's : 0, Odd number of 1's : 1
    if(pos > seq_size-1)
        return;
    else{
        int blk = pos/SIZE;
        int ind = pos - (blk*SIZE);
        unsigned int temp_xor = 1;
        unsigned int fin_xor = temp_xor << ind;
        bitvec[bitvec.size()- blk -1] ^= fin_xor; 
    }
    return ;
}

void BitVector::flip(size_t low, size_t high){
    // Same method, generate the int with all 1's in low to high positions
    if((low > high)
        || (low > seq_size-1)
        || (high > seq_size-1)
    )
    // Just iterate through all those positions and set
    for( int i = low; i <= high; ++i){
        flip(i);
    }
    return ;
}

void BitVector::flip(void){
    // Same thing with XOR
    if(seq_size == 0)
        return;
    else{
        auto it = bitvec.begin();
        // Here is the code to flip the bits for the first block
        // For the rest it's east just assign it to the largest unsigned int
        // CHECK the below it's mostly wrong
        flip(((seq_size-1)/SIZE)*SIZE, seq_size-1);
        if(bitvec.size() > 1)
        {
            ++it;
            unsigned int temp = UINT32_MAX;
            for(; it != bitvec.end(); ++it){
                *it = temp;
            }            
        }
    }
    return ;
}

int BitVector::get(size_t pos) const{
    // Go to the position
    // And with required integer
    // Return the bool value
    if(pos > seq_size-1)
        return -1;
    else{/*
        int blk = pos/SIZE;
        int ind = pos - (blk*SIZE);
        unsigned int temp = 1;
        unsigned int fin = temp << ind;
        int ret = bitvec[bitvec.size()- blk -1] & fin; 
        if(ret)
            return 1;*/
        return 0;
    }
}

size_t BitVector::size(void) const{
    return seq_size;
}

size_t countSetBits(unsigned int n) 
{ 
    size_t count = 0; 
    while (n) 
    { 
        count += n & 1; 
        n >>= 1; 
    } 
    return count; 
} 

size_t BitVector::count(void) const{
    // For each block check the number of bits, keep adding and return
    size_t r = 0;
    for(auto it = bitvec.begin(); it!= bitvec.end(); ++it){
        r += countSetBits(*it);
    }
    return r;
}

bool BitVector::any(void) const{
    // check if anything is greater than 0
    for(auto iter = bitvec.begin(); iter != bitvec.end(); ++iter){
        if(*iter){
            return true;
        }
    }
    return false;
}

bool BitVector::none(void) const{
    // Just check if all ints are 0's
    for(auto iter = bitvec.begin(); iter != bitvec.end(); ++iter){
        if(!(*iter)){
            return true;
        }
    }
    return false;
}

bool BitVector::all(void) const{
    //Check the condition for first segment according to the vector
    //Now go to the succeeding segment, and with INT_MAX for unsigned
    auto iter = bitvec.begin();
    // Check for all 1's here
    int size = seq_size;
    if(bitvec.size() > 1){
        ++iter;
        int ui = UINT_MAX;
        //  Now check if all the next segments are set;
        for( ; iter != bitvec.begin(); ++iter){
            if( *iter != ui)
                return false;
        }
        return true;
    }
}

int BitVector::operator[](size_t pos) const{
    if(pos > seq_size-1)
        return -1;
    else{
        return get(pos);
    }
}

BitVector BitVector::operator<<(size_t pos) const{

}

BitVector BitVector::operator>>(size_t pos) const{

}

bool BitVector::operator==(const BitVector &bv) const{
    if( (seq_size == (size_t)0 ) == (bv.seq_size == (size_t)0) )
        return true;
    if((seq_size == 0 && bv.seq_size != 0)
        || (seq_size != 0 && bv.seq_size == 0))
        return false;
    // Compare element by element
    if(bitvec.size() == bv.bitvec.size())
    {
        for(auto it1 = bitvec.end(), it2 = bv.bitvec.end();
            it1 != bitvec.begin(), it2 != bitvec.begin();
            --it1, --it2){
            if(*it1 != *it2)
                return false;
        }
    }
    return true;
}

bool BitVector::operator!=(const BitVector &bv) const{
    // Opposite of above
    if( (seq_size == (size_t)0 ) == (bv.seq_size == (size_t)0) )
        return true;
    if((seq_size == 0 && bv.seq_size != 0)
        || (seq_size != 0 && bv.seq_size == 0))
        return false;
    // Compare element by element
    if(bitvec.size() == bv.bitvec.size())
    {
        for(auto it1 = bitvec.end(), it2 = bv.bitvec.end();
            it1 != bitvec.begin(), it2 != bitvec.begin();
            --it1, --it2){
            if(*it1 == *it2)
                return false;
        }
    }
    return true;
}

BitVector BitVector::operator=(const BitVector &bv){
    // Just call this function
    return BitVector(bv);
}

BitVector BitVector::operator<<=(size_t pos){
    BitVector ret = BitVector::operator<<(pos);
    return ret;
}

BitVector BitVector::operator>>=(size_t pos){
    BitVector ret = BitVector::operator>>(pos);
    return ret;
}

BitVector BitVector::operator&=(const BitVector &bv){
    // Easy traverse through both the arrays and do &
    // But first make a new vector and then  do ops with that
    std::vector<DATATYPE> temp;
    size_t size;

    auto it1 = bv.bitvec.begin();
    auto it2 = bitvec.begin();
    auto it3 = temp.begin();
    for(; it1 != bv.bitvec.end(), it2 != bitvec.end(), it3 != temp.end();
        ++it1, ++it2, ++it3){
            *it3 = *it2 & *it1;
        }

    //Make a new BitVector, assign and return this
    BitVector ret;
    ret.seq_size = size;
    ret.bitvec = temp;
    return ret;
}

BitVector BitVector::operator|=(const BitVector &bv){
    // Same as above with or
    std::vector<DATATYPE> temp;
    size_t size;

    auto it1 = bv.bitvec.begin();
    auto it2 = bitvec.begin();
    auto it3 = temp.begin();
    for(; it1 != bv.bitvec.end(), it2 != bitvec.end(), it3 != temp.end();
        ++it1, ++it2, ++it3){
            *it3 = *it2 | *it1;
        }

    //Make a new BitVector, assign and return this
    BitVector ret;
    ret.seq_size = size;
    ret.bitvec = temp;
    return ret;
}

BitVector BitVector::operator^=(const BitVector &bv){
    // Same as above but with xor
    std::vector<DATATYPE> temp;
    size_t size;

    auto it1 = bv.bitvec.begin();
    auto it2 = bitvec.begin();
    auto it3 = temp.begin();
    for(; it1 != bv.bitvec.end(), it2 != bitvec.end(), it3 != temp.end();
        ++it1, ++it2, ++it3){
            *it3 = *it2 ^ *it1;
        }

    //Make a new BitVector, assign and return this
    BitVector ret;
    ret.seq_size = size;
    ret.bitvec = temp;
    return ret;
}

BitVector BitVector::operator~(void) const{
    // Just use complement operator
    // Except the first segment should be different
        // Easy traverse through both the arrays and do &
    // But first make a new vector and then  do ops with that
    std::vector<DATATYPE> temp;
    size_t size;

    auto it1 = bitvec.begin();
    auto it2 = temp.begin();

    //Make a new BitVector, assign and return this
    BitVector ret;
    ret.seq_size = size;
    ret.bitvec = temp;

    //For the first segment
    size_t s = (bitvec.size()-1) * SIZE;
    *it2 = *it1;
    ret.flip(s, seq_size);

    for(; it1 != bitvec.end(), it2 != bitvec.end();
        ++it1, ++it2){
            *it2 = ~(*it2);
        }


    return ret;
}

ostream &operator<<(ostream &os, const BitVector &bv){
    return os << bv.to_string();
}

istream &operator>>(istream &is, BitVector &bv){

}

BitVector operator&(const BitVector &bv1, const BitVector &bv2){
    // Just and everything
    
}

BitVector operator|(const BitVector &bv1, const BitVector &bv2){
    // Just or everything
}

BitVector operator^(const BitVector &bv1, const BitVector &bv2){
    // Just XOR everything

}