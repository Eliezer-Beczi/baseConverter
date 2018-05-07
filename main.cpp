#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

const unsigned precision = 24;

unordered_map<char, unsigned> char2digit;
unordered_map<unsigned, char> digit2char;

void init() {
	for(char i = '0'; i <= '9'; ++i) {
		char2digit[i] = i - '0';
	}

	for(char i = 'A'; i <= 'Z'; ++i) {
		char2digit[i] = i - '7';
	}

	for(unsigned i = 0; i <= 9; ++i) {
		digit2char[i] = i + '0';
	}

	for(unsigned i = 10; i < 36; ++i) {
		digit2char[i] = i + '7';
	}
}

bool check(unsigned forrasSzamRendszer, unsigned celSzamRendszer, string myNum, ofstream &outputFile) {
	if(forrasSzamRendszer <= 1) {
		cout << "Hiba: helytelen forrasSzamRendszer" << endl;
		outputFile << "Hiba: helytelen forrasSzamRendszer" << endl;

		return false;
	}

	if(celSzamRendszer <= 1) {
		cout << "Hiba: helytelen celSzamRendszer" << endl;
		outputFile << "Hiba: helytelen celSzamRendszer" << endl;

		return false;
	}

	for(unsigned i = 0; i < myNum.size(); ++i) {
		if(myNum[i] == ',') {
			continue;
		}

		if(char2digit[myNum[i]] >= forrasSzamRendszer) {
			cout << "Hiba: helytelen szam" << endl;
			outputFile << "Hiba: helytelen szam" << endl;

			return false;
		}
	}

	return true;
}

void getDecimalNumber(long double &decNum, unsigned forrasSzamRendszer, string myNum) {
	decNum = 0.0;
	unsigned i = 0;
	unsigned long long hatvany = 1;

	while(myNum[i] != ',' && myNum[i] != '\0') {
		++i;
	}

	for(int k = i - 1; k >= 0; --k) {
		decNum += char2digit[myNum[k]] * hatvany;
		hatvany *= forrasSzamRendszer;
	}

	++i;
	long double tort = static_cast<long double>(1) / forrasSzamRendszer;

	while(myNum[i] != '\0') {
		decNum += tort * char2digit[myNum[i]];
		tort *= static_cast<long double>(1) / forrasSzamRendszer;
		++i;
	}
}

string convertIntegerPart(unsigned long long egeszResz, unsigned celSzamRendszer) {
	string temp = "";

	while(egeszResz != 0) {
		temp += digit2char[egeszResz % celSzamRendszer];
		egeszResz /= celSzamRendszer;
	}

	if(temp != "") {
		reverse(temp.begin(), temp.end());
	}
	else {
		temp = "0";
	}

	return temp;
}

string convertFractionalPart(long double tortResz, unsigned celSzamRendszer) {
	string temp = ",";

	while(tortResz != static_cast<long double>(0) && temp.size() <= precision) {
		tortResz *= celSzamRendszer;

		unsigned egeszResz = tortResz;
		temp += digit2char[egeszResz];

		tortResz -= egeszResz;
	}

	if(temp == ",") {
		temp += '0';
	}

	return temp;
}

void kiIr(unsigned forrasSzamRendszer, unsigned celSzamRendszer, string myNum, long double decNum, ofstream &outputFile) {
	if(celSzamRendszer == 10) {
		cout << forrasSzamRendszer << ' ' << myNum << ' ' << celSzamRendszer << ' ' << decNum << endl;
		outputFile << forrasSzamRendszer << ' ' << myNum << ' ' << celSzamRendszer << ' ' << decNum << endl;
	}
	else {
		unsigned long long egeszResz = decNum;
		long double tortResz = decNum - egeszResz;

		string aux_1 = convertIntegerPart(egeszResz, celSzamRendszer);
		string aux_2 = convertFractionalPart(tortResz, celSzamRendszer);

		cout << forrasSzamRendszer << ' ' << myNum << ' ' << celSzamRendszer << ' ' << aux_1 + aux_2 << endl;
		outputFile << forrasSzamRendszer << ' ' << myNum << ' ' << celSzamRendszer << ' ' << aux_1 + aux_2 << endl;
	}
}

void beOlvas() {
	ifstream inputFile("input.dat");

	ofstream outputFile;
	outputFile.open("output.dat");

	unsigned forrasSzamRendszer;
	unsigned celSzamRendszer;

	string myNum;

	long double decNum;

	while(inputFile >> forrasSzamRendszer && inputFile >> celSzamRendszer && inputFile >> myNum) {
		if(!check(forrasSzamRendszer, celSzamRendszer, myNum, outputFile)) {
			continue;
		}

		getDecimalNumber(decNum, forrasSzamRendszer, myNum);
		kiIr(forrasSzamRendszer, celSzamRendszer, myNum, decNum, outputFile);
	}

	outputFile.close();
}

int main() {
	init();
	beOlvas();

	return 0;
}
