## 表达式求解 ExpressionCalc

数据结构实验课作业，上传以存档
</br>DSA experiment project, upload for archive purpose

## 特性 Feature

- 可进行合法的普通中缀表达式的求解
  </br>合法的表达式例： `#1.1^4.5-14.*(.1919+8.10)#`
- 未对表达式括号的合法性进行检查
- 支持 `+ - * / ^` 运算，支持 `() [] {}` 三种括号，三种括号没有使用限制
- 有两个版本，直接计算版 与 间接计算版（先计算后缀表达式）
- 对 `/` 以及 `^` 运算有数学合法性检查
- 对计算中的每一步都有讲解及图解（中文）
  
</br>

- Can calculate valid infix expression
  </br>e.g.  `#1.1^4.5-14.*(.1919+8.10)#`
- DO NOT check the validity of brackets 
- Support `+ - * / ^` operators, `() [] {}` brackets, no limitation for bracket usage
- Two version, one for calc directly, the other one for calc indirectly (transformed into suffix expression first, then calc)
- Math validity check for operator `/`  and `^` 
- Each step of the calculation is explained and illustrated (in Chinese)

## 项目结构 Project structure

```
├── README.md
├── /src
│    ├── /direct
│    │      ├── compile.bat
│    │      ├── direct.exe
│    │      ├── input.txt
│    │      └── main.cpp
│    │
│    ├── /indirect
│    │      ├── compile.bat
│    │      ├── indirect.exe
│    │      ├── input.txt
│    │      └── main.cpp
│    │
│    └── /test_for_double_length
│           ├── test.cpp
│           └── testlen.exe
│     
└── priority_table.xlsx
```