#include <iostream>
#include <fstream>

using namespace std;

char getEncodage(char V);

int main(){
    ifstream fileIn("keyNoSpace", fstream::binary);
    ofstream fileOut("ch302.exe", fstream::binary);

    char tlat[4] = {0},
         rbaa[3] = {0};


    while (fileIn.read(tlat, 4))
    {
        rbaa[0] = (getEncodage(tlat[0]) << 2) | (getEncodage(tlat[1]) >> 4);
        rbaa[1] = (getEncodage(tlat[1]) << 4) | (getEncodage(tlat[2]) >> 2);
        rbaa[2] = (getEncodage(tlat[2]) << 6) | (getEncodage(tlat[3]));

        fileOut.write(rbaa, 3);
    }
    
    fileOut.close();
    fileIn.close();
}

char getEncodage(char V){

    if (V >= 'A' && V <= 'Z')
        return V - 'A';

    else if ( V >= 'a' && V <= 'z')
        return V - 'a' + 26;

    else if ( V >= '0' && V <= '9')
        return V - '0' + 52;

    else if ( V == '+' )
        return 62;

    else if ( V == '/' )
        return 63;

    else
        return 0;
}