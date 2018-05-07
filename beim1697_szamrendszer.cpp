#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>

using namespace std;

const unsigned int precision = 24;

map<char, unsigned int> szamJegyek_1;
map<unsigned int, char> szamJegyek_2;

void init(){
	for(char i = '0'; i <= '9'; ++i){
		szamJegyek_1[i] = i - '0';
	}

	for(char i = 'A'; i <= 'Z'; ++i){
		szamJegyek_1[i] = i - '7';
	}

	for(unsigned int i = 0; i <= 9; ++i){
		szamJegyek_2[i] = i + '0';
	}

	for(unsigned int i = 10; i < 36; ++i){
		szamJegyek_2[i] = i + '7';
	}
}

bool check(unsigned int forrasSzamRendszer, unsigned int celSzamRendszer, string myNum, ofstream &outputFile){
	if(forrasSzamRendszer <= 1){
		cout << "Hiba: helytelen forrasSzamRendszer" << endl;
		outputFile << "Hiba: helytelen forrasSzamRendszer" << endl;

		return false;
	}

	if(celSzamRendszer <= 1){
		cout << "Hiba: helytelen celSzamRendszer" << endl;
		outputFile << "Hiba: helytelen celSzamRendszer" << endl;

		return false;
	}

	for(unsigned int i = 0; i < myNum.size(); ++i){
		if(myNum[i] == ','){
			continue;
		}
		
		if(szamJegyek_1[myNum[i]] >= forrasSzamRendszer){
			cout << "Hiba: helytelen szam" << endl;
			outputFile << "Hiba: helytelen szam" << endl;

			return false;
		}
	}

	return true;
}

void invertString(string &str){
	unsigned int hossz = str.size();
	unsigned int hosszFele = hossz / 2;

	for(int i = 0; i < hosszFele; ++i){
		swap(str[i], str[hossz - 1 - i]);
	}
}

void getDecimalNumber(long double &decNum, unsigned int forrasSzamRendszer, string myNum){
	decNum = 0.0;
	unsigned int i = 0;

	while(myNum[i] != ',' && myNum[i] != '\0'){
		++i;
	}

	unsigned long int hatvany = 1;

	for(int k = i - 1; k >= 0; --k){
		decNum += szamJegyek_1[myNum[k]] * hatvany;
		hatvany *= forrasSzamRendszer;
	}

	++i;
	long double tort = 1 / (long double)forrasSzamRendszer;

	while(myNum[i] != '\0'){
		decNum += tort * szamJegyek_1[myNum[i]];
		tort *= 1 / (long double)forrasSzamRendszer;
		++i;
	}
}

string convertIntegerPart(unsigned long int egeszResz, unsigned int celSzamRendszer){
	string temp = "";

	while(egeszResz != 0){
		temp += szamJegyek_2[egeszResz % celSzamRendszer];
		egeszResz /= celSzamRendszer;
	}

	if(temp != ""){
		invertString(temp);
	}
	else{
		temp = "0";
	}

	return temp;
}

string convertFractionalPart(long double tortResz, unsigned int celSzamRendszer){
	string temp = ",";
	
	while(tortResz != 0.0 && temp.size() <= precision){
		tortResz *= celSzamRendszer;

		unsigned int egeszResz = tortResz;
		temp += szamJegyek_2[egeszResz];

		tortResz -= egeszResz;
	}

	if(temp == ","){
		temp += '0';
	}

	return temp;
}

void kiIr(unsigned int forrasSzamRendszer, unsigned int celSzamRendszer, string myNum, long double decNum, ofstream &outputFile){
	if(celSzamRendszer == 10){
		cout << forrasSzamRendszer << ' ' << myNum << ' ' << celSzamRendszer << ' ' << decNum << endl;
		outputFile << forrasSzamRendszer << ' ' << myNum << ' ' << celSzamRendszer << ' ' << decNum << endl;
	}
	else{
		unsigned long int egeszResz = decNum;
		long double tortResz = decNum - egeszResz;

		string aux_1 = convertIntegerPart(egeszResz, celSzamRendszer);
		string aux_2 = convertFractionalPart(tortResz, celSzamRendszer);

		cout << forrasSzamRendszer << ' ' << myNum << ' ' << celSzamRendszer << ' ' << aux_1 + aux_2 << endl;
		outputFile << forrasSzamRendszer << ' ' << myNum << ' ' << celSzamRendszer << ' ' << aux_1 + aux_2 << endl;
	}
}

void beOlvas(){
	ifstream inputFile("input.dat");

	ofstream outputFile;
	outputFile.open("output.dat");

	unsigned int forrasSzamRendszer;
	unsigned int celSzamRendszer;

	string myNum;

	long double decNum;

	while(inputFile >> forrasSzamRendszer && inputFile >> celSzamRendszer && inputFile >> myNum){
		if(!check(forrasSzamRendszer, celSzamRendszer, myNum, outputFile)){
			continue;
		}

		getDecimalNumber(decNum, forrasSzamRendszer, myNum);
		kiIr(forrasSzamRendszer, celSzamRendszer, myNum, decNum, outputFile);
	}

	outputFile.close();
}

int main(){
	init();
	beOlvas();

	return 0;
}
