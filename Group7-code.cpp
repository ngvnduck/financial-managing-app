#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <iomanip>


using namespace std;

class Transaction {
protected:
    string namefile;
    double amount;
    string name;
    time_t curtime;
public:
    Transaction(string namefile, string name, double amount) {
        this->namefile = namefile;
        this->name = name;
        this->amount = amount;
        curtime = time(nullptr);
    }

    virtual void showDataSupport(){
        string line;
        cout << name << endl;
        ifstream File(namefile);
        while (getline(File, line)) {
            cout << line << endl;
        }
        cout << endl;
        File.close();
    }

    virtual void saveData() {
        ofstream outfile(namefile, ios::app);
        outfile << setprecision(20);
        tm *ltm = localtime(&curtime);
        outfile << name << ": $" << amount; 
        outfile << " at " << ltm->tm_hour << ":" << ltm->tm_min << " ";
        outfile << "day " << ltm->tm_mday << "/" << 1 + ltm->tm_mon << "/" << 1900 + ltm->tm_year << endl;
        outfile.close();
    }
};

class Expense : public Transaction {
private:
    string category;
public:
    Expense(string category) : Transaction("expenses.txt", category, 0.0) {
        this->category = category;
    }

    void addAmount(double a){
        amount = a;
    }
    
};

class Income : public Transaction {
private:
    string source;
public:
    Income(string source) : Transaction("incomes.txt", source, 0.0) {
        this->source = source;
    }

    void addAmount(double a){
        amount = a;
    }
    
};

class Budget : public Transaction {
private:
    string category;
public:
    Budget(string category) : Transaction("budget.txt", category, 0.0) {
        this->category = category;
    }

    void addAmount(double a){
        amount = a;
    }

};

double getSum(string namefile){
    double sum = 0;
    string line;
    ifstream File(namefile);
    while (getline(File, line)) {
        if (line.find(": $") != string::npos) {
            sum += stod(line.substr(line.find(": $") + 3));
        }
    }
    File.close();
    return sum;
}

void getBalance() {
	double nnn;
	nnn=getSum("incomes.txt") - getSum("expenses.txt") - getSum("budget.txt");
    cout << "Balance: $" << getSum("incomes.txt") - getSum("expenses.txt") - getSum("budget.txt") << endl;
    if (nnn <= 0) cout <<"-------------------------------------------------------------" << endl << "!!!!! !!!!! Chu y kiem soat chi tieu !!!!! !!!!!" <<endl<<"-------------------------------------------------------------"<< endl;
}

void resetData(string name) {
    string namefile;
    if (name == "E") namefile = "expenses.txt";
    else if (name == "I") namefile = "incomes.txt";
    else if (name == "B") namefile = "budget.txt";
    else if (name == "All") {
        ofstream file1("expenses.txt", ios::out | ios::trunc);
        file1.close();
        ofstream file2("incomes.txt", ios::out | ios::trunc);
        file2.close();
        ofstream file3("budget.txt", ios::out | ios::trunc);
        file3.close();
        cout << "Data reset successfully." << endl;
        return;
    }
    else {
        cout << "Error Category" << endl;
        return;
    }
    ofstream file(namefile, ios::out | ios::trunc);
    file.close();
    cout << "Data reset successfully." << endl;
}

void findTransactionsOnDate(string name,string namefile, string date){
    string line;
    cout << name << " in " << date << ":" << endl;
    int cnt = 0;
    ifstream File(namefile);
    while (getline(File, line)) {
        if (line.find("day") != string::npos) {
            if (line.substr(line.find("day") + 4).compare(date) == 0){
                cnt+=1;
                cout << line << endl;
            }
        }
    }
    if (!cnt != 0){
        cout << "No " << name << " in this day" << endl;
    }
    cout << endl;
    File.close();
}

void showData() {
    cout << setprecision(20);
    Expense expense("Expenses:");
    Income income("Incomes:");
    Budget budget("Budget:");
    expense.showDataSupport();
    income.showDataSupport();
    budget.showDataSupport();    
    getBalance();
}

void update_budget_item(const string& item, int amount) {
    // Open input file for reading
    ifstream infile("budget.txt");
    string line;
    bool found = false;
    
    // Open output file for writing
    ofstream outfile("budget_updated.txt");
    time_t curtime;
    curtime = time(nullptr);
    tm *ltm = localtime(&curtime);
    // Loop through each line in input file
    while (getline(infile, line)) {
        size_t pos = line.find(item);
        if (pos != string::npos) {
            // Found item in line
            found = true;
            size_t dollar_pos = line.find("$");
            if (dollar_pos != string::npos) {
                // Found dollar amount in line
                int current_amount = stoi(line.substr(dollar_pos + 1));
                if (current_amount >= amount) {
                    // Replace amount in line
                    current_amount-=amount;
                    line = item + ": $" + to_string(current_amount) + " at " + to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + " " + "day " + to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);
                    cout << "Updated: " << line << endl;
                    Expense exp(item);
                    exp.addAmount(amount);
                    exp.saveData();
                } else {
                    cout << "Amount is greater than current amount" << endl;
                }
            } else {
                cout << "Dollar amount not found in line" << endl;
            }
        }
        // Write line to output file
        outfile << line << endl;
    }
    
    infile.close();
    outfile.close();
    
    if (!found) {
        cout << "Budget not found" << endl;
    } else {
        // Replace original file with updated file
        remove("budget.txt");
        rename("budget_updated.txt", "budget.txt");
    }
}


int main() {
    int choice;
    cout << "****************************************************" << endl;
    cout << "*            PERSONAL FINANCE MANAGEMENT           *" << endl;
    cout << "****************************************************" << endl << endl;

    cout << "Welcome to Finance Manager. Please select an option:" << endl << endl;
    while(1){
        cout << "1. Add Income" << endl;
        cout << "2. Add Expense" << endl;
        cout << "3. Add Budget" << endl;
        cout << "4. Budget Spending" << endl;
        cout << "5. Show All Transactions" << endl;
        cout << "6. Search Transactions on a Specific Day" << endl;
        cout << "7. Clear Data" << endl;
        cout << "8. User manual" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                string source;
                double amount;
                cout << "Enter the source of the income: ";
                cin.ignore(1000,'\n');
                getline(cin, source);
                cout << "Enter the amount of the income: $";
                cin >> amount;
                Income inc(source);
                inc.addAmount(amount);
                inc.saveData();
                cout << "Add income successful" << endl;
                break;
            }
            case 2: {
                string category;
                double amount;
                cout << "Enter the category of the expense: ";
                cin.ignore(1000,'\n');
                getline(cin,category);
                cout << "Enter the amount of the expense: $";
                cin >> amount;
                Expense exp(category);
                exp.addAmount(amount);
                exp.saveData();
                cout << "Add expense successful" << endl;
                break;
            }
            case 3: {
                string category;
                double amount;
                cout << "Enter the category of the budget: ";
                cin.ignore(1000,'\n');
                getline(cin, category);
                cout << "Enter the amount of the budget: $";
                cin >> amount;
                Budget bud(category);
                bud.addAmount(amount);
                bud.saveData();
                cout << "Add budget successful" << endl;
                break;
            }
            case 4: {
                string category;
                double amount;
                cout <<  "Enter the category of the budget: ";
                cin.ignore(1000,'\n');
                getline(cin, category);
                cout << "Enter the amount of the spending: $";
                cin >> amount;
                update_budget_item(category,amount);
                break;
            }
            case 5: {
                cout << "Here are all the transactions:" << endl;
                showData();
                break;
            }
            case 6: {
                string date;
                cout << "Enter a date (dd/mm/yyyy): " ;
                cin.ignore(1000,'\n');
                getline(cin, date);
                findTransactionsOnDate("Expense","expenses.txt",date);
                findTransactionsOnDate("Income","incomes.txt",date);
                findTransactionsOnDate("Budget","budget.txt",date);
                break;
            }
            case 8: {
                ifstream file("UserManual.txt");
                string line;
                while(getline(file,line)){
                    cout << line << endl;
                }
                break;
            }
            case 7: {
                cout << "Choose a category to delete data from (E, I, B, All): ";
                string name;
                cin >> name;
                resetData(name);
                break;
            }
            case 9: {
                cout << "Have a nice day! ^^" << endl;
                return 0;
            }
            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
        cout << endl;
    }
    return 0;
}
