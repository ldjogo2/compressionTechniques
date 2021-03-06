#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>

using namespace std;

// vector for encodings and decodings
vector< pair<string,string> > encoder;

string encode(string fileName) {
    ifstream myfile;
    myfile.open(fileName, ios::in);
    if (!myfile) {
		cout << "Wrong file to encode!" << endl;
        return "";
	}
    
    string sLine = "";
    string stringToEncode = "";
    while(getline(myfile,sLine)) {
        stringToEncode += sLine;
    }
    myfile.close();

    // actually replacing what needs to be replaced
    int numMatch;
    string whatWeAreMatching;
    int sizeOfMatch;
    for (auto pairOfEncodings : encoder) {
        numMatch = 0;
        whatWeAreMatching = pairOfEncodings.first;
        sizeOfMatch = whatWeAreMatching.length();
        int loopLength = stringToEncode.length();
        
        for (int i = 0; i < loopLength; i++) {
            if (whatWeAreMatching[numMatch] == stringToEncode[i]) {
                numMatch++;
            }
            else {
                numMatch = 0;
            }

            if (numMatch == sizeOfMatch) {
                stringToEncode.replace(i - sizeOfMatch + 1, sizeOfMatch, pairOfEncodings.second);
                numMatch = 0;
                i = i - sizeOfMatch + pairOfEncodings.second.length();
            }
        }
    }

    return stringToEncode;
}

string decode(string encodedStr) {
    // actually replacing what needs to be replaced
    int numMatch;
    string whatWeAreMatching;
    int sizeOfMatch;
    for (auto pairOfEncodings : encoder) {
        numMatch = 0;
        whatWeAreMatching = pairOfEncodings.second;
        sizeOfMatch = whatWeAreMatching.length();
        int loopLength = encodedStr.length();
        
        for (int i = 0; i < loopLength; i++) {
            if (whatWeAreMatching[numMatch] == encodedStr[i]) {
                numMatch++;
            }
            else {
                numMatch = 0;
            }

            if (numMatch == sizeOfMatch) {
                encodedStr.replace(i - sizeOfMatch + 1, sizeOfMatch, pairOfEncodings.first);
                numMatch = 0;
                i = i - sizeOfMatch + pairOfEncodings.first.length();
            }
        }
    }

    return encodedStr;
}

void createDict (string dictFileName) {
    // here we create our dictionary from the dict file
    ifstream myfile;
    myfile.open(dictFileName, ios::in);
    if (!myfile) {
		cout << "Dictionary not created!" << endl;
        return;
	}
    
    string sLine = "";
    string delimiter = ":";
    int pos;
    while(getline(myfile,sLine)) {
        // get the position of the delimiter
        pos = sLine.find(delimiter);

        // get the strings needed
        string toReplace = sLine.substr(0,pos);
        string replacer = sLine.substr(pos+1, sLine.length());
    
        // store them in our dictionary
        encoder.push_back(make_pair(toReplace, replacer));
    }
    myfile.close();
}

// this program will take in the name of the file whose text we want to encode
// and the dictionary with the encoding representations (the dictionary will start with most frequent words)
// in dictionary key/value pairs will be separated by a colon : (this can be changed on lines 37,38 for diff delimiters)
int main (int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Wrong input format!!!" << endl;
        return 0;
    }
    std::string fileToEncode(argv[1]);
    std::string dict(argv[2]);

    // here we create our dictionary from the dict file
    createDict(dict);
    if (encoder.empty()) {
        cout << "Problem with dictionary creation!!!" << endl;
        return 0;
    }
    
    // Here we encode the string from the file and print it out
    string encodedString = encode(fileToEncode);
    cout << "Our encoded string:\n" << encodedString << endl;
    
    // here we decode the string we just encoded
    cout << "Our decoded string:\n" << decode(encodedString) << endl;

    return 0;
}