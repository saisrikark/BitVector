#include "BitVector.h"

BitVector::BitVector(size_t nbits){
    cout << "Copy constructor called" << endl;
    if(nbits == 0){
        // Assigning only 1 segment
        bitvec = NULL;
        return ;
    }
    else{
        seq_size = nbits;
        vec_size = ceil(nbits/float(SIZE));
        bitvec = (DATATYPE *)malloc(SIZE * vec_size);
        for(int i = 0; i < vec_size; ++i){
            bitvec[i] = 0;
        }
    }
    return ;
}


BitVector::BitVector(const BitVector &bv){
    vec_size = bv.vec_size;
    seq_size = bv.seq_size;
    bitvec = (DATATYPE *)malloc(SIZE * vec_size);
    for(int i = 0; i < vec_size; ++i){
        bitvec[i] = bv.bitvec[i];
    }
    return ;
}

BitVector::BitVector(const string &bv_str){
    size_t s = bv_str.size();    
    if(s == 0){
        seq_size = 0;
        vec_size = 0;
        bitvec = NULL;
        return ;
    }
    else{
        seq_size = s;
        vec_size = ceil(s/((float)SIZE));
        bitvec = (DATATYPE *)malloc(SIZE_D * vec_size);
        DATATYPE temp = 0; // used to store the temporary block
        int siter, biter;
        bool bit; 
        // siter for the inner block iteration
        // biter is to iterate through blocks
        siter = 0; bit = 0; biter = vec_size-1; 
        int temps = s - 1;
        for(int i = 0; i < vec_size; ++i){
            bitvec[i] = 0;
        }
        do{
            bit = bv_str[temps] - '0';
            if(siter == SIZE){
                bitvec[biter] = temp;
                --biter;
                siter = 1;
                temp = (DATATYPE)bit;
            }
            else{
                temp += (DATATYPE)(DATATYPE)bit * pow(2,siter);
                ++siter;
            }
        }while(temps--);
        if(temp != 0){
            bitvec[biter] = temp;
        }
    }
    return ;
}

BitVector::~BitVector(void){
    free(bitvec);
    delete bitvec;
    seq_size = 0;
    vec_size = 0;
    return ;
}

string BitVector::to_string(void) const{
    string r;
    r.reserve(seq_size);
    int s = seq_size;
    cout <<endl;
    // Some code to copy the bitvec to the string with reserved length
    // Just iterate from the end and check if the s variable is not 0
    // Just check if each bit is set or not accordingly update to the final striing
    // NOTE: pos is counted from the end of the string.
    for(int i = seq_size-1; i >= 0; --i){
        int bit = get(i);
        if(bit == 1)
            r.insert(r.end(),'1');
        else if(bit == 0)
            r.insert(r.end(),'0');
    }
    return r;
}

void BitVector::set(size_t pos){
    // First get the index in the bitvector
    // Next get the index in the unsigned int
    // Now set by using a variable left shifted the required number of time
    // Have to make this constant time
    if(pos > seq_size-1)
        return ;
    else{
        int blk = pos/SIZE;
        int ind = pos - (blk*SIZE);
        unsigned int temp_or = 1;
        unsigned int fin_or = temp_or << ind;
        bitvec[vec_size - blk -1] |= fin_or; 
    }
    return ;
}

void BitVector::set(size_t low, size_t high){
    if((low > high)
        || (low > seq_size-1)
        || (high > seq_size-1)
    )
        return ;
    // Just iterate through all those positions and set
    for(size_t i = low; i <= high; ++i){
        set(i);
    }
    return ;
}

void BitVector::set(void){
    if(seq_size == 0)
        return ;
    else{
        set(0, seq_size-1);
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
        bitvec[vec_size - blk -1] &= fin_and;
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
        return ;
    // Just iterate through all those positions and set
    for( int i = low; i <= high; ++i){
        clear(i);
    }
    return ;
}

void BitVector::clear(void){
    //Just set everything to a 0
    clear(0, seq_size-1);
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
        bitvec[vec_size - blk -1] ^= fin_xor; 
    }
    return ;
}

void BitVector::flip(size_t low, size_t high){
    // Same method, generate the int with all 1's in low to high positions
    if((low > high)
        || (low > seq_size-1)
        || (high > seq_size-1)
    )
        return ;
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
        flip(0, seq_size-1);
    }
    return ;
}

int BitVector::get(size_t pos) const{
    // Go to the position
    // And with required integer
    // Return the bool value
    if(pos > seq_size-1)
        return -1;
    else{
        size_t blk = pos/SIZE;
        int ind = pos - (blk*SIZE);
        DATATYPE temp = 1;
        DATATYPE fin = temp << ind;
        int ret = bitvec[vec_size - blk -1] & fin; 
        if(ret != 0)
            return 1;
        else
            return 0;
    }
}

size_t BitVector::size(void) const{
    return (size_t)seq_size;
}


size_t countSetBits(DATATYPE n) 
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
    for(int it = 0; it < vec_size; ++it){
        r += countSetBits(bitvec[it]);
    }
    return r;
}

bool BitVector::any(void) const{
    // check if anything is greater than 0
    for(int it = 0; it < vec_size; ++it){
        if(bitvec[it]){
            return true;
        }
    }
    return false;
}

bool BitVector::none(void) const{
    // Just check if all ints are 0's
    for(int i = 0; i < vec_size; ++i){
        if(bitvec[i])
            return false;
    }
    return true;
}

bool BitVector::all(void) const{
    //Check the condition for first segment according to the vector
    //Now go to the succeeding segment, and with INT_MAX for unsigned
    if(count() == seq_size){
        return true;
    }
    else{
        return false;
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
    if(vec_size == bv.vec_size)
    {
        for(int i = 0; i < vec_size; ++i){
            if(bitvec[i] != bv.bitvec[i])
                return false;
        }
    }
    else
        return false;
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
    if(vec_size == bv.vec_size || seq_size != bv.seq_size)
    {
        for(int i = 0; i < vec_size; ++i){
            if(bitvec[i] == bv.bitvec[i])
                return false;
        }
    }
    return true;
}

BitVector BitVector::operator=(const BitVector &bv){
    // Just call this function
    cout << "Assignment operator used" << endl;
    return BitVector(bv);
}

BitVector BitVector::operator<<=(size_t pos){
    BitVector res = BitVector::operator<<(pos);
    return res;
}

BitVector BitVector::operator>>=(size_t pos){
    BitVector res = BitVector::operator>>(pos);
    return res;
}

/*
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
*/

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


int main(){
    //string s ="001010101010101010101010101010101010101010101010101010101010101010";
    string s = "10101010101010101010101010101010101010101010101010101010101010101010";
    //string s = "1111111111111111111111111111111111111111111111111111111111111111111";
    //string s = "0000000000000000000000000000000000000000000000000000000000000000000";

    BitVector obj(s);

    //none not working
    cout << "Given string is " << s << endl;
    cout << "Your string is " << obj.to_string() << endl;
    //obj.flip();
    cout << obj.to_string() << endl;
    cout << "# set bits = " << obj.count() << endl;
    cout << "any : " << obj.any() << endl;
    cout << "all : " << obj.all() << endl;
    cout << "none : " << obj.any() << endl;
    cout << "operator : " << obj[65] << endl;
    BitVector obj2 = obj;
    cout << "Given string is " << s << endl;
    cout << "Your string is " << obj.to_string() << endl;
    //cout << "Equality" << obj != obj2;
}