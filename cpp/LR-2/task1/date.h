#ifndef DATE_H
#define DATE_H

#include <stdexcept>
#include <string>

class Date {
private:
    int day;
    int month;
    int year;

    void validate() const;
    int daysInMonth() const;

public:
    Date();
    Date(int d, int m, int y);
    Date(const std::string& dateString);

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    std::string toString() const;

    Date NextDay() const;
    Date PreviousDay() const;
    bool IsLeap() const;
    short WeekNumber() const;
    int DaysTillYourBithday(const Date& birthdayDate) const;
    int Duration(const Date& date) const;
};

class InvalidDateException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid date format or value";
    }
};

#endif
