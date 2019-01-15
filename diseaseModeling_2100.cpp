#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

/*
* person structure :
*/
struct person {
private:
	char state = 'Q';			//State is initialized with 'Q' so we can catch init mistakes
	int initset = 0;			//To ensure the state is not changed again.
	int internalClock = 0;		//Keep track of the days
	int infectious_p = 0;		//Keep track of their infectious period
	int threshold = 0;			//Keep track of their threshold
	bool check = false;			//If their thresholdcount = threshold, then check true;
	int threscount = 1;			//Automatically already 1.

public:

	// _func_ : setinitstate - Set the initial state of the person. Once set it cannot be changed. It's state can ONLY be s, i, r, or v. bool
	bool setinitstate(char currstate) {
		if (initset == 0) {
			if (tolower(currstate) != 's' && tolower(currstate) != 'i' && tolower(currstate) != 'r' && tolower(currstate) != 'v') {
				return false;
			}
			else {
				initset = 1;
				this->state = tolower(currstate);
				if (this->state == 'i') {
					internalClock++;
				}
				return true;
			}
		}
		else {
			return false;
		}
	}

	// _func_ : changestate - change the state of the person depending on what state they are currently in. void
	void changestate() {
		if (getcurrentstate() == 's') {
			this->state = 'i';
			internalClock++;
		}
		else if (getcurrentstate() == 'i'){
			if (internalClock == infectious_p) {
				this->state = 'r';
			}
			internalClock++;
		}
		else if (getcurrentstate() == 'v') {
			this->state = 'v';
		}
	}

	// _func_ : getcurrentstate - Return the current state of the person.  
	char getcurrentstate() {
		return this->state;
	}

	// _func_ : markturned - if the amount of "marked"- meaning if the 's' is next to n amount of 'i'
	// then check will equal to true - meaning this 'e' will get geturned into an 'i'
	void markturned() { 
		if (threscount >= threshold) {
			this->check = true;
		}
		else {
			threscount++;
		}
	}


	// _func_ : markturned - get the value of check - true or false 
	bool getMarkturned(){ 
		return this->check;
	}
	
	// _func_ : setthreshold - set the threshold
	void setthreshold(int thres) {
		this->threshold = thres;
	}

	// _func_ : setinfectperiod - set the infectious period
	void setinfectperiod(int period) {
		this->infectious_p = period;
	}
};




//Functions - more info below
string menu2();
char menu();
bool exitprompt();
bool fileReader(fstream&);
vector<person> vfromString(string);
void change();
void display_vec(int);

vector<vector<vector<person>>> dayStack;						//3D vector. To store all days
vector<string> conditions;			//To hold the conditions AKA threshold, infectious period and display respectively


int main()
{
	fstream FILE_;
	bool leave = false;

	//If the user wants to leave the program, leave will be set to true.
	while (leave == false) {

		bool done = false;
		char choice;

		do {

			choice = menu();
			if (choice == '1') {
				FILE_.open("input1.txt");			//Open FILENAME1
				done = true;							//done = true, to break out of do-while loop.
			}
			else if (choice == '2') {
				FILE_.open("input2.txt");			//Open FILENAME2
				done = true;
			}
			else if (choice == '3') {
				FILE_.open("input3.txt");			//Open FILENAME3
				done = true;
			}
			else if (choice == '4'){
				FILE_.open(menu2().c_str());			//Open User file name string
			done = true;
			}
			else if (choice == '5')
			{
				cout << "\nThanks for using our model!\n";
				return 0;
			}
			else{
				cout << "\nPlease pick a valid option";
			}
		} while (done == false);


		//If file is open continue, else try again
		if (fileReader(FILE_)) {
			int display = (conditions[2].at(conditions[2].length() - 1)) - '0';			//Getting the display conditions from the global vector above
			change();																	//Run the model
			display_vec(display);														//Display model depeing on the display condition.
		}
		else {
			cout << "\nThe file didn't open, try again!";
		}

		//Prompt exit to make sure if the user really want to exit our amazing model.
		if (exitprompt() == false) {
			leave = true;
			cout << "\nThanks for using our model!\n";
			return 0;
		}
	}
	return 0;
}


/*
* _func_ : menu()
* return type : char
* param : none
* task : prompt user to pick a choice between 1-5
*/
char menu() {
	char choice;

	cout << "\nPick a file to run";
	cout << "\n(1).File 1";
	cout << "\n(2).File 2";
	cout << "\n(3).File 3";
	cout << "\n(4).Enter file name";
	cout << "\n(5).Quit" << endl;
	cout << " > ";
	cin >> choice;

	return choice;
}


/*
* _func_ : menu2()
* return type : string
* param : none
* task : prompt user to type in there own input file
*/
string menu2() {
	string name_o_file;
	cout << "\nPut the file locally and enter the name of .txt file with \".txt\" at the end." << endl;
	cout << " > ";
	cin >> name_o_file;
	return name_o_file;
}




/*
* _func_ :	exitprompt()
* return type : bool
* param : none
* task : Ask user if they want to exit
*/
bool exitprompt() {
	char answer;
	bool done = false;

	while (done == false) {
		cout << "\nThe model has ended, do you want to do something else? (Y/N) : ";
		cin >> answer;
		if (toupper(answer) == 'Y') {
			return true;
		}
		else if (toupper(answer) == 'N') {
			return false;
		}
		else {
			cout << "\nPlease enter a valid option.";
			done = false;
		}
	}
}


/*
* _func_ : fileReader()
* return type : bool
* param : fstream
* task : To read the file in to create the initial 2D vector of each input.
*/
bool fileReader(fstream& file) {

	vector<vector<person>> currvect_;

	if (file.fail()) {
		return false;
	}

	//To hold the line.
	string line;

	//while-loop to get the each line from file
	while (getline(file, line)) {
		if (toupper(line[0]) == 'T' || toupper(line[0]) == 'I' || toupper(line[0]) == 'D') {
			conditions.push_back(line);							//Reading the conditions.
		}
		else {
			std::string::iterator it = line.begin();			//for-loop with iterator to get rid of the ','
			for (; it != line.end(); it++)
			{
				if ((*it) == ',')
					line.erase(it);
			}

			std::vector<person> ROWvec = vfromString(line);		//creating the rows of person struct
			currvect_.push_back(ROWvec);						//pushing rows into the current vector - the 'day'
		}
	}
	dayStack.push_back(currvect_);							//Pushback to the first index to create the first day;
	file.close();											//Close file.
	return true;
}



/*
* _func_ :	vfromString()
* return type : vector<person>
* param : string
* task : Getting the whole line and separating each char in order to create
*		a 1D vector of person struct and returning it back.
*/
vector<person> vfromString(string row) {

	int threshold = (conditions[0].at(conditions[0].length() - 1)) - '0';
	int infecPeriod = (conditions[1].at(conditions[1].length() - 1)) - '0';

	//instant of vector
	std::vector<person> row_vec;

	for (int i = 0; i < row.length(); i++)
	{
		person p;					//creating person
		p.setinitstate(row[i]);		//setting  the initial state of person to s, i, r, or v
		p.setinfectperiod(infecPeriod);
		p.setthreshold(threshold);
		row_vec.push_back(p);		//pushing new person struct to end of row_vect

	}
	return row_vec;
}


/*
* _func_ : display_vec()
* return type :	void
* param : int
* task : To display the vector only. Take in the display conditions and print accordingly.
*/
void display_vec(int display)
{
	int countI = 0;						//Keep track of 'i' counts
	int countS = 0;						//Keep track of 's' counts
	int countR = 0;						//Keep track of 'r' counts
	int countV = 0;						//Keep track of 'v' counts
	int peakDay = 0;					//Keep track of what the peakday is
	int peakDayAmount = 0;				//Keep track of what the peakamount is


	//This for-loop will iterate through the 3D vector to display each day according to the display condition.
	for (int i = 0; i < dayStack.size(); i++) {
		if (i % display == 0) {
			cout << "\nDay " << i << endl;							//output i day
			for (auto row : dayStack[i]) {
				for (auto col : row) {

					//outputting the current state to console. 
					cout << col.getcurrentstate() << " ";

					//Count the i's and r's of one day
					if (col.getcurrentstate() == 'i') {
						countI++;
					}
					if (col.getcurrentstate() == 'r') {
						countR++;
					}

				}
				cout << endl;
			}
			cout << endl;

			//If the count of i's equal to 0
			if (countI == 0) {
				cout << "\n\nOutbreak Ends : " << "Day " << endl;
				cout << "Peak Day : Day " << peakDay << endl;
				cout << "\tS : " << countS << endl;
				cout << "\tI : " << countI << endl;
				cout << "\tR : " << countR << endl;
				cout << "\tV : " << countV << endl;
			}

			//If the last peakday is less then the current infected count, 
			//then the current infected count if the new peak day.
			if (peakDayAmount < countI) {
				peakDayAmount = countI;
				peakDay = i;
			}

			//Set all counts to zero to run the next day.
			countI = 0;
			countR = 0;
		}
	}

	//After running the model. Clear everything. Get it ready for the next model.
	conditions.clear();
	dayStack.clear();
}


/*
* _func_ : change()
* return type : none
* param : int, int
* task : This funciton takes in the threshold and the infectious period of the model
*			and apply it to how the state will change.
*			It will do this until all the day is made and stored into a 3D vector.
*/
void change() {

	//To keep track of the 'i' count. This will be our breakout condition in the do while loop below.
	int countI = 0;

	do {
		vector<vector<person>> newDay = dayStack[dayStack.size() - 1];			//newDay will hold the last day of the dayStack.
		countI = 0;												//count will get set to 0 every day.

		for (int i = 0; i < newDay.size(); i++) {
			for (int g = 0; g < newDay[0].size(); g++) {
				if (newDay[i][g].getcurrentstate() == 's') {

					if (g == newDay[0].size() - 1) {

						//Check right of last column
						if (newDay[i][0].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}
					else {
						//Check right not last column
						if (newDay[i][g + 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}

					if (g == 0) {

						//Check left first column
						if (newDay[i][newDay[0].size() - 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}


						//Check right of first column
						if (newDay[i][g + 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}
					else {

						//Check left not first column
						if (newDay[i][g - 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}

					}


					if (i != 0 && g == newDay[0].size() - 1 && i != newDay[0].size() - 1) {
						//Check 1 row down from the right of last column
						if (newDay[i + 1][0].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
						//Check 1 row down from the right of last column
						if (newDay[i - 1][0].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}

					
					//Check up
					if (i == 0) {
						if (newDay[i + 1][g].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}
					else {
						if (newDay[i - 1][g].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}

					//Check down
					if (i == newDay[0].size() - 1) {
						if (newDay[i - 1][g].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}
					else {
						if (newDay[i + 1][g].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}


					//Check bottom right corner if [0][0]
					if (i == 0 && g == 0) {
						if (newDay[i + 1][g + 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}


					//Check upper left corner if [9][9]
					if (i == 9 && g == 9) {
						if (newDay[i - 1][g - 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}

					//Check bottom left corner if [0][9]
					if (i == 0 && g == 9) {
						if (newDay[i + 1][g - 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}

					//Check upper right corner if [9][0]
					if (i == 9 && g == 0) {
						if (newDay[i - 1][g + 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}



					//MID
					if (i != 0 && g != 0 && i != newDay[0].size() - 1 && g != newDay[0].size() - 1) {
						//bottom right
						if (newDay[i + 1][g + 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
						//top right
						if (newDay[i - 1][g + 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}

						//bottom right
						if (newDay[i + 1][g - 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
						//top right
						if (newDay[i - 1][g - 1].getcurrentstate() == 'i') {
							newDay[i][g].markturned();
						}
					}
				}
			}
		}

		//if it's state is an i then call changestate() to see if it's qualify to change.
		for (int i = 0; i < newDay.size(); i++) {
			for (int g = 0; g < newDay[0].size(); g++) {
				if (newDay[i][g].getcurrentstate() == 'i') {
					newDay[i][g].changestate();
				}
			}
		}



		//If the check == true, meaning if the person was marked enough times to equal to the 
		//threshold then check should be true therefore it is able to change it's state. 
		for (int i = 0; i < newDay.size(); i++) {
			for (int g = 0; g < newDay[0].size(); g++) {
				if(newDay[i][g].getcurrentstate() == 's'){
					if (newDay[i][g].getMarkturned() == true) {
						newDay[i][g].changestate();
					}
				}
			}
		}

		for (int i = 0; i < newDay.size(); i++) {
			for (int g = 0; g < newDay[0].size(); g++) {
				if (newDay[i][g].getcurrentstate() == 'i') {
					countI++;

				}
			}
		}

		//Push the newly changed chart to the 3D vector.
		dayStack.push_back(newDay);

	} while (countI > 0);
}



