#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include<fstream>
#include <cstdlib>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}

string nCharString(int t_length, char t_character) { //Parameter: length, (Number of times character is repeated) / Parameter character (Character to be repeated).
	string returnString;						 //String to be returned.
	for (int i = 0; i < t_length; i++) {			 //Loop 'length' amount of times.  
		returnString += t_character;				 //Adding the 'character' to 'returnString' each time.
	}
	return returnString;						 //Return 'returnString'.
}

boolean checkIfDigitAndRange(string input, int lowEnd, int highEnd, int& choice) { // Function that checks the string input is in between low end and high end range and returns true if so,changing choice value. 
	if (isdigit(input[0])) {
		int temp = stoi(input);
		if (temp >= lowEnd && temp <= highEnd) {
			choice = temp;
			return true;
		}
		else{
			cout << "Out of range choice, try again!" << endl; // If digit but out of range
			return false;
		}
	}
	cout << "Digit was not entered, try again!" << endl;
	return false;	// If NOT digit
}

boolean checkIfInputYesOrNo(char input) { // If input Y or y return true
	if (input == 'y' || input == 'Y')
		return true;
	return false;
}
   
void choiceOne() { // If user choices option one on main menu. 
	cout << "List of quantity of each item purchased on a given day." << endl << nCharString(55, '-') << endl;
	CallProcedure("optionOne");		// calls python method
	cout << endl << "Would you like to rerun this program? Enter 'Y' for yes, enter anything else for no!" << endl;
}
 
void choiceTwo() { // If user choices option one on main menu. 
	string itemToFind;
	cout << "Enter an item to see how much that item sold on a given day:" << endl;
	cin >> itemToFind; // Gets name of item to find from user
	system("cls");
	cout << endl;	
	int timesSold = callIntFunc("optionTwo", itemToFind); // Calls python function 
	itemToFind[0] = std::toupper(itemToFind[0]);
	if (timesSold > 0){	// if the item was sold or not
		cout << itemToFind << " was sold " << timesSold << " times!" << endl;
	}
	else {
		cout << itemToFind << " was not sold at all!" << endl;
	}
	cout << endl << "Would you like to rerun this program? Enter 'Y' for yes, enter anything else for no!" << endl;
	
}

void choiceThree() {	// if user chooses option three on main menu.
	fstream file;	
	file.open("frequency.dat", ios::in); // open file
	if (file.is_open()) {
		string line;
		std::string delimiter = " "; // split file
		while (getline(file, line)) {
			cout <<  line.substr(0, line.find(delimiter)) << " " << nCharString(stoi(line.substr(line.find(delimiter))),'*') << endl; // prints histogram 
		}
	}
	else
		cout << "Unable to open file\n";
	file.close();
	cout << endl << "Would you like to rerun this program? Enter 'Y' for yes, enter anything else for no!" << endl;
}

void printMenu() { // function that prints out the main menu 
	cout << nCharString(78, '-') << endl;
	cout << "Choose an option, '1', '2', '3', or '4'" << endl;
	cout << "1: Produce list of number of times each item was purchased." << endl;
	cout << "2: Produce a number representing how many times a specific item was purchased." << endl;
	cout << "3: Produce a text-based histogram listing all items purchased." << endl;
	cout << "4: Exit Program." << endl;
	cout << nCharString(78, '-') << endl;
}

int getMenuAndChoice() {
	char yOrNo;	// Initalize YesOrNo choice to rerun program
	string input;	// Inialize user input
	CallProcedure("writeFile");	// Call python fucntion "writeFile" 
	while (true) {	// Programming running loop
		int choice = 0;	// User choice
		printMenu();	// Call printMenu()
		cin >> input;	// Get input
		system("cls");	
		if (checkIfDigitAndRange(input, 1, 4, choice)) { // Make sure user inputs a digit
			if (choice == 1) {	// User enters 1
				choiceOne();	// Function that prints choice one.
				cin >> yOrNo;	
				if (!checkIfInputYesOrNo(yOrNo)) // Check if input is y or Y
					break;	// if not y end loop
				system("cls");
			}
			else if (choice == 2) {	// User inputs 2 
				choiceTwo();	// Function that prints choice two
				cin >> yOrNo;	
				if (!checkIfInputYesOrNo(yOrNo))// Check if input is y or Y
					break;// if not y end loop
				system("cls");
			}
			else if (choice == 3) { // User inputs 3 
				choiceThree();	// Function that prints choice three
				cin >> yOrNo;
				if (!checkIfInputYesOrNo(yOrNo))// Check if input is y or Y
					break;// if not y end loop
				system("cls");
			}
			else {
				break; // if user enters 4 end program
			}
		}
	}
}


int main()
{
	getMenuAndChoice(); 

}