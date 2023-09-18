using namespace std;

#include "../header/util.h"

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

int main(int argc, const char *argv[])
{
    ifstream input("input.txt", ios::binary);
    string expr;
    int len;

    // get the whole infix expression, and the length of it
    pre_work(input, expr, len);

    // stack for operators
    stack<char> st_op;

    // stack for nums
    stack<double> st_num;

    // discard the character '#'
    input.get();
    
    // start progress : calc the value of the infix expression
    cout << "### 开始计算中缀表达式的值 ###\n" << endl;

    char ch;

    // the len of "─" to be illustrated
    int stack_boundary_len = 1;

    while (ch = input.peek(), ch != '#')
    // peek the initial character of the term
    {   
        segmentation('=', len + 9);

        // show the reading expression 
        cout << "表达式: " << expr << endl;

        // show the arrow "↑" pointing to the term which is being reading
        cout << string((int)input.tellg() + 8, ' ') << "↑" << endl;

        if (isdigit(ch) || ch == '.')
        // if the term is a double
        // i.e. the term with the beginning charactor of "0123456789."
        // should be push into the op-num-stack 
        {
            double num;
            // read the double
            input >> num;

            // show progress
            cout << "当前指向：" << num << " ——→ 判断为实数，入实数栈" << endl;

            // push the num into the op-num-stack 
            st_num.push(num);

            // increase the op-num-stack boundary length, the extra 1 is for the separator ' '
            stack_boundary_len += (get_double_len(num) + 1);

            // show current op-num-stack
            show_stack_in_graph(st_num, ' ', 0, 0, stack_boundary_len, "当前实数栈");
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

            segmentation('-', len + 9);

            char op;
            while (op = st_op.top(), st_op.pop(), op != corr_bracket(ch))
            // the operator stack should pop until meet the corresponding left bracket
            {
                // show progress
                cout << "符号栈中弹出符号：\'" << op << "\'，进行计算" << endl;

                // show current op-stack
                show_stack_in_graph(st_op, ' ', 2, op, st_op.size() * 2 + 1, "当前符号栈");

                calc_process(st_num, stack_boundary_len, op);
                getch();

                segmentation('-', len + 9);
            }
            // show the corresponding left bracket, and the progress
            cout << "符号栈中弹出符号：\'" << op << "\'，结束弹栈" << endl;

            // show current operator stack
            show_stack_in_graph(st_op, ' ', 2, op, st_op.size() * 2 + 1, "当前符号栈");
            // show current op-num-stack
            show_stack_in_graph(st_num, ' ', 0, 0, stack_boundary_len, "当前实数栈");
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
                if (st_op.empty() || is_bigger(ch, st_op.top()))
                // op-stack is empty, or the reading character is "bigger" than the top one
                // push in direct
                {
                    // show progress
                    if (st_op.empty())
                        cout << "符号栈空，直接入栈" << endl;
                    else
                        cout << "当前操作符\'" << ch << "\'的优先级大于符号栈顶的操作符：\'" << st_op.top() << "\'，将\'" << ch << "\'入栈" << endl;

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
                    cout << "当前操作符\'" << ch << "\'的优先级不大于符号栈顶的操作符：\'" << temp << "\'，符号栈弹栈，进行计算" << endl;

                    // op-stack pop
                    st_op.pop();

                    // show current operator stack
                    show_stack_in_graph(st_op, ' ', 2, temp, st_op.size() * 2 + 1, "当前符号栈");

                    calc_process(st_num, stack_boundary_len, temp);
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

            // show progress
            cout << "弹出操作符\'" << temp << "\'，进行计算" << endl;

            // show current operator stack
            show_stack_in_graph(st_op, ' ', 2, temp, st_op.size() * 2 + 1, "当前符号栈");

            calc_process(st_num, stack_boundary_len, temp);
            getch();

            // segmentation
            if (st_op.size() != 0)
                segmentation('-', len + 9);
        }
    }
    segmentation('=', len + 9);

    // show result
    cout << "\n表达式读取完毕，结果为" << st_num.top() << endl;
    getch();

    return 0;
}
