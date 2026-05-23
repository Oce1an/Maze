#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <algorithm>

class Student {
private:
    std::string groupNumber;
    std::string fullName;
    std::vector<int> winterGrades;
    std::vector<int> summerGrades;
    double winterAverage;
    double summerAverage;

    void calculateAverages();

public:
    Student();
    Student(const std::string& group, const std::string& name,
            const std::vector<int>& winter, const std::vector<int>& summer);

    std::string getGroupNumber() const { return groupNumber; }
    std::string getFullName() const { return fullName; }
    std::vector<int> getWinterGrades() const { return winterGrades; }
    std::vector<int> getSummerGrades() const { return summerGrades; }
    double getWinterAverage() const { return winterAverage; }
    double getSummerAverage() const { return summerAverage; }

    void setGroupNumber(const std::string& group) { groupNumber = group; }
    void setFullName(const std::string& name) { fullName = name; }
    void setWinterGrades(const std::vector<int>& grades);
    void setSummerGrades(const std::vector<int>& grades);

    bool hasWinterDebts() const;
    bool shouldBeExpelled() const;
    int getWinterDebtCount() const;
    int getSummerDebtCount() const;

    std::string toString() const;
    static Student fromString(const std::string& str);
};

#endif