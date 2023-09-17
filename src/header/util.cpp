#include "util.h"

void pre_work(std::ifstream& input, std::string& expr, int& len)
{
    // set terminal to code UTF-8
    system("chcp 65001");

    system("cls");

    // check the validity of the file
    if (input.peek() != '#')
    {
        std::cout << "Input file format error!" << std::endl;
        getch();
        exit(-1);
    }

    // read whole expression, for show purpose
    input >> expr;
    len = expr.length();

    // set the file reading pointer to the beginning of the file
    input.seekg(0);
}

// print terms in suffix_expr
void show_suffix_expr(std::string memo, std::vector<Term> &v)
{
    std::cout << memo;
    for (auto term: v)
    {
        if (term.type == 0)
            std::cout << stod(term.str) << ' ';
        else
            std::cout << term.str << ' ';
    }
    std::cout << std::endl;
}

double calc(double a, double b, const std::string& oper)
{
    if (oper == "+")
        return a + b;
    else if (oper == "-")
        return a - b;
    else if (oper == "*")
        return a * b;
    else if (oper == "/")
    {
        if (std::isinf(a / b))
            throw "！！！ 除数不能为零 ！！！";
        else
            return a / b;
    }
    else if (oper == "^")
    {
        if (!std::isnormal(pow(a, b)))
            throw "！！！ 该乘方无意义 ！！！";
        else
            return pow(a, b);
    }
    else
        return 0;
}

int get_double_len(double num)
{
    num = (int)(num * 100000) / 100000.0;

    int len = 0;

    if (num < 0)
    {
        ++len;
        num = -num;
    }
    int integer = round(num);
    
    do
    {
        ++len;
        integer /= 10;
    }
    while (integer > 0);
    
    double decimals = num - round(num);
    decimals = fabs(decimals);
    if (decimals > 1e-6)
    {
        ++len;
        do
        {
            ++len;
            decimals *= 10;
            decimals -= round(decimals);
            decimals = fabs(decimals);
        }
        while (decimals > 1e-4);
    }

    return len;
}

int get_suffix_expr_len(std::vector<Term>& suffix_expr)
{
    int len = 0;
    for (int i = 0; i < suffix_expr.size(); i++)
    {
        Term &temp = suffix_expr.at(i);
        if (temp.type == NUM)
            len += get_double_len(stod(temp.str));
        else // temp.type == OPERATOR
            len += 1;
    }
    len += suffix_expr.size();
    return len;
}

void calc_process(std::stack<double>& st_num, int& stack_boundary_len, char op)
{
    // get the two op-num out, decrease the stack boundary length, the extra 1 is for the separator ' '
    double num2 = st_num.top();
    st_num.pop();
    stack_boundary_len -= (get_double_len(num2) + 1);
    double num1 = st_num.top();
    st_num.pop();
    stack_boundary_len -= (get_double_len(num1) + 1);

    // show progress
    std::cout << "实数栈弹出 " << num2 << " 与 " << num1 << " ，与\'" << op << "\'进行运算" << std::endl;

    // show current operand stack
    show_stack_in_graph(st_num, ' ', 0, 0, stack_boundary_len, "当前实数栈");

    // calc the result
    double res;
    try
    {
        res = calc(num1, num2, std::string(1, op));
    }
    catch (const char* str)
    // math error
    {
        std::cout << str << std::endl;
        getch();
        exit(-1);
    }

    // show progress
    std::cout << num1 << ' ' << op << ' ' << num2 << " = " << res << "，将 " << res << " 入实数栈" << std::endl;

    // push the result into the operand stack, increase the stack boundary length
    st_num.push(res);
    stack_boundary_len += (get_double_len(res) + 1);

    // show current operand stack
    show_stack_in_graph(st_num, ' ', 0, 0, stack_boundary_len, "当前实数栈");
}