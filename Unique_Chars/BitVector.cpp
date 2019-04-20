#include "BitVector.h"

BitVector::BitVector(size_t nbits){
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
    /*free(bitvec);
    delete bitvec;
    seq_size = 0;
    vec_size = 0;
    //cout << "destructor";
    return ;*/
}

string BitVector::to_string(void) const{
    string r;
    //r.reserve(seq_size);
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
    size_t ss = size();
    if(pos >= ss || (int)ss == 0)
        return ;
    else{
        DATATYPE blk = pos/SIZE;
        DATATYPE ind = pos - (blk*SIZE);
        DATATYPE temp_or = 1;
        DATATYPE fin_or = temp_or << ind;
        //cout << "here " << seq_size << " " << pos << " " <<vec_size << " " << blk << endl;
        bitvec[vec_size - blk -1] |= fin_or; 
    }
    return ;
}

void BitVector::set(size_t low, size_t high){
    if((low > high)
        || (size() == 0)
        || (low > size()-1)
        || (high > size()-1)
    )
        return ;
    // Just iterate through all those positions and set
    for(int i = (int)low; i <= (int)high; ++i){
        set(i);
    }
    return ;
}

void BitVector::set(void){
    if(seq_size == 0){
        return ;
    }
    else{
        for( DATATYPE i = 0; i < (int)vec_size; ++i)
            bitvec[i] = MAX;
        DATATYPE shift = SIZE - (seq_size / SIZE) * SIZE;
        bitvec[0] >>= shift;
        //set(0, seq_size-1);
    }
    return ;
}

void BitVector::clear(size_t pos){
    // Finding the index is same as above
    // Instead of all 0's and a 1
    // Use all 1's and a 0
    if(pos > seq_size-1)
        return ;
    else{
        DATATYPE blk = pos/SIZE;
        DATATYPE ind = pos - (blk*SIZE);
        DATATYPE temp_and = MAX - 1;
        DATATYPE fin_and = (temp_and << ind) | (temp_and >> (SIZE - ind));
        bitvec[vec_size - blk -1] &= fin_and;
    }
    return ;
}

void BitVector::clear(size_t low, size_t high){
    // Finding the index is same as above
    // Instead of all 0's and a 1
    // Use all 1's and a 0
    if((low > high)
        || (size() == 0)
        || (low > size()-1)
        || (high > size()-1)
    )
        return ;
    // Just iterate through all those positions and set
    for( int i = (int)low; i <= (int)high; ++i){
        clear(i);
    }
    return ;
}

void BitVector::clear(void){
    //Just set everything to a 0
    for (int i = 0; i < vec_size; ++i)
        bitvec[i] = 0;
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
        || (size() == 0)
        || (low > size()-1)
        || (high > size()-1)
    )
        return ;
    // Just iterate through all those positions and set
    for( int i = (int)low; i <= (int)high; ++i){
        flip(i);
    }
    return ;
}

void BitVector::flip(void){
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
    return seq_size;
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
    if(any())
        return false;
    else
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
   // Now left shift from end, keep track of the carry generated and | or with 
   // the next block
   // First shift left, store carry and or with previous carry
    /*
        First for the first segment generate a respective integer
        that is necessary ex :  first 0's and then 1's to cap till the seq_size 
    */
    BitVector ret = *this;
    while(pos >= SIZE){
        for(int i = 0; i < ((int)vec_size)-1; ++i){
            ret.bitvec[i] = ret.bitvec[i+1];
        }
        ret.bitvec[vec_size-1] = 0;
        pos = pos - SIZE;
    }
    // Now for the remaining position
    DATATYPE prev_carry, curr_carry;
    prev_carry = curr_carry = 0;
    prev_carry = (ret.bitvec[0] >> (SIZE-pos));
    ret.bitvec[0] <<= pos;
    for(int i = ((int)vec_size)-1; i >= 0; --i){
        curr_carry = (ret.bitvec[i] >> (SIZE - pos));
        ret.bitvec[i] <<= pos;
        ret.bitvec[i] |= prev_carry;
        prev_carry = curr_carry;
    }
    return ret;
}

BitVector BitVector::operator>>(size_t pos) const{
    /*
        start from the first segment
        shift right and store the carry
        keep shifting right, store the carry and | or with the previous carry
    */
    BitVector ret = *this;
    DATATYPE temp = 0;
    // In case the shift pos is greater than the SIZE
    // Keeps shifting blocks at a time
    while(pos >= SIZE){
        DATATYPE prev = ret.bitvec[0];
        DATATYPE curr = 0;
        for(int i = 1; i < int(vec_size); ++i){
            curr = ret.bitvec[i];
            ret.bitvec[i] = prev;
            prev = curr;
        }
        ret.bitvec[0] = 0;
        pos = pos - SIZE;
    }
    // Now for the remaining position
    DATATYPE prev_carry, curr_carry;
    prev_carry = curr_carry = 0;
    prev_carry = (ret.bitvec[0] << (SIZE-pos));
    ret.bitvec[0] >>= pos;
    for(int i = 1; i< int(vec_size); ++i){
        curr_carry = (ret.bitvec[i] << (SIZE - pos));
        ret.bitvec[i] >>= pos;
        ret.bitvec[i] |= prev_carry;
        prev_carry = curr_carry;
    }
    return ret;
}

bool BitVector::operator==(const BitVector &bv) const{
    if(seq_size != bv.seq_size)
        return false;
    else{
        for(int i = 0; i < vec_size; ++i){
            if(bitvec[i] != bv.bitvec[i])
                return false;
        }
    }
    return true;
}

bool BitVector::operator!=(const BitVector &bv) const{
    if(size() == bv.size()){
        for(int i = 0; i < vec_size; ++i){
            if(bitvec[i] != bv.bitvec[i])
                return true;
        }
        return false;
    }
    return true;
}

BitVector BitVector::operator=(const BitVector &bv){
    // Just call this function
    vec_size = bv.vec_size;
    seq_size = bv.seq_size;
    bitvec = (DATATYPE *)malloc(SIZE * vec_size);
    for(int i = 0; i < vec_size; ++i){
        bitvec[i] = bv.bitvec[i];
    }
    return *this;
}

BitVector BitVector::operator<<=(size_t pos){
    while(pos >= SIZE){
        for(int i = 0; i < ((int)vec_size)-1; ++i){
            bitvec[i] = bitvec[i+1];
        }
        bitvec[vec_size-1] = 0;
        pos = pos - SIZE;
    }
    // Now for the remaining position
    DATATYPE prev_carry, curr_carry;
    prev_carry = curr_carry = 0;
    prev_carry = (bitvec[0] >> (SIZE-pos));
    bitvec[0] <<= pos;
    for(int i = ((int)vec_size)-1; i >= 0; --i){
        curr_carry = (bitvec[i] >> (SIZE - pos));
        bitvec[i] <<= pos;
        bitvec[i] |= prev_carry;
        prev_carry = curr_carry;
    }
    return *this;
}

BitVector BitVector::operator>>=(size_t pos){
    DATATYPE temp = 0;
    // In case the shift pos is greater than the SIZE
    // Keeps shifting blocks at a time
    while(pos >= SIZE){
        DATATYPE prev = bitvec[0];
        DATATYPE curr = 0;
        for(int i = 1; i < int(vec_size); ++i){
            curr = bitvec[i];
            bitvec[i] = prev;
            prev = curr;
        }
        bitvec[0] = 0;
        pos = pos - SIZE;
    }
    // Now for the remaining position
    DATATYPE prev_carry, curr_carry;
    prev_carry = curr_carry = 0;
    prev_carry = (bitvec[0] << (SIZE-pos));
    bitvec[0] >>= pos;
    for(int i = 1; i< int(vec_size); ++i){
        curr_carry = (bitvec[i] << (SIZE - pos));
        bitvec[i] >>= pos;
        bitvec[i] |= prev_carry;
        prev_carry = curr_carry;
    }
    return *this;
}


BitVector BitVector::operator&=(const BitVector &bv){
    // Easy traverse through both the arrays and do &
    // But first make a new vector and then  do ops with that
    for(int i = 0; i < bv.size(); ++i){
        if(get(i) && bv.get(i)){
            set(i);
        }
        else{
            clear(i);
        }
    }
    return *this;
}

BitVector BitVector::operator|=(const BitVector &bv){
    // Same as above with or
    //Make a new BitVector, assign and return this
    for(int i = 0; i < bv.size(); ++i){
        if(get(i) || bv.get(i)){
            set(i);
        }
        else{
            clear(i);
        }
    }
    return *this;
}

BitVector BitVector::operator^=(const BitVector &bv){
    // Same as above but with xor
    //Make a new BitVector, assign and return this
    int p1, p2;
    for(int i = 0; i < vec_size; ++i){
        bitvec[i] ^= bv.bitvec[i];
    }
    return *this;
}

BitVector BitVector::operator~(void) const{
    // Just use complement operator
    // Except the first segment should be different
        // Easy traverse through both the arrays and do &
    // But first make a new vector and then  do ops with that
    DATATYPE *temp = (DATATYPE *)malloc(SIZE_D * vec_size);
    //Make a new BitVector, assign and return this
    BitVector ret;
    ret.seq_size = seq_size;
    ret.bitvec = temp;
    ret.vec_size = vec_size;
    //For the first segment
    size_t s = (vec_size-1) * SIZE;
    for(int i = 0; i < vec_size; ++i){
            temp[i] = ~bitvec[i];
    }
    DATATYPE mask = MAX;
    mask = mask >> (SIZE - (seq_size / SIZE)*SIZE);
    temp[0] &= mask;
    //For all other segments
    return ret;
}


ostream &operator<<(ostream &os, const BitVector &bv){
    return os << bv.to_string() << flush;
}

istream &operator>>(istream &is, BitVector &bv){
    string s;
    size_t ss;
    unsigned int vs;
    is >> s;
    ss = s.size();
    vs = ceil(float(ss)/SIZE);
    BitVector temp(s);
    for(int i = 0; i < ss; ++i){
        if(s[i]-'0' == 1){
            temp.set(ss-i-1);
        }
    }
    bv = temp;
    //Somehow assign these values to the bv object
}

BitVector operator&(const BitVector &bv1, const BitVector &bv2){
    // Call public functions of bv1 and bv2 for their size
    // Then iterate through both and keep anding
    // Just and everything
    BitVector ret(bv1.size());
    if(bv1.size() != bv2.size())
        return ret;
    for(size_t i = 0; i < bv1.size(); ++i){
        if(bv1.get(i) & bv2.get(i))
            ret.set(i);
    }
    return ret;
}

BitVector operator|(const BitVector &bv1, const BitVector &bv2){
    // Just or everything
    // Call public functions of bv1 and bv2 for their size
    // Then iterate through both and keep |ing
    BitVector ret(bv1.size());
    if(bv1.size() != bv2.size())
        return ret;
    for(size_t i = 0; i < bv1.size(); ++i){
        if(bv1.get(i) | bv2.get(i))
            ret.set(i);
    }
    return ret;
}

BitVector operator^(const BitVector &bv1, const BitVector &bv2){
    // Just XOR everything
    BitVector ret(bv1.size());
    if(bv1.size() != bv2.size())
        return ret;
    for(size_t i = 0; i < bv1.size(); ++i){
        if(bv1.get(i) ^ bv2.get(i))
            ret.set(i);
    }
    return ret;
}

/*
int main(){
    string s2 ="10101010101010101010101010101010101010101010101010101010101010101010";
    string s1 ="10101010101010101010101010101010101010101010101010101010101010101010";
    string s3 ="01010101010101010101010101010101010101010101010101010101010101010100";
    string st ="111111111111111111111111111111110000000000000000000000000000000011111111111111111111111111111111";
    //string s = "1111111111111111111111111111111111111111111111111111111111111111111";
    //string s = "0000000000000000000000000000000000000000000000000000000000000000000";

    BitVector obj1(st);
    //BitVector obj2(s3);
    //cout << "Your string1 is " << obj1 << endl;
    //cout << "Your string2 is " << obj2 << endl;
    //BitVector ls = obj1 << 4;
    //BitVector rs = obj1 >> 6;
    //cout << rs << endl << ls;
    //ls <<= 2;
    cout << st << endl;
    cout << rs << endl << rs.size();
}
*/