#include <iostream> //for input/output
#include <fstream> //for file handling
#include <sstream> //for parsing lines
#include <iomanip> //for formatting numbers
#include <vector> //for storing name parts
using namespace std;

//function to calculate federal tax based on annual salary
double federalTax(double salary) 
{
    if (salary < 5000) return 0; // No tax for income below $5,000
    if (salary < 10000) return salary * 0.06; // 6% tax for $5,000–$9,999.99
    if (salary < 20000) return salary * 0.15; // 15% tax for $10,000–$19,999.99
    if (salary < 40000) return salary * 0.20; // 20% tax for $20,000–$39,999.99
    if (salary < 60000) return salary * 0.25; // 25% tax for $40,000–$59,999.99
    return salary * 0.30; // 30% tax for $60,000 and above
}

// Function to format name as "Last, First Middle_initial"
string formatName(vector<string> parts) {
    if (parts.size() == 2) { // only first and last
        return parts[1] + ", " + parts[0];
    } else if (parts.size() == 3) { // first, middle, last
        return parts[2] + ", " + parts[0] + " " + parts[1].substr(0,1) + ".";
    }
    return "Unknown"; // fallback
}

int main() {
    ifstream file("employees.txt"); // Open the input file
    if (!file) { // Check if file opened successfully
        cout << "Cannot open file.\n";  
        return 1; // Exit program if file cannot be opened
    }

string line; // To store each line from the file
    cout << fixed << setprecision(2); // Format numbers to 2 decimal places

    while (getline(file, line)) { // Read file line by line
        if (line.empty()) continue; // Skip empty lines

        stringstream ss(line); // split line
        string ssn, first, middleOrLast, retire;
        double salary;
        int covered;

        ss >> ssn >> first >> middleOrLast; // read first three parts

        vector<string> nameParts; // store name pieces
        nameParts.push_back(first);      
        nameParts.push_back(middleOrLast);

        string temp; // next part
        ss >> temp; // read next

        if (isdigit(temp[0])) { // if number → it is salary
            salary = stod(temp);
            ss >> covered >> retire; // read remaining
        } else { // else → it is last name
            nameParts.push_back(temp);
            ss >> salary >> covered >> retire;
        }

        // calculate deductions
        double fed = federalTax(salary); // federal tax
        double state = salary * 0.06; // state tax
        double health = covered * 100 * 12; // health insurance
        double retireDeduction = (retire == "Y") ? salary * 0.06 : 0; // retirement

        double netMonthly = (salary - (fed + state + health + retireDeduction)) / 12; // monthly net pay

        // print results
        cout << "SSN: " << ssn << "\n";
        cout << "Name: " << formatName(nameParts) << "\n";
        cout << "Federal Tax Deduction: $" << fed << "\n";
        cout << "State Tax Deduction: $" << state << "\n";
        cout << "Health Insurance Deduction: $" << health << "\n";
        cout << "Retirement Deduction: $" << retireDeduction << "\n";
        cout << "Monthly Net Pay: $" << netMonthly << "\n";
        cout << "---------------------------\n";
    }

    return 0; // program ends
}