using namespace std;

class Strip{
private:
	string symbols;
	int pointer;
	char blank;
public:
	Strip();
	Strip(char);
	Strip(string,char);
	~Strip();
	void setSymbols(string);
	string getSymbols();
	void left();
	void right();
	void write(char);
	void print();
};

Strip::Strip(){
	symbols = "";
	pointer = 0;
	blank = 0;
}
Strip::Strip(char _blank){
	blank = _blank;
	symbols = _blank;
	symbols += _blank;
	pointer = 0;
}
Strip::Strip(string _symbols,char _blank){
	blank = _blank;
	symbols = blank;
	symbols += _symbols;
	symbols += _blank;
	pointer = 1;
}
Strip::~Strip(){
}
void Strip::setSymbols(string _symbols){
	symbols = _symbols;
}
string Strip::getSymbols(){
	return symbols;
}
void Strip::left(){
	pointer--;
	if(pointer < 0){
		pointer = 0;
	}
}	
void Strip::right(){
	pointer++;
	if(pointer >= symbols.size()){
		pointer = symbols.size();
	}
}
void Strip::write(char symbol){
	symbols[pointer] = symbol;
	if(pointer == 0){
		pointer = 1;
		symbols.insert(symbols.begin(),blank);
	}else if(pointer == symbols.size()){
		symbols.push_back(blank);
	}
}
void Strip::print(){
	cout<<symbols<<" -> "<<symbols[pointer]<<endl;
}

class TuringMachine{
private:
	Strip strip;
	Alphabet inSymbols;
	Alphabet stripSymbols;
	char blank;
	int states;
	int initialState;
	int actualState;
	vector<int> finalStates;
	vector<vector<vector<int>>> transitionFunction;
	Alphabet getAlphabet(int,string);
	char getBlank(string);
	vector<int> getNumbers(string,int);
	string getTransitionS(string);
	vector<vector<vector<int>>> convertTransition(string);
public:
	~TuringMachine();
	TuringMachine();
	TuringMachine(string);
	void print();
};
TuringMachine::~TuringMachine(){
}
TuringMachine::TuringMachine(){
	strip = Strip();
	inSymbols = Alphabet();
	stripSymbols = Alphabet();
	blank = 0;
	states = 0;
	initialState = 0;
	actualState = 0;
	finalStates = vector<int>(0);
	transitionFunction = vector<vector<vector<int>>>(0);
}
TuringMachine::TuringMachine(string file){
	ifstream f;
	f.open(file,ios::in);
	if(!f.fail()){
		strip = Strip();
		stripSymbols = getAlphabet(0,file);
		inSymbols = getAlphabet(1,file);
		blank = getBlank(file);
		states = getNumbers(file,8)[0];
		initialState = getNumbers(file,10)[0];
		actualState = initialState;
		finalStates = getNumbers(file,12);
		string tr = getTransitionS(file);
		transitionFunction = convertTransition(tr);
	}else{
		cerr<<"Error tm file"<<endl;
	}
}
Alphabet TuringMachine::getAlphabet(int nAlp,string file){
	ifstream f;
	f.open(file,ios::in);
	if(!f.fail()){
		string typ = "",sym = "";
		Alphabet alf;
		for(unsigned i = 0; i < nAlp*2; i++){
			getline(f,typ);
		}
		getline(f,typ);
		getline(f,sym);
		if(typ=="string:"){
			alf = Alphabet(sym);
		}else if(typ=="range:"){
			int ini,fin;
			ini = getStringNumber(sym,1);
			fin = getStringNumber(sym,2);
			alf = Alphabet(ini,fin);
		}else if(typ=="ascii:"){
			vector<int> ascii = getStringNumbers(sym);
			sym = "";
			for(unsigned i = 0; i < ascii.size(); i++){
				sym += ascii[i];
			}
			alf = Alphabet(sym);
		}
		return alf;
	}else{
		cerr<<"Error tm file"<<endl;
	}
}
char TuringMachine::getBlank(string file){
	ifstream f;
	f.open(file,ios::in);
	if(!f.fail()){
		char _blank;
		string aux = "";
		for(unsigned i = 0;i < 6; i++){
			getline(f,aux);
		}					
		_blank = aux[0];
		if(stripSymbols.getChars().find(_blank)!=string::npos){
			return _blank;
		}else{
			cerr<<"Error blank symbol"<<endl;
		}
	}else{
		cerr<<"Error tm file"<<endl;
	}
}
vector<int> TuringMachine::getNumbers(string file,int line){
	ifstream f;
	f.open(file,ios::in);
	if(!f.fail()){
		vector<int> numbers;
		string aux = "";
		for(unsigned i = 0;i < line; i++){
			getline(f,aux);
		}
		numbers = getStringNumbers(aux);
		return numbers;
	}else{
		cerr<<"Error tm file"<<endl;
	}
}
string TuringMachine::getTransitionS(string file){
	ifstream f;
	f.open(file,ios::in);
	if(!f.fail()){
		string aux = "",tran="";
		for(unsigned i = 0;i < 13; i++){
			getline(f,aux);
		}
		aux = "";
		while(!f.eof()){
			getline(f,aux);
			tran += aux;
		}
		return tran;	
	}else{
		cerr<<"Error tm file"<<endl;
	}
}
vector<vector<vector<int>>> TuringMachine::convertTransition(string tr){
	vector<vector<vector<int>>> transition = vector<vector<vector<int>>>(0);
	vector<vector<int>> row = vector<vector<int>>(0);
	vector<int> numbers = vector<int>(0);
	vector<string> split = splitString(tr,","),moves;
	int counter = 0, state,symbol,move;
	string aux = "";
	for(unsigned i = 0; i < states; i++){
		for(unsigned j = 0; j < inSymbols.getChars().size(); j++){
			numbers.clear();
			aux = split[counter];
			aux.pop_back();
			aux.replace(0,1,"");
			moves = splitString(aux,":");
			if(moves.size() == 3){
				numbers.push_back(getStringNumber(moves[0]));
				numbers.push_back(moves[1][0]);
				numbers.push_back(getStringNumber(moves[2]));
			}else{
				numbers = vector<int>(0);
			}
			row.push_back(numbers);
			counter++;
		}
		transition.push_back(row);
		row.clear();
	}
	return transition;
}
void TuringMachine::print(){
	cout<<"\t";
	for(unsigned i = 0; i < inSymbols.getChars().size(); i++){
		cout<<"|"<<inSymbols.getChars()[i]<<"       ";
	}
	cout<<endl;
	for(unsigned i = 0; i < inSymbols.getChars().size(); i++){
		cout<<"------------";
	}
	cout<<endl;
	for(unsigned i = 0; i < states; i++){
		if(containsVectorInt(finalStates,i)){
			cout<<"*";
		}
		if(i == initialState){
			cout<<"->";
		}
		cout<<i<<"\t|";
		for(unsigned j = 0; j < inSymbols.getChars().size(); j++){
			if(transitionFunction[i][j].size() == 3){
				cout<<"("<<transitionFunction[i][j][0]<<':'<<((char)transitionFunction[i][j][1]);
				cout<<':'<<transitionFunction[i][j][2]<<")  ";
			}else{
				cout<<"( : : )  ";
			}
		}
		cout<<endl;
	}
	cout<<endl;
}