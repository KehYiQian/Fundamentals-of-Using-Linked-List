#define _CRT_SECURE_NO_WARNINGS
#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include    <iomanip>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"


using namespace std;

bool ReadFile(string, List*);
bool DeleteRecord(List*, char*);
bool Display(List, int, int);
bool InsertBook(string, List*);
bool SearchStudent(List*, char* id, LibStudent&);
bool computeAndDisplayStatistics(List*);
bool printStuWithSameBook(List*, char*);
bool displayWarnedStudent(List*, List*, List*);
int menu();
bool printList(List);

int main() {
	int source, detail, menu_choice;
	List* student_list = new List;
	List* warned_list1 = new List;
	List* warned_list2 = new List;
	LibStudent* student = new LibStudent;
	char* delete_id = new char[10];
	char* Stud = new char[10];
	char* call = new char[20];
	do {
		menu_choice = menu();
		if (menu_choice == 1) {
			cout << "\n\nREAD FILE\n\n";
			if (!ReadFile("student.txt", student_list))
			{
				cout << "The records are unsuccessfully read.\n" << endl;
			}
		}
		else if (menu_choice == 2) {
			cout << "\n\nDELETE RECORD\n\n";
			DeleteRecord(student_list, delete_id);
		}
		else if (menu_choice == 3) {
			cout << "\n\nSEARCH STUDENT\n\n";
			//pass list, a empty char array pointer and a empty LibStudent type pointer which will be filled with found student
			if (SearchStudent(student_list, Stud, *student))
			{
				cout << "\nStudent Information: \n";
				cout << "Name: " << student->name << endl;
				cout << "ID: " << student->id << endl;
				cout << "Course: " << student->course << endl;
				cout << "Phone No: " << student->phone_no << endl;
			}
		}
		else if (menu_choice == 4) {
			cout << "\n\nINSERT BOOK\n\n";
			if (InsertBook("book.txt", student_list))
			{
				cout << "\nBook inserted successfully to student list\n\n\n";
			}
		}
		else if (menu_choice == 5) {
			cout << "\n\nDISPLAY OUTPUT\n\n";
			cout << "Where do you want to display the output (1 - File / 2 - Screen): ";
			cin >> source;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Do you want to display book list for every student (1 - YES / 2 - NO): ";
			cin >> detail;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (Display(*student_list, source, detail))
			{
				cout << "Print successfully!!" << endl;
			}
		}
		else if (menu_choice == 6) {
			cout << "\n\nCOMPUTE AND DISPLAY STATISTICS\n\n";
			computeAndDisplayStatistics(student_list);
		}
		else if (menu_choice == 7) {
			cout << "\n\nSTUDENT WITH SAME BOOK\n\n";
			//pass list and char array pointer
			printStuWithSameBook(student_list, call);
		}
		else if (menu_choice == 8) {
			cout << "\n\nDISPLAY WARNED STUDENT\n\n";
			displayWarnedStudent(student_list, warned_list1, warned_list2);
		}

	} while (menu_choice != 9);

	cout << "\n\nThank you for using our program.\n";
	system("pause");
	delete student_list;
	delete warned_list1;
	delete warned_list2;
	delete student;
	delete[] delete_id;
	delete[] Stud;
	delete[] call;

	return 0;
}

bool ReadFile(string filename, List* list)
{
	int count = 0;
	string id, name, course, phonenumber, line;
	ifstream file(filename);
	if (!file.is_open())
	{
		cout << "The File is  not exist!!" << endl;
		return false;
	}
	else
	{
		while (!file.eof())
		{
			count++;
			getline(file, line);
			if (line.empty()) continue;// in order to skip the empty line for second round

			id = line.substr(line.find('=') + 2);//Extract the student id

			getline(file, line);
			name = line.substr(line.find('=') + 2); // Extract the name

			getline(file, line);
			course = line.substr(line.find('=') + 2); // Extract the course

			getline(file, line);
			phonenumber = line.substr(line.find('=') + 2); // Extract the phone number


			LibStudent* student = new LibStudent();
			strcpy(student->id, id.c_str());
			strcpy(student->name, name.c_str()); // use c_str to convert string to const char*
			strcpy(student->course, course.c_str());
			strcpy(student->phone_no, phonenumber.c_str());

			for (int i = 0; i < list->size(); i++)
			{
				if (strcmp(list->find(i)->item.id, student->id) == 0)
				{
					cout << "The student id is duplicate!! " << endl;
					return false;
				}
			}
			list->insert(*student);

			delete student;
		}
		cout << endl << list->size() << " records have been successfully read.\n\n" << endl;
		return true;
	}
}

bool DeleteRecord(List* list, char* id)
{
	if (list->empty())
	{
		cout << "Student deleted unsuccessfully because the student list is empty!!" << endl << endl;
		return false;
	}
	cout << "Kindly insert the id of the student that you want to delete: ";
	cin >> id;
	int a = 0;
	while (id[a] != '\0')//check the validation of id
	{
		if (!isdigit(id[a]))
		{
			cout << "The format of the inserted student id is invalid. Please check again!!" << endl;
			return false;
		}
		a++;
	}
	for (int i = 1; i <= list->size(); i++)
	{
		if (i <= list->size() && strcmp(list->find(i)->item.id, id) == 0)//make sure the validation of i and meanwhile do the comparison with the id
		{
			cout << "The student information of " << list->find(i)->item.name << " is already delete." << endl;
			list->remove(i);
			return true;
		}
	}
	cout << "The student id is not found in the list!! Please check again!" << endl;
	return false;
}

bool SearchStudent(List* list, char* id, LibStudent& stu)
{
	if (list->empty())
	{
		cout << "Student searched unsuccessfully because the student list is empty!!" << endl << endl;
		return false;
	}
	int num, a = 0;
	cout << "Insert the student id you want to find: ";
	cin >> id;

	while (id[a] != '\0')//check the validation of id
	{
		if (!isdigit(id[a]))
		{
			cout << "The format of the inserted student id is invalid. Please chack again!!" << endl;
			return false;
		}
		a++;
	}
	for (num = 1; num <= list->size(); num++)
	{
		list->get(num, stu);
		if (strcmp(id, stu.id) == 0)//return true if two id are matched
		{
			return true;
			break;
		}
	}
	cout << "The id is not matched with any student in the list!" << endl;
	return false;
}

bool InsertBook(string filename, List* list) {

	if (list->empty())
	{
		cout << "Book inserted unsuccessfully to student list because no student is defined at the list!!" << endl << endl;
		return false;
	}

	// Check for duplicated records
	Node* check;
	check = list->head;
	while (check != NULL) {
		if (check->item.book[0].title[0] != ' ') {
			cout << "Error! Duplicated records trying to be inserted!!" << endl << endl;
			return false;
		}
		check = check->next;
	}



	ifstream InFile;
	Node* cur;
	char temp[500];
	char ReadID[10];

	// Variables used to calculate the fine for each book before inserting the read book info
	int current_day = 29, current_month = 3, current_year = 2020;
	double BookFine = 0.00;

	InFile.open(filename);

	// Check if the text file can be opened or not
	if (!InFile) {
		cout << "Unable to open the file " << filename << "." << endl;
		return false;
	}
	else {
		while (!(InFile.eof())) {
			LibBook* ReadBook = new LibBook; // Create a new LibBook object for every insertion of book

			InFile >> ReadID;
			// To ensure the first element of id array to be number only but not whitespace
			if (!isdigit(ReadID[0]))
				break;


			int q = 0;
			int m = 0;
			InFile >> temp; // To store one or more than one author names temporarily for a book

			string author = temp;//store the temp become string 
			size_t pos = 0;
			size_t endPos;
			int index = 0;
			// Initialise the pointer that points to author names for each book
			while ((endPos = author.find("/", pos)) != string::npos)
			{
				ReadBook->author[index] = new char[endPos - pos + 1];//define the size of new memory
				index++;
				pos = endPos + 1;
			}
			ReadBook->author[index] = new char[author.length() - pos + 1];//for last author

			// Read author names into separate element of array
			for (int i = 0; i < strlen(temp) + 1; i++) {

				if (temp[i] == '\0') {
					*(ReadBook->author[m] + q) = '\0';
					break;
				}
				if (temp[i] == '_') {
					*(ReadBook->author[m] + q) = ' ';
					q++;
				}
				else if (temp[i] != '/') {
					*(ReadBook->author[m] + q) = temp[i];
					q++;
				}
				else {
					*(ReadBook->author[m] + q) = '\0';
					q = 0;
					m++;
				}
			}

			// To read and store book info temporarily
			InFile >> ReadBook->title;
			for (int i = 0; i < strlen(ReadBook->title); i++) {
				if (ReadBook->title[i] == '_')
					ReadBook->title[i] = ' ';
			}
			InFile >> ReadBook->publisher;
			for (int i = 0; i < strlen(ReadBook->publisher); i++) {
				if (ReadBook->publisher[i] == '_')
					ReadBook->publisher[i] = ' ';
			}
			InFile >> ReadBook->ISBN;
			InFile >> ReadBook->yearPublished;
			InFile >> ReadBook->callNum;
			InFile >> ReadBook->borrow.day;
			while (!(InFile >> ReadBook->borrow.month)) {
				InFile.clear();
				InFile.ignore();
			}
			while (!(InFile >> ReadBook->borrow.year)) {
				InFile.clear();
				InFile.ignore();
			}
			InFile >> ReadBook->due.day;
			while (!(InFile >> ReadBook->due.month)) {
				InFile.clear();
				InFile.ignore();
			}
			while (!(InFile >> ReadBook->due.year)) {
				InFile.clear();
				InFile.ignore();
			}
			InFile.ignore();

			// To calculate fine before inserting
			BookFine = 0.50 * ((current_day + (current_month - 1) * 30 + current_year * 12 * 30) - (ReadBook->due.day + (ReadBook->due.month - 1) * 30 + ReadBook->due.year * 12 * 30));
			if (BookFine <= 0) BookFine = 0.00;


			// Inserting the the readbook info into every LibStudent element read from Linked List
			cur = list->head;
			int a;
			while (cur != NULL) {
				a = 0;
				// Find the correct stundet based on id read to insert read book info
				if (strcmp(cur->item.id, ReadID) == 0) {
					// Check if it is an empty record, if yes then move to next element of book array
					while (strcmp(cur->item.book[a].title, " ") != 0)
						a++;
					if (a > 14)
						a = 14;
					strcpy(cur->item.book[a].title, ReadBook->title);
					for (int y = 0; y < 10; y++)
					{
						cur->item.book[a].author[y] = ReadBook->author[y];
					}
					strcpy(cur->item.book[a].publisher, ReadBook->publisher);
					strcpy(cur->item.book[a].ISBN, ReadBook->ISBN);
					cur->item.book[a].yearPublished = ReadBook->yearPublished;
					strcpy(cur->item.book[a].callNum, ReadBook->callNum);
					cur->item.book[a].borrow = ReadBook->borrow;
					cur->item.book[a].due = ReadBook->due;
					cur->item.book[a].fine = BookFine;
					cur->item.totalbook++;
				}

				cur = cur->next;
			}
			delete ReadBook;
		}

		InFile.close();

		// Calculate and update the Total Fine for each student
		cur = list->head;
		while (cur != NULL) {
			cur->item.calculateTotalFine();
			cur = cur->next;
		}

		return true;
	}
}

bool Display(List list, int source, int detail)
{
	Node* cur;
	int j, count = 0;
	ofstream file("student_info.txt");
	ofstream output("student_booklist.txt");
	if ((source != 1 && source != 2) || (detail != 1 && detail != 2) || !source || !detail)
	{
		cout << "Invalid Input. Please check again!!\n" << endl;
		return false;
	}
	if (list.empty())
	{
		cout << "\n\nCannot print from an empty list" << endl;
		return false;
	}

	cur = list.head;
	while (cur != NULL)
	{
		if (source == 1)
		{
			if (detail == 1)
			{
				output << "\nStudent " << ++count;
				cur->item.print(output);
				for (int i = 0; i < cur->item.totalbook; i++)
				{
					cur->item.book[i].print(output);
				}
				output << "\n***********************************************************************************" << endl;

			}
			else if (detail == 2)
			{
				file << "\nStudent " << ++count;
				cur->item.print(file);
				file << "\n***********************************************************************************" << endl;
			}
			cur = cur->next;

		}
		else if (source == 2)
		{
			if (detail == 1)
			{
				cout << "STUDENT " << ++count << endl << endl;
				cout << "Name: " << cur->item.name << endl;
				cout << "ID: " << cur->item.id << endl;
				cout << "Course: " << cur->item.course << endl;
				cout << "Phone No: " << cur->item.phone_no << endl;
				cout << setprecision(2) << fixed;
				cout << "Total Fine: RM" << cur->item.total_fine << endl << endl;
				cout << "BOOK LIST:" << endl;
				for (int i = 0; i < cur->item.totalbook; i++)
				{
					j = 0;
					cout << endl;
					cout << "BOOK " << i + 1 << endl << endl;
					cout << "Title: " << cur->item.book[i].title << endl;
					cout << "Author: ";
					for (int j = 0; cur->item.book[i].author[j] != NULL; j++)
					{
						cout << cur->item.book[i].author[j];
						if (cur->item.book[i].author[j + 1] != NULL)
							cout << "  /  ";
					}
					cout << endl;
					cout << "Publisher: " << cur->item.book[i].publisher << endl;
					cout << "Year Published: " << cur->item.book[i].yearPublished << endl;
					cout << "ISBN: " << cur->item.book[i].ISBN << endl;
					cout << "Call Number: " << cur->item.book[i].callNum << endl;
					cout << "Borrow Date: " << cur->item.book[i].borrow.day << "/" << cur->item.book[i].borrow.month << "/" << cur->item.book[i].borrow.year << endl;
					cout << "Due Date: " << cur->item.book[i].due.day << "/" << cur->item.book[i].due.month << "/" << cur->item.book[i].due.year << endl;
					cout << "Fine: RM" << cur->item.book[i].fine << endl;

				}

				cout << endl;

			}
			else if (detail == 2)
			{
				cout << "STUDENT " << ++count << endl << endl;
				cout << "Name: " << cur->item.name << endl;
				cout << "ID: " << cur->item.id << endl;
				cout << "Course: " << cur->item.course << endl;
				cout << "Phone No: " << cur->item.phone_no << endl;
				cout << setprecision(2) << fixed;
				cout << "Total Fine: RM" << cur->item.total_fine << endl << endl;

			}
			cout << "\n***********************************************************************************" << endl;

			cur = cur->next;
		}
	}

	if (file.is_open() || output.is_open()) return true;
	else//error handling
	{
		cout << "Print unsuccessfully!!" << endl;
		return false;
	}
	file.close();
	output.close();
}

bool computeAndDisplayStatistics(List* list)
{
	const char* course[5] = { "CS", "IA", "IB", "CN", "CT" };

	if (list->empty())
	{
		cout << "Cannot do computing becasue list is empty!!\n" << endl;
		return false;
	}
	else
	{
		cout << left << setw(15) << "Course" << setw(25) << "Number of Students" << setw(25) << "Total Books Borrowed" << setw(25) << "Total Overdue Books" << setw(25) << "Total Overdue Fine(RM)" << endl;
		for (int i = 0; i < 5; i++)
		{
			int no_student = 0, book_borrowed = 0, book_overdue = 0;
			double total_fine = 0;

			for (int j = 1; j <= list->size(); j++)
			{
				if (strcmp(list->find(j)->item.course, course[i]) == 0)
				{
					no_student++;
					book_borrowed += list->find(j)->item.totalbook;
					for (int k = 0; k < list->find(j)->item.totalbook; k++)
					{
						if (list->find(j)->item.book[k].fine > 0)
						{
							book_overdue++;
						}
					}
					total_fine += list->find(j)->item.total_fine;
				}
			}
			cout << setprecision(2) << fixed;
			cout << left << setw(15) << course[i] << setw(25) << no_student << setw(25) << book_borrowed << setw(25) << book_overdue << setw(25) << total_fine << endl;
		}

		return true;
	}
}

bool printStuWithSameBook(List* list, char* callNum)
{
	LibStudent* studen = new LibStudent;
	int count = 0, i = 0;
	char CALL[20];
	cout << "Insert call number of book to find students who borrow the book: ";
	cin >> CALL;
	if (list->empty())
	{
		cout << "\nNo student is found because the list is empty!\n\n";
		delete studen;
		return false;
	}
	//Make every input to be upper case
	for (int i = 0; i < 20; i++)
	{
		CALL[i] = toupper(CALL[i]);
	}
	strcpy(callNum, CALL);
	//Use for loop to access every struct from list for further checking 
	for (int e = 1; e <= list->size(); e++)
	{
		list->get(e, *studen);
		//get number of students who borrow the book with the callnum
		for (i = 0; i <= studen->totalbook; i++)
		{
			if (strcmp(callNum, studen->book[i].callNum) == 0)
			{
				count++;
			}
		}
	}
	//count 0 means no student is found to borrow the book with this call number
	if (count == 0)
	{
		cout << "\nThe call number is undefined!\n\n";
		delete studen;
		return false;
	}
	cout << "\nThere are " << count << " students that borrow the book with call number " << callNum << " as shown below: \n\n";
	for (int i = 1; i <= list->size(); i++)
	{
		for (int k = 0; k < list->find(i)->item.totalbook; k++)
		{
			if (strcmp(callNum, list->find(i)->item.book[k].callNum) == 0)
			{
				cout << "Student ID = " << list->find(i)->item.id;
				cout << "\nName = " << list->find(i)->item.name;
				cout << "\nCourse = " << list->find(i)->item.course;
				cout << "\nPhone Number = " << list->find(i)->item.phone_no;
				cout << "\nBorrow Date: " << list->find(i)->item.book[k].borrow.day << "/"
					<< list->find(i)->item.book[k].borrow.month << "/" << list->find(i)->item.book[k].borrow.year;
				cout << "\nDue Date: " << list->find(i)->item.book[k].due.day << "/"
					<< list->find(i)->item.book[k].due.month << "/" << list->find(i)->item.book[k].due.year;
				cout << "\n\n";
			}
		}
	}
	delete studen;
	return true;
}

bool displayWarnedStudent(List* list, List* type1, List* type2) {
	//Special Case
	if (list->empty()) {
		cout << "Cannot print from an empty list!!\n";
		return false;
	}

	Node* cur = list->head;
	LibStudent temp;
	int node = 1;

	//List
	while (cur != NULL) {
		int overdue = 0;
		bool overdue2 = true;
		for (int i = 0; i < cur->item.totalbook; i++) {
			if (cur->item.book[i].fine > 5) {
				overdue++;
			}
			if (cur->item.book[i].fine == 0) {
				overdue2 = false;
			}
		}
		if (overdue > 2) {
			list->get(node, temp);
			type1->insert(temp);
		}
		if (overdue2 && cur->item.total_fine > 50) {
			list->get(node, temp);
			type2->insert(temp);
		}

		node++;
		cur = cur->next;
	}

	//Type1
	cout << "\n---------------Type1--------------------\n";
	if (printList(*type1)) cout << "\nThe students that have more than 2 books that are overdue for >= 10 days are printed" << endl << endl;

	//Type2
	cout << "\n---------------Type2---------------------\n";
	if (printList(*type2)) cout << "\nThe students that total fine is more than RM50.00 and every book in the student's book list are overdue are printed" << endl << endl;

	return true;
}
bool printList(List list)
{
	Node* cur;
	int j, count = 0;

	if (list.empty())

	{
		cout << "\n\nNo students at the list\n" << endl;
		return false;

	}
	cur = list.head;
	while (cur != NULL)
	{
		cout << "STUDENT " << ++count << endl << endl;
		cout << "Name: " << cur->item.name << endl;
		cout << "ID: " << cur->item.id << endl;
		cout << "Course: " << cur->item.course << endl;
		cout << "Phone No: " << cur->item.phone_no << endl;
		cout << setprecision(2) << fixed;
		cout << "Total Fine: RM" << cur->item.total_fine << endl << endl;

		cout << "BOOK LIST:" << endl;
		for (int i = 0; i < cur->item.totalbook; i++)
		{
			j = 0;
			cout << endl;
			cout << "BOOK " << i + 1 << endl << endl;
			cout << "Title: " << cur->item.book[i].title << endl;
			cout << "Author: ";
			for (int j = 0; cur->item.book[i].author[j] != NULL; j++)
			{
				cout << cur->item.book[i].author[j];
				if (cur->item.book[i].author[j + 1] != NULL)
					cout << " / ";
			}
			cout << endl;
			cout << "Publisher: " << cur->item.book[i].publisher << endl;
			cout << "Year Published: " << cur->item.book[i].yearPublished << endl;
			cout << "ISBN: " << cur->item.book[i].ISBN << endl;
			cout << "Call Number: " << cur->item.book[i].callNum << endl;
			cout << "Borrow Date: " << cur->item.book[i].borrow.day << "/" << cur->item.book[i].borrow.month << "/" << cur->item.book[i].borrow.year << endl;
			cout << "Due Date: " << cur->item.book[i].due.day << "/" << cur->item.book[i].due.month << "/" << cur->item.book[i].due.year << endl;
			cout << "Fine: RM" << cur->item.book[i].fine << endl;
		}
		cout << endl;
		cur = cur->next;
	}
	return true;
}

int menu()
{
	int choice;
	bool a = true;

	do
	{
		cout << "\nMenu\n\n";
		cout << "1. Read File\n";
		cout << "2. Delete Record\n";
		cout << "3. Search Student\n";
		cout << "4. Insert book\n";
		cout << "5. Display output\n";
		cout << "6. Compute and Display Statistics\n";
		cout << "7. Student with Same Book\n";
		cout << "8. Display Warned Student\n";
		cout << "9. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;
		cout << endl;

		if (choice >= 1 && choice <= 9)
		{
			a = false;
		}
		else if (!choice || (choice < 1 || choice > 9))
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "The option should be between 1-9. Please key in again.\n" << endl << endl;
			a = true;
		}
	} while (a);
	return choice;
}