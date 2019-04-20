// Unique Characters Program

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "BitVector.h"

int main(){
    string inp_str;
    cin >> inp_str;
    BitVector bv(256);
    int bitpos = 0;
    BitVector shift(256);
    BitVector zero(256);
    shift.set(0);
    for( int i = 0; i < strlen(inp_str); ++i){
        bitpos = inp_str[i];
        BitVector temp = (shift << bitpos);
        if((bv & temp) != zero){
            cout << "Not Unique " << endl;
            return false;
        }
        bv |= temp;
    }
    cout << "Unique" << endl;
    return true;
}