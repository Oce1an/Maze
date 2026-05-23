#include <stdexcept>
#include <iostream>

class Expression {
public:
    virtual ~Expression() = default;

    virtual double evaluate() const = 0;
};

class Number : public Expression {
private:
    double value;

public:
    explicit Number(double val) : value(val) {}

    double evaluate() const override {
        return value;
    }
};

class BinaryOperation : public Expression {
private:
    Expression* left;
    Expression* right;
    char operation;

public:
    BinaryOperation(Expression* l, char op, Expression* r)
        : left(l), right(r), operation(op) {
    }

    ~BinaryOperation() {
        delete left;
        delete right;
    }

    double evaluate() const override {
        double leftVal = left->evaluate();
        double rightVal = right->evaluate();

        switch (operation) {
        case '+':
            return leftVal + rightVal;
        case '-':
            return leftVal - rightVal;
        case '*':
            return leftVal * rightVal;
        case '/':
            if (rightVal == 0) {
                throw std::runtime_error("Division by zero");
            }
            return leftVal / rightVal;
        default:
            throw std::runtime_error("Unknown operation");
        }
    }
};

int main() {
    Expression* sube = new BinaryOperation(new Number(4.5), '*', new Number(5));

    Expression* expr = new BinaryOperation(new Number(3), '+', sube);

    std::cout << expr->evaluate() << std::endl;

    delete expr;

    return 0;
}