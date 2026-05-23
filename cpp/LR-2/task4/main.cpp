#include <iostream>

class Expression {
public:
    virtual ~Expression() = default;
    virtual double evaluate() const = 0;
};

class Number : public Expression {
    double value;
public:
    Number(double v) : value(v) {}
    double evaluate() const override {
        return value;
    }
};

class BinaryOperation : public Expression {
    Expression const* left;
    Expression const* right;
    char op;
public:
    BinaryOperation(Expression const* l, char o, Expression const* r)
        : left(l), right(r), op(o) {
    }
    double evaluate() const override {
        double l = left->evaluate();
        double r = right->evaluate();
        switch (op) {
        case '+': return l + r;
        case '-': return l - r;
        case '*': return l * r;
        case '/': return l / r;
        default: return 0;
        }
    }
};

bool check_equals(Expression const* left, Expression const* right) {
    void const* vptr_left = *reinterpret_cast<void const* const*>(left);
    void const* vptr_right = *reinterpret_cast<void const* const*>(right);
    return vptr_left == vptr_right;
}

int main() {
    Number n1(5);
    Number n2(10);
    BinaryOperation b1(&n1, '+', &n2);
    BinaryOperation b2(&n1, '-', &n2);

    std::cout << std::boolalpha;
    std::cout << "n1 and n2: " << check_equals(&n1, &n2) << std::endl;
    std::cout << "b1 and b2: " << check_equals(&b1, &b2) << std::endl;
    std::cout << "n1 and b1: " << check_equals(&n1, &b1) << std::endl;

    return 0;
}