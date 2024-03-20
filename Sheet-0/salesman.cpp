#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <iomanip>
using namespace std;

// <----------------- Helper Struct and functions ----------------->
/**
 * @brief return the line but bolded
 *        [Uses ANSI Escape Code, check if your terminal supports it or not]
 * 
 * @param line 
 * @return string 
 */
string underline(const string &line)
{
    return "\033[4m" + line + "\033[0m";
}

/**
 * @brief return the line but underlined
 *        [Uses ANSI Escape Code, check if your terminal supports it or not]
 * 
 * @param line 
 * @return string 
 */
string bold(const string &line)
{
    return "\e[1m" + line + "\e[0m";
}

// This is a struct that represents a sales record. 
// It stores information about the region number, salesman number, product code, and units sold.
struct record
{
    short regionNo;
    short salesmanNo;
    string productCode;
    int unitSold;
};

/*
 * converts a csv line to record struct
*/
record lineToRecord(string &line)
{
    stringstream s(line);
    string word;
    record r;

    getline(s, word, ',');
    r.regionNo = stoi(word);

    getline(s, word, ',');
    r.salesmanNo = stoi(word);

    getline(s, word, ',');
    r.productCode = word;

    getline(s, word, ',');
    r.unitSold = stoi(word);

    return r;
}

// <----------------- Class which reads from csv ----------------->
class dataReader
{
    ifstream fin;
    vector<record> records;
    void readData();

public:
    dataReader(const string &filename);
    vector<record> &getRecords()
    {
        return records;
    }
};

dataReader::dataReader(const string &filename)
{
    fin.open(filename, ios::in);
    readData();
}

/**
 * reads data from @filename, and save all the record in a vector 
*/
void dataReader::readData()
{
    string line;
    fin >> line; //taking heading

    while (fin >> line)
    {
        records.push_back(lineToRecord(line));
    }
}

// <----------------- Class which displays and writes ----------------->
class displayRecord
{
    string companyName;
    vector<record> &records;
    unordered_map<string, int> productCodeToCost;
    vector<vector<int>> sales;
    const short numRegion = 4;
    const short numSalesman = 6;
    void calculateSales();

    //the writer ofstream
    ofstream fout;

public:
    displayRecord(const string &comName, dataReader &d, const string &outputFilename);
    void displayAndPrint();
};

displayRecord::displayRecord(const string &comName, dataReader &d, const string &outputFilename)
    : companyName{comName}, records{d.getRecords()}
{
    //company defined cost of product id
    productCodeToCost["12345678A"] = 50;
    productCodeToCost["54326778B"] = 100;
    productCodeToCost["23679812C"] = 200;
    productCodeToCost["65478321D"] = 400;
    productCodeToCost["87678943E"] = 250;
    productCodeToCost["76579940F"] = 150;
    productCodeToCost["32671898G"] = 300;
    productCodeToCost["74526829H"] = 500;

    sales = vector<vector<int>>(numRegion + 1, vector<int>(numSalesman + 1, 0));
    fout.open(outputFilename, ios::out);
    calculateSales();
}

/*
 *  goes through record and calculate region and salesman wise revenue generated
*/
void displayRecord::calculateSales()
{
    for (auto record : records)
        sales[record.regionNo][record.salesmanNo] += productCodeToCost[record.productCode] * record.unitSold;
}

/*
* print all region and salesman wise revenue generated
*/
void displayRecord::displayAndPrint()
{
    cout << setw(45) << bold(underline(companyName)) << endl;
    fout << setw(27) << companyName << endl;
    fout << endl;

    // Get the current system time
    time_t currentTime = time(nullptr);

    // Convert the system time to a string representation
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

    // Print the current system date and time in the console
    cout << "Current system date and time: " << buffer << endl;
    cout << endl;
    fout << "Current system date and time: " << buffer << endl;
    fout << endl;

    int sumSales;
    for (int i = 1; i <= numRegion; i++)
    {
        cout << bold(underline("Region " + to_string(i))) << endl;
        cout << endl;
        fout << "Region " + to_string(i) << endl;
        fout << endl;
        fout << endl;

        sumSales = 0;
        for (int j = 1; j <= numSalesman; j++)
        {
            cout << "Salesman " << j << "\t\t\t\t\tRs." << sales[i][j] << "/-\n";
            cout <<endl;
            fout << "Salesman " << j << "\t\t\t\t\tRs." << sales[i][j] << "/-\n";
            fout << endl;
            fout << endl;
            fout << endl;
            sumSales += sales[i][j];
        }

        cout << bold("Total sale at Region " + to_string(i))
             << "\t\t" << bold("Rs." + to_string(sumSales) + "/-") << "\n\n";
        fout << "Total sale at Region " + to_string(i)
             << "\t\t" << "Rs." + to_string(sumSales) + "/-" << "\n\n";
        cout << "\t\t\t\t\t" << " Page : "<< i << "/" << numRegion <<"\n\n";
        cout<<"=================================================================================================="<<endl<<endl;
        fout << "\t\t\t\t\t\t\t" << " Page : "<< i << "/" << numRegion <<"\n\n";
        fout <<"================================================================================================="<<endl<<endl;
    }
}

const string INPUT_FILENAME = "./input_data_1.csv";
const string OUTPUT_FILENAME = "./output.txt";

int main()
{
    // Reader reads from csv
    dataReader reader(INPUT_FILENAME);

    // Display prints data in the given format
    displayRecord display("ABC Company", reader, OUTPUT_FILENAME);
    display.displayAndPrint();

    return 0;
}
