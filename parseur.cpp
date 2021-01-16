#include <iostream>
#include <fstream>

#include <string>

#define DECALAGE_XOR 7
#define OFFSET_XOR_AL 10
#define OFFSET_XOR_EAX 11

    //constantes for the disassembled ch30.bin
#define OFFSET_PASS_UN 27
#define PASS_PTR_LOC_UN "[rbp-0x8],"

    //constantes for the disassembled ch302.bin
#define OFFSET_PASS_SI 28
#define PASS_PTR_LOC_SI "[ebp-0x10],"


using namespace std;

string extractBloc(ifstream& fileIn);
char extractXOR(string bloc);
char extractPASS(string bloc, int offset, string PTR_LOC);

int main(int argc, char *argv[]){

    if ( argc != 3 && argc != 4 )
    {
        cout << "Usage: ./parseur inAsm outBase16 [-offset]" << endl;
        exit(0);
    }
    
    ifstream fileIn(argv[1], fstream::in | fstream::binary);
    ofstream fileOut(argv[2], fstream::out | fstream::binary);

    bool dayen = false;
    string bloc, flagStr(""), ptr_loc("");

    char keyByte, passByte;
    int offset_pass = (argc == 4) ? OFFSET_PASS_SI : OFFSET_PASS_UN;

    if (argc == 4)
    {
        offset_pass = OFFSET_PASS_SI;
        ptr_loc = PASS_PTR_LOC_SI;
    }
    else
    {
        offset_pass = OFFSET_PASS_UN;
        ptr_loc = PASS_PTR_LOC_UN;
    }
    
    

    while (!dayen)
    {
        bloc = extractBloc(fileIn);
        dayen = bloc == "";

        if (!dayen)
        {
            keyByte = extractXOR(bloc);
            
            passByte = extractPASS(bloc, offset_pass, ptr_loc);

            flagStr.push_back(keyByte ^ passByte);            
        }    
    }

    fileOut << flagStr;
    
    fileIn.close();
    fileOut.close();
}   

string extractBloc(ifstream& fileIn){
    string bloc = "";
    char buffer[200] = {0};
    
    while (bloc.find("badboy") == string::npos && fileIn.getline(buffer, 200))
    {
        bloc.append(buffer);
        bloc.append("\n");
    }

    return bloc;
}

char extractXOR(string bloc){
    int posi = bloc.find("xor");

    if ( posi != string::npos ){
        string operand = "";
        posi = bloc[posi+DECALAGE_XOR] == 'a' ? posi+OFFSET_XOR_AL : posi+OFFSET_XOR_EAX;

        while (bloc[posi] != '\n')
        {
            operand += bloc[posi];
            posi++;
        }
        
        return stoi(operand, nullptr, 0);
    }
    else
        return 0;   //Some blocks don't include an eor operation, this is (possibily)due to the fact 
                    //that the key here is equal to 0, then "xor A, 0" is equivalent to doing nothing
    
}

char extractPASS(string bloc, int OFFSET, string PTR_LOC){
    int posi = bloc.find("mov    DWORD PTR " + PTR_LOC) + OFFSET;
    string operand = "";

    while (bloc[posi] != '\n')
        {
            operand += bloc[posi];
            posi++;
        }

    return stoi(operand, nullptr, 0);
}