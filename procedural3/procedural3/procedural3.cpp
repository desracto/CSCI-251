#include <iostream> // USING : cout
#include <vector> // USING : vector
#include <string> // USING : string, getline
#include <fstream> // USING : fstream
#include <utility> // USING : pair
#include <sstream> // USING : STRINGSTREAM

using std::cout;
using std::cin;
using std::endl;

using std::vector;

using std::string;
using std::getline;

using std::pair;

using std::fstream;

using std::stringstream;

// FUNCTION DECLARATIONS
void startProgram();

void displayBus(vector < vector<pair<string, char>> >& bus);
void startPlan(vector < vector<pair<string, char>> >& bus, char currentStop, int& inPointer, int& familyCounter);

vector<pair<string, char>> grabListFromFile(const char currentStop, int& inPointer);

void seatPasseneger(vector<vector<pair<string, char>> >& bus, pair<string, char> passenger, int& familyCounter);
void removePassenger(vector<vector<pair<string, char>>>& bus, pair<string, char> passenger);
void removeFamily(vector<vector<pair<string, char>>>& bus, pair<string, char> passenger);

void savePlanToFile(const vector<vector<pair<string,char>>>& bus, const char & currentStop);
void resetPlan(vector < vector<pair<string, char>> >& bus, char& currentStop, int& inPointer);

void startAuditProgram();
void retrievePlan(const char& stopChoice);
// END OF FUNCTION DECLARATIONS

int main()
{
	startProgram();

	return 0;
}

void startProgram()
{
	// initalized once throughout the program. 
	// PASS AS REFERENCE ONLY
	// 
	// <first->string, second->char> 
	// first-> is the information about the passeneger. 
	// second-> is the passeneger itself

	vector < vector<pair<string, char>> > bus =    {	{ {"N", 'D'}, {"N",'O'}, {"N",'O'}, {"N",'R'} },
												    
														{ {"X", '_'}, {"X",'_'}, {"X",'_'}, {"X",'_'} },
														{ {"X", '_'}, {"X",'_'}, {"X",'_'}, {"X",'_'} },
														{ {"X", '_'}, {"X",'_'}, {"X",'_'}, {"X",'_'} },
														{ {"X", '_'}, {"X",'_'}, {"X",'_'}, {"X",'_'} },

														{ {"N", 'D'}, {"N",'O'}, {"N",'O'}, {"N",'R'} },

														{ {"X", '_'}, {"X",'_'}, {"X",'_'}, {"X",'_'} },
														{ {"X", '_'}, {"X",'_'}, {"X",'_'}, {"X",'_'} },
														{ {"X", '_'}, {"X",'_'}, {"X",'_'}, {"X",'_'} },
														{ {"X", '_'}, {"X",'_'}, {"X",'_'}, {"X",'_'} },

														{ {"N", 'D'}, {"N",'O'}, {"N",'O'}, {"N",'R'} },
	};

	// Pass these variables as reference ONLY
	char currentStop = '@';
	int fileReaderPointer = 0; // Location where the file reader stops in grabListFromFile()
	int familyCounter = 0;

	// constants for Switch Case
	const char CASE_NEXT_STOP = 'd';
	const char CASE_RESET_STOP = 'r';
	const char CASE_ADMIN_START = 'a';
	const char CASE_QUIT_PROGRAM = 'q';

	cout << "  Welcome!\n";
	cout << "  What would you like to do?";

	// menu - loop until quit
	bool quit = false;
	while (!quit)
	{
		displayBus(bus);
		cout << "\n  Current Stop: " << currentStop;

		cout << "\n  Press D to proceed to next stop\n";
		cout << "  Press S to save current simulation to plan.\n";
		cout << "  Press R to reset.\n";
		cout << "  Press Q to quit\n";
		cout << "  Press A to enter Admin Mode\n";
		cout << "  Choice: ";

		char choice;
		cin >> choice;

		switch (tolower(choice))
		{
			case CASE_NEXT_STOP:
			{
				if (currentStop >= 'F')
				{
					cout << "  You have reached the final stop. ";
					cout << "  Please reset the plan or insert a new set of passenegers.";
					break;
				}

				currentStop += 1;
				startPlan(bus, currentStop, fileReaderPointer, familyCounter);
				break;
			}
			case CASE_RESET_STOP:
			{
				resetPlan(bus, currentStop, fileReaderPointer);
				break;
			}
			case CASE_ADMIN_START:
			{
				startAuditProgram();
				break;
			}
			case CASE_QUIT_PROGRAM:
			{
				quit = true;
				return;
			}
			default:
			{
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				cout << "\n  INVALID CHOICE!";
				
				break;
			}
		}
	}
}

void displayBus(vector < vector<pair<string, char>> >& bus)
{
	cout << endl << endl;

	for (int row = 0; row < bus.size(); row++) // returns inner vector row
	{
		if (row == 0 || row == 5 || row == 10)
		{
			cout << "                                 DOOR\n";
			continue;
		}

		for (int col = 0; col < bus[row].size(); col++) // returns inner pair per index of inner vector
		{
			cout << "       " << bus[row][col].second;

			// DEBUGGING PRINT TO CHECK KEYS
			//cout << "       " << bus[row][col].first;
		}
		cout << "\n\n";
	}
}
void startPlan(vector < vector<pair<string, char>> >& bus, char currentStop, int& inPointer, int& familyCounter)
{
	// first -> STOP|STATE|DESTINATION
	// STOP = The current stop
	// STATE = (A)dd OR(R)emove
	// DESTINATION = Which stop they are being ADDED to or REMOVED from.This changes depending on state
	// 
	//second-> passenger = Lady/Man/Child/Teenager
	vector<pair<string, char>> passengerList = grabListFromFile(currentStop, inPointer);

	for (auto it = passengerList.begin(); it != passengerList.end(); it++)
	{
		// stop contains the stop from which either
		// a passenger is added or removed based on state (first[1])
		// With the A state, it adds the passenger
		// With the R state, it removes the passenger
		// The 2 index of the key will contain the stop its adding to

		string stop;
		stop.push_back(it->first[2]); 
		pair<string, char> passenger = { stop, it->second };

		if (it->first[1] == 'A') // ADDING PASSENGER
		{			
			// DEBUGGING PRINT
			// cout << "ADDING TO: ";
			// cout << stop << " ";
	
			seatPasseneger(bus, passenger, familyCounter);
		}
		else if (it->first[1] == 'R') // REMOVING PASSENGER
		{
			// DEBUGGING PRINT
			// cout << "REMOVING FROM: ";
			// cout << stop << " ";

			removePassenger(bus, passenger);
		}
	}

	savePlanToFile(bus, currentStop);
}
vector<pair<string, char>> grabListFromFile(const char currentStop, int& fileReaderPointer)
{
	std::fstream passList("passengerList.txt", std::ios::in);
	vector<pair<string, char>> passenegerList;

	// Set the stream pointer to the next destination in the file
	// So it doesnt need to read the previous stops.
	// And it doesn't trigeger the out-of-stop-scope check
	passList.seekg(fileReaderPointer, std::ios::beg);

	string line;
	while (getline(passList, line))
	{
		// skip comment lines
		if (line[0] == '#') continue;

		int delim_find = line.find(' ');

		// stop = STOP|STATE|DESTINATION
		// STOP = The current stop
		// STATE = (A)dd OR (R)emove 
		// DESTINATION = Which stop they are being ADDED to or REMOVED from. This changes depending on state

		string stop = line.substr(0, delim_find);

		// OUT-OF-STOP-SCOPE CHECK
		// not reading current stop passengers anymore 
		// when reader goes out-of-stop-scope
		if (stop[0] != currentStop)
			break;

		char passenger = line.substr(delim_find + 1, line.size())[0];

		if (passenger == 'E')
		{
			cout << "  END OF ROUTE!";
			break;
		}

		passenegerList.push_back({ stop, passenger });
	}
	// fileReaderPointer saves the location where the file stops reading. 
	// Lets the next iteration of this program to start exactly from
	// the next stop in the file.
	fileReaderPointer = passList.tellg();; // Location where the file reader stops in grabListFromFile()

	passList.close();
	return passenegerList;
}
void seatPasseneger(vector<vector<pair<string, char>>>& bus, pair<string, char> passenger, int& familyCounter)
{
	// Male Passengers	  :	COLUMNS  (A B C D)  | ROWS (6 7 8 9) |
	// Female Passenegers : COLUMNS  (A B C D)  | ROWS (1 2 3 4) | Exception: if 1 to 4 full, use 6 to 9
	// Child Passenegers  : COLUMNS  (A B)      | ROWS (1 to 9)  |
	// Family Passenegers : COLUMNS  (A B C D)  | ROWS ( 1 to 9) |

	char category = passenger.second;
	bool Seatedflag = false;
	int rowStart = 0;
	int colStart = 0;

	int rowLimit = 0;
	int colLimit = 0;

	// CONSTANT DECLARATION
	const char LADY = 'L'; 
	const char MEN = 'M'; 
	const char TEENAGER = 'T'; 
	const char CHILD = 'C'; 
	const char FAMILY = 'F'; 

	if (category == LADY)
	{
		rowStart = 1;
		rowLimit = 9;

		colStart = 0;
		colLimit = 4;
	}
	else if (category == MEN || category == TEENAGER)
	{
		rowStart = 6;
		rowLimit = 10;

		colStart = 0;
		colLimit = 4;
	}
	else if (category == CHILD)
	{
		rowStart = 1;
		rowLimit = 9;

		colStart = 0;
		colLimit = 2;
	}
	else if (category == FAMILY)
	{
		familyCounter += 1;

		// Stop will contain the stop they got on 
		// And the unique family identifier
		// To link the children to the mother
		string stop;
		stop.push_back(passenger.first[0]); // The stop they get on
		stop.push_back(category); // adds family category
		stop.append(std::to_string(familyCounter)); // adds the family identifier

		// Result example : BF1

		for (int f = 0; f < 3; f++)
		{
			if (f == 0) // first is mother
			{
				seatPasseneger(bus, { stop, LADY }, familyCounter);
				Seatedflag = true;
			}
			else // 2 and 3 are children
			{
				seatPasseneger(bus, { stop, CHILD }, familyCounter);
				Seatedflag = true;
			}
		}
	}

	for (int r = rowStart; r < rowLimit; r++)
	{
		for (int c = colStart; c < colLimit; c++)
		{
			bool ifOccupied = (bus[r][c].second != '_');

			if (!ifOccupied)
			{
				bus[r][c] = passenger;
				Seatedflag = true;
				
				return;
			}
		}
	}

	if (!Seatedflag)
	{
		cout << "  " << passenger.second << " from stop " << passenger.first[0] << " cannot be seated\n";
	}
}
void removePassenger(vector<vector<pair<string, char>>>& bus, pair<string, char> passenger)
{
	// Logic different for removing families
	// Calls a special function
	const char FAMILY_CATEGORY = 'F';
	if (passenger.second == FAMILY_CATEGORY)
	{
		passenger.first.push_back(FAMILY_CATEGORY);
		removeFamily(bus, passenger);
		return;
	}

	for (int r = 0; r < 11; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			if (bus[r][c] == passenger)
			{
				bus[r][c].first = "X";	
				bus[r][c].second = '_';

				cout << "\n  " << passenger.second << " from stop " << passenger.first << " has gotten off.\n";
				return;
			}
		}
	}
}
void removeFamily(vector<vector<pair<string, char>>>& bus, pair<string, char> passenger)
{
	// Match: Passenger.first-> STOP|F|IDEN
	// STOP = the stop the family got on
	// IDEN = The unique family identifier to link the children to the mother
	// This will match the first family it comes across in the bus
	string firstFamilyFoundMatchAssigned;
	bool flag = true; // change to false when match found
	
	LABEL:
	for (int r = 0; r < 11; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			bool familyFound = bus[r][c].first[0] == passenger.first[0] && bus[r][c].first[1] == passenger.first[1];

			if (flag && familyFound)
			{
				firstFamilyFoundMatchAssigned = bus[r][c].first[2];
				passenger.first.append(firstFamilyFoundMatchAssigned);

				flag = false; // switch if statement
				goto LABEL; // reset loop
			}

			if (!flag) 
			{
				bool familyKeyMatch = bus[r][c].first == passenger.first;

				if (familyKeyMatch)
				{
					bus[r][c].first = "X";
					bus[r][c].second = '_';
				}
			}
		}
	}
	cout << "\n  " << passenger.second << " from stop " << passenger.first[0] << " left the bus.";
}
void savePlanToFile(const vector<vector<pair<string,char>>>& bus, const char & currentStop)
{
	if (currentStop > 'E')
	{
		cout << "\n  You have reached the final stop!";
		return;
	}

	cout << "  Saving stop: " << currentStop << " to records";
	string fileName = "savedstops/Saved_Stop.txt";
	fileName.insert(21, 1, currentStop);

	fstream outPutFile(fileName, std::ios::out);

	for (int r = 0; r < bus.size(); r++)
	{
		for (int c = 0; c < bus[r].size(); c++)
		{
			outPutFile << bus[r][c].first << "|" << bus[r][c].second << ",";
		}

		outPutFile << "\n";
	}
}
void resetPlan(vector < vector<pair<string, char>> >& bus, char& currentStop, int& inPointer)
{
	for (int row = 0; row < bus.size(); row++) // return inner loop
	{
		if (row == 0 || row == 5 || row == 10) // door rows
			continue;

		for (int col = 0; col < bus[row].size(); col++) // return inner pair
		{
			bus[row][col].first = "X";
			bus[row][col].second = '_';
		}
	}

	// setting variables to default
	currentStop = '@';
	inPointer = 0;
}

void startAuditProgram()
{
	bool returnState = false;

	// CONSTANTS
	const char CASE_QUIT_STATE = 'q';
	const char STOP_A = 'a';
	const char STOP_B = 'b';
	const char STOP_C = 'c';
	const char STOP_D = 'd';
	const char STOP_E = 'e';

	while (!returnState)
	{
		cout << "\n\n  ADMIN MODE\n\n";
		cout << "  Which stop would you like to retrieve?\n";
		cout << "  Press Q to exit Admin Mode\n";
		cout << "  Stop: A/B/C/D/E\n";
		cout << "  Choice: ";

		char choice;
		cin >> choice;

		switch (tolower(choice))
		{
			case CASE_QUIT_STATE:
			{
				returnState = true;
				break;
			}
			case STOP_A:
			case STOP_B:
			case STOP_C:
			case STOP_D:
			case STOP_E:
			{
				retrievePlan(choice);
				break;
			}
			default: 
			{
				cout << "  Invalid Choice!";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}

	}
}

void retrievePlan(const char& stopChoice)
{	
	string fileName = "savedstops/Saved_Stop.txt";
	fileName.insert(21, 1, stopChoice);
	fstream file(fileName, std::ios::in);

	if (!file.is_open())
	{
		cout << "\n\n  The simulated stop " << stopChoice <<  " has not been saved yet.Please run the simulation and save it.";
		return;
	}

	cout << "\n  Retrieving plan: " << stopChoice;

	int row = 0;
	int col = 0;
	vector < vector<pair<string, char>>> displayingBus(11, vector<pair<string, char>> (4));

	// Getting each row from the file
	// Each row in the file is a row in the bus
	string line;
	while (getline(file, line)) // Row
	{
		const char delim_find = ',';
		stringstream fileRow (line);
		
		// Splitting file line at delim ','
		// to get all the pairs
		// Each pair is one col in the bus
		string fileCol;
		while (getline(fileRow, fileCol, delim_find)) // Col
		{
			const char pairSeperator = '|';
			int pairEnd = fileCol.find(pairSeperator);
			
			// Splitting fileCol into key-value 
			// and pushing it into the array
			while (pairEnd != std::string::npos)
			{
				string key = fileCol.substr(0, pairEnd);
				char value = fileCol.substr(pairEnd + 1, fileCol.size())[0];

				// Only one | per pair
				// No need for reassigning pairEnd
				// This is to end the while loop
				pairEnd = -1; 

				// cout << key << value << " ";
				displayingBus[row][col] = { key, value };
			}
			col++;
		}
		col = 0;
		row++;
	}

	displayBus(displayingBus);
}
