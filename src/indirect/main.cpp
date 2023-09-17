#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <conio.h>

using namespace std;

int priority_table[8][8] = {
    {0, 0, 0, 0, 0, 1, 1, 1},
    {0, 0, 0, 0, 0, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};

enum TermType
{
    NUM = 0,
    OPERATOR
};

class Term
{
public:
    TermType type;
    string str;
    Term(TermType type, string str): type(type), str(str) {}
    Term(TermType type, char ch) : type(type), str(1, ch) {}
};

void pre_work(ifstream& input, string& expr, int& len);

void show_suffix_expr(string memo, vector<Term> &v);
template <typename T> void show_stack(stack<T> &st, char separator);
template <typename T> void show_stack_in_graph(stack<T> &st, char separator, int type, char elem, int boundary_len, const string& memo);

void show_str_rpt(string str, int times);
void segmentation(string rpt_str, int rpt_cnt, char suffix = '\0');
void segmentation(char rpt_char, int rpt_cnt, char suffix = '\0');

int is_bigger(char op_1, char op_2);
int get_op_type(char op);
int is_right_bracket(char bracket);
char corr_bracket(char right_bracket);
double calc(double a, double b, const string& oper);
int get_double_len(double num);
int get_suffix_expr_len(vector<Term>& suffix_expr);

void calc_process(stack<double>& st_num, int& stack_boundary_len, char op);

int main(int argc, const char *argv[])
{
    ifstream input("input.txt");
    string expr;
    int len;

    // get the whole infix expression, and the length of it
    pre_work(input, expr, len);

    // stack for operators
    stack<char> st_op;

    // vector for suffix expression
    vector<Term> suffix_expr;

    // discard the character '#'
    input.get();
    
    // start progress 1 : calc out the suffix expression
    cout << "### 开始通过中缀表达式求解后缀表达式 ###\n" << endl;

    char ch;
    while (ch = input.peek(), ch != '#')
    // peek the initial character of the term
    {   
        segmentation('=', len + 9);

        // show the reading expression 
        cout << "表达式: " << expr << endl;

        // show the arrow "↑" pointing to the term which is being reading
        // XXX: Unpredicted value plus behind the tellg() to make the arrow pointing correctly
        cout << string((int)input.tellg() + 5, ' ') << "↑" << endl;

        if (isdigit(ch) || ch == '.')
        // if the term is a double
        // i.e. the term with the beginning character of "0123456789."
        // should be push back into the suffix expr
        {
            double num;
            // read the double
            input >> num;

            // show progress
            cout << "当前指向：" << num << " ——→ 判断为实数，添加至后缀表达式" << endl;

            // push back the term into the suffix expression
            suffix_expr.emplace_back(Term(NUM, to_string(num)));

            // show current suffix expression
            show_suffix_expr("当前后缀表达式：", suffix_expr);
        }
        else if (is_right_bracket(ch))
        // if the term is a right bracket
        // i.e. be of ")]}"
        // the operator stack should pop until meet the corresponding bracket
        // i.e. be of "([{"
        {
            // discard the character
            input.get();

            // show progress
            cout << "当前指向：\'" << ch << "\' ——→ 判断为右括号，应弹出符号栈元素，直到遇到左括号\'" << corr_bracket(ch) << '\'' << endl;

            // show current operator stack
            show_stack_in_graph(st_op, ' ', 0, 0, st_op.size() * 2 + 1, "当前符号栈");
            getch();

            char op;
            while (op = st_op.top(), st_op.pop(), op != corr_bracket(ch))
            // the operator stack should pop until meet the corresponding left bracket
            {
                // show progress
                cout << "符号栈中弹出符号：\'" << op << "\'，添加至后缀表达式" << endl;

                // push back the popped operator into the suffix expr
                suffix_expr.emplace_back(Term(OPERATOR, op));

                // show current suffix expression
                show_suffix_expr("当前后缀表达式：", suffix_expr);

                // show current operator stack
                show_stack_in_graph(st_op, ' ', 2, op, st_op.size() * 2 + 1, "当前符号栈");
                getch();

                segmentation('-', len + 9);
            }
            // show the corresponding left bracket, and the progress
            cout << "符号栈中弹出符号：\'" << op << "\'，结束弹栈" << endl;

            // show current operator stack
            show_stack_in_graph(st_op, ' ', 2, op, st_op.size() * 2 + 1, "当前符号栈");

            // show current suffix expression
            show_suffix_expr("当前后缀表达式：", suffix_expr);
        }
        else
        // if the term is a operator
        // i.e. be of "+-*/^([{"
        // push into the operator stack, obeying the rule "the upper op's priority should be greater than its lower op's"
        {
            // discard the character
            input.get();

            // show progress
            cout << "当前指向：\'" << ch << "\' ——→ 判断为操作符" << endl;
            while (1)
            {
                if (st_op.empty())
                // op-stack is empty, push in direct
                {
                    // show progress
                    cout << "符号栈空，直接入栈" << endl;

                    // push
                    st_op.push(ch);

                    // show current operator stack
                    show_stack_in_graph(st_op, ' ', 1, 0, st_op.size() * 2 + 1, "当前符号栈");

                    // over
                    break;
                }
                else if (is_bigger(ch, st_op.top()))
                // the reading character is "bigger" than the top one
                // push in direct
                {
                    // show progress
                    cout << "当前操作符\'" << ch << "\'的优先级大于符号栈顶的操作符：\'" << st_op.top() << "\'，将\'" << ch << "\'入栈" << endl;

                    // push
                    st_op.push(ch);

                    // show current operator stack
                    show_stack_in_graph(st_op, ' ', 1, 0, st_op.size() * 2 + 1, "当前符号栈");

                    // over
                    break;
                }
                else
                // the reading character is "not bigger" than the top one
                // pop the op-stack until the the reading character is "bigger" than the top one
                {
                    // get op-stack top
                    char temp = st_op.top();

                    // show progress
                    cout << "当前操作符\'" << ch << "\'的优先级不大于符号栈顶的操作符：\'" << temp << "\'，符号栈弹栈" << endl;

                    // push back the popped operator into the suffix expr
                    suffix_expr.emplace_back(Term(OPERATOR, temp));

                    // show current suffix expression
                    show_suffix_expr("当前后缀表达式：", suffix_expr);

                    // op-stack pop
                    st_op.pop();

                    // show current operator stack
                    show_stack_in_graph(st_op, ' ', 2, temp, st_op.size() * 2 + 1, "当前符号栈");
                    getch();

                    segmentation('-', len + 9);

                    // pop the op-stack until the the reading character is "bigger" than the top one
                }
            }
        }
        segmentation('=', len + 9, '\n');
        getch();
    }
    input.close();
    segmentation('=', len + 9);

    // show progress
    cout << "表达式读取完毕" << endl;
    
    if (!st_op.empty())
    // if there are some operators left in the op-stack
    {
        // show progress
        cout << "符号栈有剩余操作符，弹栈直到空栈" << endl;
        
        while (!st_op.empty())
        // while there are some operators left in the op-stack
        {
            // get top and pop
            char temp = st_op.top();
            st_op.pop();

            // show current operator stack
            show_stack_in_graph(st_op, ' ', 2, temp, st_op.size() * 2 + 1, "当前符号栈");

            // show progress
            cout << "弹出操作符\'" << temp << "\'，添加至后缀表达式" << endl;

            // push back the popped operator into the suffix expr
            suffix_expr.emplace_back(Term(OPERATOR, temp));

            // show current suffix expression
            show_suffix_expr("当前后缀表达式：", suffix_expr);
            getch();

            if (st_op.size() != 0)
                segmentation('-', len + 9, '\n');
        }
    }
    cout << endl;

    // show final suffix expression
    show_suffix_expr("得到最终后缀表达式：", suffix_expr);

    segmentation('=', len + 9);
    getch();

    //==============================================================================

    // start progress 2 : calc the value of the expression
    cout << "\n### 开始计算后缀表达式的值 ###\n" << endl;

    // stack for operation nums
    stack<double> st_num;

    // calc the title len (just for aesthetics)
    int title_len = get_suffix_expr_len(suffix_expr);

    // show title, 12 is the len of "后缀表达式："
    segmentation('=', title_len + 12);

    int arrow_index = 12;

    // the len of "─" to be illustrated
    int stack_boundary_len = 1;

    for (int i = 0; i < suffix_expr.size(); ++i)
    {
        // show suffix expression
        show_suffix_expr("后缀表达式：", suffix_expr);

        // show arrow pointing to the term being read
        cout << string(arrow_index, ' ') << "↑" << endl;

        Term& now_term = suffix_expr.at(i);
        if (now_term.type == NUM)
        // the term is a double (operand)
        {
            // get the double value
            double temp = stod(now_term.str);

            // show progress
            cout << "当前指向：" << temp << " ——→ 判断为实数，入栈" << endl;

            // push into the operand stack
            st_num.push(temp);

            // increase the stack boundary length, the extra 1 is for the separator ' '
            stack_boundary_len += (get_double_len(temp) + 1);

            // show current operand stack
            show_stack_in_graph(st_num, ' ', 0, 0, stack_boundary_len, "当前实数栈");

            // increase the arrow index, let the arrow point to the next term, the extra 1 is for the separator ' '(space)
            arrow_index += (get_double_len(temp) + 1);
        }
        else
        // the term is a operator
        {
            // show progress
            cout << "当前指向：\'" << now_term.str << "\' ——→ 判断为操作符，计算" << endl;

            calc_process(st_num, stack_boundary_len, now_term.str[0]);

            // increase the arrow index, let the arrow point to the next term
            // for 2, one is for the operator, the other one is for separator ' '(space)
            arrow_index += 2;
        }

        if (i != suffix_expr.size() - 1)
        // if it is not the last term
            // print the segmentation
            segmentation('-', title_len + 12);

        getch();
    }
    segmentation('=', title_len + 12, '\n');

    // show final result
    cout << "表达式值为：" << st_num.top() << endl;
    getch();

    return 0;
}

void pre_work(ifstream& input, string& expr, int& len)
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
void show_suffix_expr(string memo, vector<Term> &v)
{
    cout << memo;
    for (auto term: v)
    {
        if (term.type == 0)
            cout << stod(term.str) << ' ';
        else
            cout << term.str << ' ';
    }
    cout << endl;
}

// print stack content
template <typename T> void show_stack(stack<T> &st, char separator)
{
    stack<T> temp;
    while (!st.empty())
    {
        temp.push(st.top());
        st.pop();
    }
    while (!temp.empty())
    {
        T ch = temp.top();
        temp.pop();
        cout << ch << separator;
        st.push(ch);
    }
}

// the memo should be 6 Chinese charactor for formating
template <typename T> void show_stack_in_graph(stack<T> &st, char separator, int type, char elem, int boundery_len, const string& memo)
{
    cout << "            ┌"; show_str_rpt("─", boundery_len), cout << endl;
    cout << memo << "：│ "; show_stack(st, separator);

    switch (type)
    {
    case 0:
        cout << endl;
        break;
    case 1:
        cout << "  ←——" << endl;
        break;
    case 2:
        cout << "  ——→  \'" << elem << "\'" << endl; 
        break;
    }
    cout << "            └"; show_str_rpt("─", boundery_len), cout << endl;
}

inline void show_str_rpt(string str, int times)
{
    for (int i = 0; i < times; i++)
        cout << str;
}

inline void segmentation(string rpt_str, int rpt_cnt, char suffix)
{
    show_str_rpt(rpt_str, rpt_cnt);
    cout << suffix << endl;
}

inline void segmentation(char rpt_char, int rpt_cnt, char suffix)
{
    cout << string(rpt_cnt, rpt_char) << suffix << endl;
}

inline int is_bigger(char op_1, char op_2)
{
    int i = get_op_type(op_1), j = get_op_type(op_2);
    return priority_table[i][j];
}

inline int get_op_type(char op)
{
    string str = "+-*/^([{";
    return str.find(op);
}

inline int is_right_bracket(char bracket)
{
    return bracket == ')' || bracket == ']' || bracket == '}';
}

inline char corr_bracket(char right_bracket)
{
    switch (right_bracket)
    {
    case ')':
        return '(';
    case ']':
        return '[';
    case '}':
        return '{';
    default:
        return '\0';
    }
}

double calc(double a, double b, const string& oper)
{
    if (oper == "+")
        return a + b;
    else if (oper == "-")
        return a - b;
    else if (oper == "*")
        return a * b;
    else if (oper == "/")
    {
        if (isinf(a / b))
            throw "！！！ 除数不能为零 ！！！";
        else
            return a / b;
    }
    else if (oper == "^")
    {
        if (!isnormal(pow(a, b)))
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

int get_suffix_expr_len(vector<Term>& suffix_expr)
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

void calc_process(stack<double>& st_num, int& stack_boundary_len, char op)
{
    // get the two op-num out, decrease the stack boundary length, the extra 1 is for the separator ' '
    double num2 = st_num.top();
    st_num.pop();
    stack_boundary_len -= (get_double_len(num2) + 1);
    double num1 = st_num.top();
    st_num.pop();
    stack_boundary_len -= (get_double_len(num1) + 1);

    // show progress
    cout << "实数栈弹出 " << num2 << " 与 " << num1 << " ，与\'" << op << "\'进行运算" << endl;

    // show current operand stack
    show_stack_in_graph(st_num, ' ', 0, 0, stack_boundary_len, "当前实数栈");

    // calc the result
    double res;
    try
    {
        res = calc(num1, num2, string(1, op));
    }
    catch (const char* str)
    // math error
    {
        cout << str << endl;
        getch();
        exit(-1);
    }

    // show progress
    cout << num1 << ' ' << op << ' ' << num2 << " = " << res << "，将 " << res << " 入实数栈" << endl;

    // push the result into the operand stack, increase the stack boundary length
    st_num.push(res);
    stack_boundary_len += (get_double_len(res) + 1);

    // show current operand stack
    show_stack_in_graph(st_num, ' ', 0, 0, stack_boundary_len, "当前实数栈");
}