#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <conio.h>

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
    std::string str;
    Term(TermType type, std::string str): type(type), str(str) {}
    Term(TermType type, char ch) : type(type), str(1, ch) {}
};

void pre_work(std::ifstream& input, std::string& expr, int& len);

void show_suffix_expr(std::string memo, std::vector<Term> &v);
template <typename T>
void show_stack(std::stack<T> &st, char separator);
template <typename T>
void show_stack_in_graph(std::stack<T> &st, char separator, int type, char elem, int boundary_len, const std::string& memo);

void show_str_rpt(std::string str, int times);
void segmentation(std::string rpt_str, int rpt_cnt, char suffix = '\0');
void segmentation(char rpt_char, int rpt_cnt, char suffix = '\0');

int is_bigger(char op_1, char op_2);
int get_op_type(char op);
int is_right_bracket(char bracket);
char corr_bracket(char right_bracket);
double calc(double a, double b, const std::string& oper);
int get_double_len(double num);
int get_suffix_expr_len(std::vector<Term>& suffix_expr);

void calc_process(std::stack<double>& st_num, int& stack_boundary_len, char op);

// print stack content
template <typename T>
void show_stack(std::stack<T> &st, char separator)
{
    std::stack<T> temp;
    while (!st.empty())
    {
        temp.push(st.top());
        st.pop();
    }
    while (!temp.empty())
    {
        T ch = temp.top();
        temp.pop();
        std::cout << ch << separator;
        st.push(ch);
    }
}

// the memo should be 6 Chinese charactor for formating
template <typename T>
void show_stack_in_graph(std::stack<T> &st, char separator, int type, char elem, int boundary_len, const std::string& memo)
{
    std::cout << "            ┌"; show_str_rpt("─", boundary_len), std::cout << std::endl;
    std::cout << memo << "：│ "; show_stack(st, separator);

    switch (type)
    {
    case 0:
        std::cout << std::endl;
        break;
    case 1:
        std::cout << "  ←——" << std::endl;
        break;
    case 2:
        std::cout << "  ——→  \'" << elem << "\'" << std::endl; 
        break;
    }
    std::cout << "            └"; show_str_rpt("─", boundery_len), std::cout << std::endl;
}
