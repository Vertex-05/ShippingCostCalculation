#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <typeinfo>
using namespace std;

class Parcel {
private:
	double weight;
	double height;
	double length;
	double width;
	char type;

public:
	// Default constructor
	Parcel() = default;
	Parcel(double wei, double hei, double len, double wid, char type) {
		weight = wei;
		height = hei;
		length = len;
		width = wid;
	}

	// Constructor for defined parcel types
	Parcel(double wei, char type) {
		if (type == 'a') {
			weight = wei; width = 24; height = 1; length = 32;
		}
		else if (type == 'b') {
			weight = wei; width = 14; height = 4; length = 23;
		}
		else if (type == 'c') {
			weight = wei; width = 20; height = 15; length = 35;
		}
		else if (type == 'd') {
			weight = wei; width = 35; height = 35; length = 75;
		}
	}
	// Constructor for Pallets
	Parcel(double wei, double hei, char type) {
		if (type == 'e') {
			weight = wei; height = hei; width = 110; length = 110;
		}
		else if (type == 'f') {
			weight = wei; height = hei; width = 106; length = 106;
		}
		else if (type == 'g') {
			weight = wei; height = hei; width = 120; length = 100;
		}
	}

	void setWeight(double wei) { weight = wei; }
	void setHeight(double hei) { height = hei; }
	void setLength(double len) { length = len; }
	void setWidth(double wid) { width = wid; }
	void setType(char ty) { type = ty; }
	double getWeight() { return weight; }
	double getHeight() { return height; }
	double getLength() { return length; }
	double getWidth() { return width; }
	double getVolume() { return width * length * height; }
	char getType() { return type; }
};


// Base class representing a shipment
class Shipment {
protected:
	string origin;
	string destination;
	double distance;
	double quotes;

	// Method to calculate distance based on a CSV file containing distance data
	double calcDistance() {
		// Container to store country names and distance mapping
		vector<string> countries;
		map<pair<string, string>, double> distanceMap;

		// Open the distance data file
		ifstream file("Asian_Countries_Distance.csv");
		if (!file.is_open()) {
			cerr << "Unable to open file Asian_Countries_Distance.csv!" << endl;
			exit(EXIT_FAILURE);
		}

		// Read the header row(country names)
		string line, cell;
		getline(file, line);
		stringstream headerStream(line);
		getline(headerStream, cell, ','); // Skip the first column
		while (getline(headerStream, cell, ',')) {
			countries.push_back(cell);
		}

		// Read the distance data
		while (getline(file, line)) {
			stringstream rowStream(line);
			string rowCountry;
			getline(rowStream, rowCountry, ','); // Country name in the row

			for (size_t i = 0; i < countries.size(); ++i) {
				string distanceStr;
				getline(rowStream, distanceStr, ',');

				if (!distanceStr.empty()) {
					double distance = stod(distanceStr); // string to double
					distanceMap[{rowCountry, countries[i]}] = distance;
				}
			}
		}

/***************************************************************************************
*    Title: c++ reading csv file
*    Author: kbyun
*    Date: Nov 12, 2013
*    Availability: https://stackoverflow.com/questions/19936483/c-reading-csv-file
***************************************************************************************/

		file.close();

		// Retrieve the distance for the specified origin and destination
		auto key = make_pair(origin, destination);
		distance = distanceMap[key];

		return distance;
	}

public:
	Shipment() = default;
	Shipment(string o, string d) {
		origin = o;
		destination = d;
	}

	void setOrigin(string o) { origin = o; }
	void setDest(string d) { destination = d; }
	string getOrigin() { return origin; }
	string getDest() { return destination; }
	double getDistance() {
		distance = calcDistance();
		return distance;
	}

	// Virtual function to calculate shipping quotes, overridden in subclasses
	double virtual getQuotes(Parcel* ptr) {
		return 0;
	}
};

// Subclass representing road shipment, calculates cost based on specific formular
class Road : public Shipment {
public:
	Road() = default;
	Road(string o, string d) {
		origin = o;
		destination = d;
	}

	// Overridden method to calculate road shipment quotes
	double getQuotes(Parcel* Ptr) {
		quotes = 1 * max(Ptr->getWeight(), 0.8*Ptr->getVolume()/6000) + 0.1 * getDistance();
		return quotes;
	}
};

// Subclass representing air shipment
class Air : public Shipment {
private:

public:
	Air() = default;
	Air(string o, string d) {
		origin = o;
		destination = d;
	}
	double getQuotes(Parcel* Ptr) {
		quotes = 20 * max(2 * Ptr->getWeight(), 3 * Ptr->getVolume()/6000) + 0.2 * getDistance();
		return quotes;
	}
};

// Subclass representing air shipment
class Ocean : public Shipment {
private:

public:
	Ocean() = default;
	Ocean(string o, string d) {
		origin = o;
		destination = d;
	}
	double getQuotes(Parcel* Ptr) {
		quotes = 0.8 * max(0.5 * Ptr->getWeight(), 0.3 * Ptr->getVolume()/6000) + 0.05 * getDistance();
		return quotes;
	}
};


// Base class for user information
class Client {
protected:
	string lastName;
	string firstName;
	char payMethod;

public:
	Client() = default;
	Client(string l, string f) {
		lastName = l;
		firstName = f;
	}

	void setLast(string l) { lastName = l; }
	void setFirst(string f) { firstName = f; }
	void setPay(char s) { payMethod = s; }
	string getLast(void) { return lastName; }
	string getFirst(void) { return firstName; }
	char getPay(void) { return payMethod; }

	// Virtual method for displaying user information, overridden in subclasses
	virtual void printInfo(Parcel* parcel_ptr, Shipment* shipment_ptr) {};
};

// Subclass representing private clients
class Private : public Client {
private:
	string gender;
	int YOB;

public:
	Private() = default;
	Private(string l, string f, string g, int Y) {
		lastName = l;
		firstName = f;
		gender = g;
		YOB = Y;
	}

	void setGender(string g) { gender = g; }
	void setYOB(int Y) { YOB = Y; }
	string getGender() { return gender; }
	int getYOB() { return YOB; }

	void printInfo(Parcel* parcel_ptr, Shipment* shipment_ptr) {
		cout << endl << endl;
		cout << "****************************************************************" << endl;
		cout << "\t\t\tSHIPMENT REPORT" << endl;
		cout << "****************************************************************" << endl;
		// personal Info
		cout << "Name: " << lastName << " " << firstName << endl;
		cout << "Gender: " << gender << endl;
		cout << "Age: " << 2024 - YOB << endl << endl;

		// parcel Info
		cout << "Parcel Information:" << endl;
		cout << "Weight: " << parcel_ptr->getWeight() << "kg" << endl;
		cout << "Height: " << parcel_ptr->getHeight() << "cm" << endl;
		cout << "Width: " << parcel_ptr->getWidth() << "cm" << endl;
		cout << "Length: " << parcel_ptr->getLength() << "cm" << endl;
		cout << endl;

		// shipment Info
		const char* mangledName = typeid(*shipment_ptr).name();

        int index = 0;
        while (std::isdigit(mangledName[index])) {
            ++index;
        }

		cout << "Shipment Information:" << endl;
		cout << "Origin: " << shipment_ptr->getOrigin() << endl;
		cout << "Destination: " << shipment_ptr->getDest() << endl;
		cout << "Mode: " << (mangledName + index) << endl;
		cout << endl;

		// payment Info
		cout << "Payment Information:" << endl;
		cout << "Quotes: " << shipment_ptr->getQuotes(parcel_ptr) << endl;
		cout << "Payment Method: ";
		if (payMethod == 'a')
			cout << "Credit Card" << endl;
		else if (payMethod == 'b')
			cout << "DuitNow" << endl;
		else if (payMethod == 'c')
			cout << "Alipay" << endl;
		else
			cout << "other" << endl;
		cout << endl << "Thank you for choosing Aseana Parcles!" << endl;
		cout << "****************************************************************" << endl;
	}

};

// Subclass representing business clients
class Business : public Client {
private:
	string company;

public:
	Business() = default;
	Business(string l, string f, string c) {
		lastName = l;
		firstName = f;
		company = c;
	}

	void setCompany(string c) {
		company = c;
	}
	void printInfo(Parcel* parcel_ptr, Shipment* shipment_ptr) {
		cout << endl << endl;
		cout << "****************************************************************" << endl;
		cout << "\t\t\tSHIPMENT REPORT" << endl;
		cout << "****************************************************************" << endl;
		// personal Info
		cout << "Personal Information: " << endl;
		cout << "Name: " << lastName << " " << firstName << endl;
		cout << "Company: " << company << endl;
		cout << endl;

		// parcel Info
		cout << "Parcel Information:" << endl;
		cout << "Weight: " << parcel_ptr->getWeight() << "kg" << endl;
		cout << "Height: " << parcel_ptr->getHeight() << "cm" << endl;
		cout << "Width: " << parcel_ptr->getWidth() << "cm" << endl;
		cout << "Length: " << parcel_ptr->getLength() << "cm" << endl;
		cout << endl;

		// shipment Info
		const char* mangledName = typeid(*shipment_ptr).name();

        int index = 0;
        while (std::isdigit(mangledName[index])) {
            ++index;
        }
		cout << "Shipment Information:" << endl;
		cout << "Origin: " << shipment_ptr->getOrigin() << endl;
		cout << "Destination: " << shipment_ptr->getDest() << endl;
		cout << "Mode: " << (mangledName + index) << endl;
		cout << endl;

		// payment Info
		cout << "Payment Information:" << endl;
		cout << "Quotes: " << shipment_ptr->getQuotes(parcel_ptr) << endl;
		cout << "Payment Method: ";
		if (payMethod == 'a')
			cout << "Credit Card" << endl;
		else if (payMethod == 'b')
			cout << "DuitNow" << endl;
		else if (payMethod == 'c')
			cout << "Alipay" << endl;
		else
			cout << "other" << endl;
		cout << endl << "Thank you for choosing Aseanas Parcles!" << endl;
		cout << "****************************************************************" << endl;
	}

};


// Function declaration
// create objects
Parcel createParcel(char serviceType);
Shipment* createShipment(string& origin, string& destination);

// Handling shipment validation
string getValidatedStringInput(const string& prompt, const string& validationFile);
void handleSpecialCityLogic(string& origin, string& destination);
bool checkModeAvailable(const string& origin, const string& destination, const string& filename);

// Handling error input
char getValidatedCharInput(const string& prompt, const string& validOptions);
int getValidatedIntInput(const string& prompt, int minVal, int maxVal);
double getValidatedDoubleInput(const string& prompt, double minVal);


int main() {
	cout << "Welcome to Aseana Parcels!\n" << endl;

	// service type (Pribate or Business)
	char serviceType = getValidatedCharInput(
		"a. Private\tb. Business\nPlease choose the type of shipping service: ",
		"ab"
	);

	// Collect client info
	cout << endl << "Personal Information:" << endl;
	string lastName = getValidatedStringInput("Please enter your last name: ", "");
	string firstName = getValidatedStringInput("Please enter your first name: ", "");

	// Dynamically create a client object based on the selected service type
	Client* client = nullptr;
	if (serviceType == 'a') {  // Private
		string gender = (getValidatedCharInput("a. Female\tb. Male\nPlease choose your gender: ", "ab") == 'a') ? "Female" : "Male";
		int yearOfBirth = getValidatedIntInput("Please enter your year of birth: ", 1800, 2023);

		// Create a 'Private' client object
		Private* privateUser = new Private(lastName, firstName, gender, yearOfBirth);
		client = privateUser;
	}
	else {  // Business
		string company = getValidatedStringInput("Please enter your company's name: ", "");

		// Create a 'Business' client object
		Business* businessUser = new Business(lastName, firstName, company);
		client = businessUser;
	}

	cout << endl << "Shipment Information:" << endl;

	// origin & destination
    string origin = getValidatedStringInput("Please enter your origin: ", "country.txt");
    string destination = getValidatedStringInput("Please enter your destination: ", "country.txt");
	while(true){
        // special city
        if ((origin == "china" || origin == "malaysia") && (destination == "china" || destination == "malaysia")){
            handleSpecialCityLogic(origin, destination);
            break;
        }
        else if (origin == destination){
            cout << endl << "Origin and destination can't be the same, please enter again:" << endl;
            origin = getValidatedStringInput("Please enter your origin: ", "country.txt");
            destination = getValidatedStringInput("Please enter your destination: ", "country.txt");
        }
        else
            break;

	}


    retry:
	cout << endl << "Parcel Information:" << endl;
	// create a parcel object
	Parcel parcel = createParcel(serviceType);

	// create a shipment object
	Shipment* shipment = createShipment(origin, destination);

	cout << endl << "The quote is " << shipment->getQuotes(&parcel) << endl;


    char confirm = getValidatedCharInput("Please confirm the quotation(y/n): ", "yn");
	if (confirm == 'y') {
		// choose a payment method
		char pay = getValidatedCharInput("a. Credit Card\tb. DuitNow\tc. Alipay\td. other\nPlease choose your payment method: ", "abcd");
		client->setPay(pay);

		// Confirm the shipment request
		char request = getValidatedCharInput("If you have paid, please enter 'y' (else enter 'n' for retrying): ", "yn");
		if (request == 'y') {
			// Print report
			client->printInfo(&parcel, shipment);
		}
		else
            goto retry;

	}
    else {
		// If user declines the quote, they can retry with a new parcel and shipment
		cout << endl << endl;
		goto retry;
	}

	// Clean up dynamically allocated memory
	delete shipment;
	delete client;

	return 0;
}


// Create a parcel object
Parcel createParcel(char serviceType) {
	double weight = getValidatedDoubleInput("Please enter the weight of your parcel: ", 0.0);
	char parcelType;

	if (serviceType == 'a') {
		parcelType = getValidatedCharInput(
			"a. A4 Envelope (32*24*1cm)\tb. One or two books (23*14*4cm)\tc. Shoe box (35*20*15cm)\nd. Moving box (75*35*35cm)\t"
			"z.Custom\nPlease choose the type of your parcel : ",
			"abcdz"
		);
	}
	else {
		parcelType = getValidatedCharInput(
			"d. Moving box (75*35*35cm)\te. MY-1 Pallet (110*110cm)\tf. MY-2 Pallet (106*106cm)\ng. MY-3 Pallet (120*100cm)\t"
			"z. Custom\nPlease choose the type of your parcel: ",
			"defgz"
		);
	}

	if (parcelType == 'z') {
		double height = getValidatedDoubleInput("Please enter the height(cm): ", 0.0);
		double length = getValidatedDoubleInput("Please enter the length(cm): ", 0.0);
		double width = getValidatedDoubleInput("Please enter the width(cm): ", 0.0);
		return Parcel(weight, height, length, width, parcelType);
	}
	else if (parcelType == 'e' || parcelType == 'f' || parcelType == 'g') {
		double height = getValidatedDoubleInput("Please enter the height(cm): ", 0.0);
		return Parcel(weight, height, parcelType);
	}
	else {
		return Parcel(weight, parcelType);
	}
}

// Create a shipment object
Shipment* createShipment(string& origin, string& destination) {
	bool roadAvailable = checkModeAvailable(origin, destination, "road_countries.csv");
	bool airAvailable = checkModeAvailable(origin, destination, "air_countries.csv");
	bool oceanAvailable = checkModeAvailable(origin, destination, "ocean_countries.csv");

	string prompt = "\nAvailable shipment modes:\n";
	string validOptions;

	if (roadAvailable) {
		prompt += "a. Road\n";
		validOptions += "a";
	}
	if (airAvailable) {
		prompt += "b. Air\n";
		validOptions += "b";
	}
	if (oceanAvailable) {
		prompt += "c. Ocean\n";
		validOptions += "c";
	}

	if (validOptions.empty()) {
		cerr << "No available shipment modes for the selected origin and destination." << endl;
		return nullptr;
	}

	prompt += "Please choose the shipment mode: ";
	char mode = getValidatedCharInput(prompt, validOptions);

	Shipment* shipment = nullptr;
	if (mode == 'a') {
		shipment = new Road(origin, destination);
	}
	else if (mode == 'b') {
		shipment = new Air(origin, destination);
	}
	else if (mode == 'c') {
		shipment = new Ocean(origin, destination);
	}

	return shipment;
}



// Check origin& destination
string getValidatedStringInput(const string& prompt, const string& validationFile) {
	string input;

	// Preload valid words from the file if a validation file is provided
	vector<string> validWords;
	if (!validationFile.empty()) {
		ifstream file(validationFile);
		if (!file.is_open()) {
			cerr << "Unable to open file: " << validationFile << endl;
			exit(EXIT_FAILURE); // Exit the program if the file cannot be opened
		}

		string line;
		while (getline(file, line)) {
			line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end()); // Remove whitespace
			for (char& c : line) {
				c = tolower(c); // Normalize to lowercase
			}
			validWords.push_back(line);
		}
		file.close();
	}

	// Input loop with validation
	while (true) {
		cout << prompt;
		cin >> input;
		for (char& c : input) {
			c = tolower(c); // Normalize input to lowercase
		}
		// Validate input if a validation file was provided
		if (validationFile.empty() || find(validWords.begin(), validWords.end(), input) != validWords.end()) {
			break; // Valid input
		}
		cout << "Invalid input. Please try again.\n";
	}

	return input;
}

// handle special city(china & malaysia)
void handleSpecialCityLogic(string& origin, string& destination) {
    string originFile = (origin == "china") ? "china_city.txt" : "malaysia_city.txt";
	string destinationFile = (destination == "china") ? "china_city.txt" : "malaysia_city.txt";

	origin = getValidatedStringInput("Please enter the city of your origin: ", originFile);
	destination = getValidatedStringInput("Please enter the city of your destination: ", destinationFile);

    while (true){
        if (origin == destination){
            cout << endl << "Origin and destination can't be the same, please enter again:" << endl;
            origin = getValidatedStringInput("Please enter the city of your origin: ", originFile);
            destination = getValidatedStringInput("Please enter the city of your destination: ", destinationFile);
        }
        else
            break;
    }

}

// Check air& ocean& road
bool checkModeAvailable(const string& origin, const string& destination, const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		cerr << "Error opening file: " << filename << endl;
		return false;
	}

	vector<string> countries;
	map<pair<string, string>, double> modeMap;

	string line, cell;
	getline(file, line);
	stringstream headerStream(line);
	getline(headerStream, cell, ',');
	while (getline(headerStream, cell, ',')) {
		countries.push_back(cell);
	}

	while (getline(file, line)) {
		stringstream rowStream(line);
		string rowCountry;
		getline(rowStream, rowCountry, ',');

		for (size_t i = 0; i < countries.size(); ++i) {
			string modeStr;
			getline(rowStream, modeStr, ',');
			if (!modeStr.empty()) {
				double mode = stod(modeStr);
				modeMap[{rowCountry, countries[i]}] = mode;
			}
		}
	}
	file.close();

	auto key = make_pair(origin, destination);
	if (modeMap.find(key) != modeMap.end()) {
		return modeMap[key] == 1;
	}
	else {
		cerr << "Route not found: " << origin << " -> " << destination << endl;
		return false;
	}
}



// Check service type& shipment mode& payment method input
char getValidatedCharInput(const string& prompt, const string& validOptions) {
	char input;
	while (true) {
		cout << prompt;
		cin >> input;
		if (validOptions.find(input) != string::npos && cin.peek() == '\n') {
			break;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input. Please try again.\n";
	}
	return input;
}

// Check Year of Birth input
int getValidatedIntInput(const string& prompt, int minVal, int maxVal) {
	int input;
	while (true) {
		cout << prompt;
		cin >> input;
		if (!cin.fail() && cin.peek() == '\n' && input >= minVal && input <= maxVal) {
			break;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input. Please enter a valid number(1900 - 2023).\n";

	}
	return input;
}

// Check weight& height& length& width input
double getValidatedDoubleInput(const string& prompt, double minVal) {
	double input;
	while (true) {
		cout << prompt;
		cin >> input;
		if (!cin.fail() && cin.peek() == '\n' && input > minVal) {
			break;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input. Please enter a valid number.\n";
	}
	return input;
}
