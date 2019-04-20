// Unique Characters Program

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cstring>
#include "BitVector.h"

using namespace std; 

int main(){
    string inp_str;
    cin >> inp_str;
    BitVector bv(256);
    int bitpos = 0;
    BitVector shift(256);
    BitVector zero(256);
    shift.set(0);
    int i;
    for( i = 0; i < inp_str.size(); ++i){
        bitpos = inp_str[i];
        BitVector temp = shift << (int)bitpos;
        if((bv & temp) != zero){
            cout << "Not Unique " << endl;
            return 0;
        }
        bv |= temp;
    }
    cout << "Unique" << endl;
    return 1;
}