#include<iostream>
#include<fstream>
#include<math.h>
#include<vector>
#include"NumericBase.h"
#include"Alphabet.h"
#include"NumericString.h"
#include"TuringMachine.h"

int main(void){
	Strip st = Strip("0101",'b');
	TuringMachine tm = TuringMachine("turing.tm");
	tm.print();
	return 0;
}