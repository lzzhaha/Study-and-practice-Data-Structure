#ifndef DATE_H
#define DATE_H

#include<sstream>
#include <string>
class Date{
	
	private:
		int year;
		int month;
		int day;

	public:
		Date(int y, int m, int d);
		int getMonth()const;
		int getDay()const;
		int getYear()const;

		bool operator==(const Date&)const;
		std::string toString()const;
		
};

#endif
