
#include <iostream>
#include <mysql.h>
#include <string.h>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

using namespace std;

MYSQL_RES* res;
MYSQL* conn;

class Database
{

public:
	MYSQL* database_creation()
	{
		conn = mysql_init(0);
		if (conn)
		{
			cout << "\nDatabase created\n";
		}
		else
		{
			cout << "\nerror " << mysql_errno(conn);
		}

		conn = mysql_real_connect(conn, "localhost", "root", "", "testdb", 3306, NULL, 0);
		if (conn)
		{
			cout << "\nConnected to database\n";
		}
		else
		{
			cout << "\nDatabase failed to create\n";
		}
		return conn;
	}
};

class Start :public Database
{
private:
	void e_insert();
	void e_update();
	void e_delete();
	void e_display();
public:

	void decide()
	{
		Database d;
		int choice;
		//d.database_creation();
		while (1)
		{

			Start s;
			int ch;
			cout << "\n1.Insert Employee data\n2.Update Employee data\n3.Delete Employee data";
			cout << "\n4.Display Employee data\n5.Exit";
			cout << "\nEnter your choice: ";
			cin >> ch;
			switch (ch)
			{
			case 1:
			{
				thread e_thread(&Start::e_insert, s);
				e_thread.join();
				break;
			}
			case 2:
			{
				thread e_thread(&Start::e_update, s);
				e_thread.join();
				break;
			}
			case 3:
			{
				thread e_thread(&Start::e_delete, s);
				e_thread.join();
				break;
			}
			case 4:
			{
				thread e_thread(&Start::e_display, s);
				e_thread.join();
				break;
			}
			case 5:
			{
				exit(0);
			}
			default:
				cout << "\nInvalid choice\n";
			}
		}
	}
};


void Start::e_insert()
{
	int qstate;
	string first_name, last_name, salary, id;

	cout << "\nenter the employee id : ";
	cin >> id;

	cout << "\nenter the first name: ";
	cin >> first_name;

	cout << "\nenter the last name: ";
	cin >> last_name;

	cout << "\nenter the salary: ";
	cin >> salary;

	string query = "INSERT INTO EMPLOYEE(ID, FIRSTNAME, LASTNAME, SALARY) VALUES('" + id + "','" + first_name + "', '" + last_name + "', '" + salary + "')";

	const char* q = query.c_str();

	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		cout << "\nData inserted\n";
	}
	else
	{
		cout << "\nError: " << mysql_errno(conn);
	}
}

void Start::e_update()
{
	string id, salary;
	string firstname, lastname;
	string update_query;
	int qstate;

	cout << "\n\nEnter the Employee id to update : ";
	cin >> id;

	int status, ch;

	qstate = mysql_query(conn, "SELECT * FROM employee");
	if (!qstate)
	{
		res = mysql_store_result(conn);
		cout << "\n1.Update first name\n2.Update last name\n3.Update salary";
		cout << "\n\nEnter the choice : ";
		cin >> ch;

		switch (ch)
		{
		case 1:
		{
			cout << "\nEnter the first name : ";
			cin >> firstname;
			update_query = "UPDATE employee SET FIRSTNAME = '" + firstname + "' WHERE ID = '" + id + "'";
			const char* qu = update_query.c_str();
			status = mysql_query(conn, qu);
			res = mysql_store_result(conn);
			if (!status)
			{
				cout << "\nFirst name updated\n";
			}
			else
			{
				cout << "\nError : " << mysql_errno(conn);
			}
			break;
		}
		case 2:
		{
			cout << "\nEnter the last name : ";
			cin >> lastname;
			update_query = "UPDATE employee SET LASTNAME = '" + lastname + "' WHERE ID = '" + id + "'";
			const char* qu = update_query.c_str();
			status = mysql_query(conn, qu);
			res = mysql_store_result(conn);
			if (!status)
			{
				cout << "\nLast name updated\n";
			}
			else
			{
				cout << "\nError : " << mysql_errno(conn);
			}
			break;
		}
		case 3:
		{
			cout << "\nEnter the salary : ";
			cin >> salary;
			update_query = "UPDATE employee SET SALARY = '" + salary + "' WHERE ID = '" + id + "'";
			const char* qu = update_query.c_str();
			status = mysql_query(conn, qu);
			res = mysql_store_result(conn);
			if (!status)
			{
				cout << "\nSalary updated\n";
			}
			else
			{
				cout << "\nError : " << mysql_errno(conn);
			}
			break;
		}
		default:
			cout << "\nInvalid choice\n";
		}
	}
}

void Start::e_delete()
{
	string id;
	int qstate;
	cout << "\n\nenter the id to delete : ";
	cin >> id;

	string delete_query = "DELETE FROM employee WHERE ID = '" + id + "'";
	const char* qd = delete_query.c_str();
	qstate = mysql_query(conn, qd);
	if (!qstate)
	{
		cout << "\nDeleted ";
	}
	else
		cout << "\nNot deleted ";
}

void Start::e_display()
{
	int n;
	MYSQL_ROW row;
	n = mysql_query(conn, "SELECT * FROM EMPLOYEE");
	cout << "\nEmployee database\n";
	if (!n)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << "\n";
			cout << "EmpID" << "  \t  "
				<< "First Name" << "  \t  "
				<< "Last Name" << "  \t  "
				<< "Salary" << "\n";

			while (row = mysql_fetch_row(res))
			{
				cout << row[0] << "  \t  "
					<< row[1] << "  \t  "
					<< row[2] << "  \t  "
					<< row[3] << "\n";
			}
		}
	}
}


int main()
{
	Database d;
	Start s;

	thread t1(&Database::database_creation, d);
	t1.join();

	thread t2(&Start::decide, s);
	t2.join();
}
