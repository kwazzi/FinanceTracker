// FinanceTracker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
// if you type in return with data in the txt file and then add a transaction, it doesnt save properly

class Transaction
{
public:
	std::string location;
	double amount;
	std::string date;
	int index = 0;

	Transaction()
	{

	}

	Transaction(std::string location1, double amount1, std::string date1)
	{
		location = location1;
		amount = amount1;
		date = date1;
	}

	void printName()
	{
		index = index;
		std::cout << index << ": " << "$" << amount << " was spent on " << date << " at " << location << ".\n";
	}
};

class FinanceTracker
{
public:
	std::vector <Transaction> transactions;
	std::fstream file;


	void open()
	{
	
		std::cout << "Hello, welcome to Finance Tracker!\nPlease type 'return' if you already have stored data.\n";
		std::cout << "Please type 'new' if you are a new user, or don't have any data\n";
		getInitialInput();
	}

	void getInitialInput() 
	{
		std::string input;
		std::cin >> input;
		if (input == "return")
			// read in previous data from text file
			readData(true);
		else if (input == "new")
			menu();
		else
		{
			std::cout << "Please enter a valid response\n";
			getInitialInput();
		}
	}

	void menu()
	{
		std::cout << "\nPlease press the number of the action you would like to do\n";
		std::cout << "1: Add Transaction\n2: Delete Transaction\n3: Edit Transaction\n4: Save Data\n5: See Transactions\n6: Exit program\n";
		std::string input;
		std::cin >> input;
		if (input == "1")
			addTransaction();
		else if (input == "2")
			deleteTransaction();
		else if (input == "3")
			editTransaction();
		else if (input == "4")
			saveData(true);
		else if (input == "5")
			printTransactions(true);
		else if (input == "6")
			exit();
		else
		{
			std::cout << "Please enter a valid response\n";
			menu();
		}
	}

	void addTransaction()
	{
		std::cout << "Where did you make your transaction?\n";
		std::string location;
		std::cin.ignore();
		getline(std::cin, location);

		std::cout << "How much did you spend?\n";
		std::string amount;
		getline(std::cin, amount);
		double dAmount;
		// changes it from a string to a double
		char* end;
		dAmount = strtod(amount.c_str(), &end);

		std::cout << "When did you make the transaction?\n";
		std::string date;
		getline(std::cin, date);

		Transaction t(location, dAmount, date);
		transactions.push_back(t);
		menu();
	}

	void deleteTransaction()
	{
		printTransactions(false);
		std::cout << "Please enter the number of the transaction you would like to delete\n";
		int number;
		std::cin.ignore();
		std::cin >> number;
		bool bValue = false;
		if (number <= transactions.size() && number > 0)
		{
			transactions.erase(transactions.begin() + number - 1);
			std::cout << "Transaction number " << number << " has been deleted.\n";
			printTransactions(true);
		}
		else 
		{
			std::cout << "Please enter a number that corresponds with a transaction\n";
			deleteTransaction();
		}
	}

	void editTransaction()
	{
		printTransactions(false);
		std::cout << "Enter the number of the transaction you would like to edit\n";
		int number;
		std::cin.ignore();
		std::cin >> number;
		if (number <= transactions.size() && number > 0)
		{
			std::cout << "Enter the number of the value you would like to edit\n";
			std::cout << "1: Location  2: Amount  3:  Date\n";
			int newNumber;
			std::cin.ignore();
			std::cin >> newNumber;
			
			if (newNumber <= 3 && newNumber > 0) {
				std::string newData;
				std::cin.ignore();
				std::cout << "Please enter the new value\n";
				getline(std::cin, newData);
				switch (newNumber) 
				{
				case 1:
					transactions[number - 1].location = newData;
					break;
				case 2:
					double dAmount;
					char* end;
					dAmount = strtod(newData.c_str(), &end);
					transactions[number -1].amount = dAmount;
					break;
				case 3:
					transactions[number-1].date = newData;
					break;
				default:
					break;
				}
			}
			else
			{
				std::cout << "Please enter a number that corresponds with a value\n";
				editTransaction(); 
			}
		}
		else
		{
			std::cout << "Please enter a number that corresponds with a transaction\n";
			editTransaction();
		}
		std::cout << "The transaction has been changed\n";
		printTransactions(true);
	}

	void saveData(bool bValue)
	{
		file.open("financeTracker.txt.txt", std::ios::app);
		if (file.is_open())
		{
			//readData(false);
			int index = 0;
			for (Transaction t : transactions)
			{
				index++;
				file << index << "," << t.location << "," << t.amount << "," << t.date << "\n";
			}
			std::cout << "Your data has been saved\n";
			file.close();
			if(bValue)
				menu();
		}
		else
		{
			std::cout << "Data could not be saved";
		}
	}

	void readData(bool bValue)
	{
		file.open("financeTracker.txt.txt", std::ios::in);
		std::string line;
		std::string comma = ",";

		// parse through line
		while (getline(file, line))
		{
			if (bValue)
			{
				Transaction t;
				char* end;
				// getting index
				std::string basicIndex = line.substr(0, line.find(comma));
				t.index = strtod(basicIndex.c_str(), &end);
				line.erase(0, line.find(comma) + 1);
				//getting location
				std::string location = line.substr(0, line.find(comma));
				t.location = location;
				line.erase(0, line.find(comma) + 1);
				// getting amount
				std::string basicAmount = line.substr(0, line.find(comma));
				t.amount = strtod(basicAmount.c_str(), &end);
				line.erase(0, line.find(comma) + 1);
				//getting date
				std::string date = line;
				t.date = date;

				transactions.push_back(t);
			}
			else
			{
				line.replace(0, line.length(), "");
			}
		}
		std::cout << "Here is your stored data:\n";
		file.close();
		printTransactions(true);
	}

	void exit()
	{
		std::cout << "Would you like to save before you exit?\nEnter 1 to save, and 2 to close without saving\n";
		int num;
		std::cin >> num;
		switch (num)
		{
		case 1:
			saveData(false);
			break;
		case 2:
			break;
		default:
			break;
		}
	}

	void printTransactions(bool bMenu)
	{
		int index = 0;
		for (Transaction trans : transactions)
		{
			index++;
			trans.index = index;
			trans.printName();
		}
		if(bMenu)
			menu();
	}
};


int main()
{
	FinanceTracker financeTracker;
	financeTracker.open();
}