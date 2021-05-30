/*------------------------------------------------------------------------------
    * File:        Calculator.h                                                *
    * Description: Declaration of functions and data types used for            *
    *              calculating math expressions.                               *
    * Created:     15 may 2021                                                 *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
    * GitHub:      https://github.com/hellopuza                                *
    * Copyright © 2021 Artem Puzankov. All rights reserved.                    *
    *///------------------------------------------------------------------------

#ifndef CALCULATOR_H_INCLUDED
#define CALCULATOR_H_INCLUDED

#define _CRT_SECURE_NO_WARNINGS
//#define NDEBUG


#if defined (__GNUC__) || defined (__clang__) || defined (__clang_major__)
    #define __FUNC_NAME__   __PRETTY_FUNCTION__

#elif defined (_MSC_VER)
    #define __FUNC_NAME__   __FUNCSIG__

#else
    #define __FUNC_NAME__   __FUNCTION__

#endif


#include "../StringLib/StringLib.h"
#include "../StackLib/Stack.h"
#include "../TreeLib/Tree.h"
#include "Operations.h"


//==============================================================================
/*------------------------------------------------------------------------------
                   Calculator errors                                           *
*///----------------------------------------------------------------------------
//==============================================================================


enum CalculatorErrors
{
    CALC_NOT_OK = -1                                                       ,
    CALC_OK = 0                                                            ,
    CALC_NO_MEMORY                                                         ,

    CALC_DESTRUCTED                                                        ,
    CALC_NULL_INPUT_CALCULATOR_PTR                                         ,
    CALC_SYNTAX_ERROR                                                      ,
    CALC_SYNTAX_NO_CLOSE_BRACKET                                           ,
    CALC_SYNTAX_NUMER_ERROR                                                ,
    CALC_SYNTAX_UNIDENTIFIED_FUNCTION                                      ,
};

static const char* calc_errstr[] =
{
    "ERROR"                                                                ,
    "OK"                                                                   ,
    "Failed to allocate memory"                                            ,

    "Calculator has already destructed"                                    ,
    "The input value of the calculator pointer turned out to be zero"      ,
    "Syntax error"                                                         ,
    "Close bracket \')\' required here"                                    ,
    "Wrong number"                                                         ,
    "Unidentified function"                                                ,
};

static const char* CALCULATOR_LOGNAME = "calculator.log";

#define CHECK_SYNTAX(cond, err, expr, len) if (cond)                                                                   \
                                           {                                                                           \
                                               PrintError(CALCULATOR_LOGNAME, __FILE__, __LINE__, __FUNC_NAME__, err); \
                                               PrintBadExpr(CALCULATOR_LOGNAME, expr, len);                            \
                                               return 0;                                                               \
                                           }

#define CALC_ASSERTOK(cond, err) if (cond)                                                                 \
                                 {                                                                         \
                                   PrintError(CALCULATOR_LOGNAME, __FILE__, __LINE__, __FUNC_NAME__, err); \
                                   exit(err);                                                              \
                                 }


//==============================================================================
/*------------------------------------------------------------------------------
                   Calculator constants and types                              *
*///----------------------------------------------------------------------------
//==============================================================================


static const char* GRAPH_FILENAME = "Equation.dot";
const size_t       MAX_STR_LEN    = 256;

enum NODE_TYPE 
{
    NODE_FUNCTION = 1,
    NODE_OPERATOR = 2,
    NODE_VARIABLE = 3,
    NODE_NUMBER   = 4,
};

struct Expression 
{
    char* str      = nullptr;
    char* symb_cur = nullptr;
};

struct CalcNodeData
{
    operation op   = {};
    char node_type = 0;
};

template<> const char*            PRINT_TYPE<CalcNodeData> = "CalcNodeData";
template<> constexpr CalcNodeData POISON<CalcNodeData>     = {};

bool isPOISON  (CalcNodeData value);
void TypePrint (FILE* fp, const CalcNodeData& node_data);


struct Variable
{
    NUM_TYPE value = 0;
    const char* name = nullptr;
};

template<> const char*        PRINT_TYPE<Variable> = "Variable";
template<> constexpr Variable POISON<Variable>     = {};

bool isPOISON  (Variable value);
void TypePrint (FILE* fp, const Variable& var);




class Calculator
{
private:

    int state_;
    char* filename_;

    Tree<CalcNodeData> tree_;
    Stack<Variable>    variables_;

    Stack<char*> path2badnode_;

public:

//------------------------------------------------------------------------------
/*! @brief   Calculator default constructor.
*/

    Calculator ();

//------------------------------------------------------------------------------
/*! @brief   Calculator constructor.
 *
 *  @param   filename    Name of input file
 */

    Calculator (char* filename);

//------------------------------------------------------------------------------
/*! @brief   Calculator copy constructor (deleted).
 *
 *  @param   obj         Source calculator
 */

    Calculator (const Calculator& obj);

    Calculator& operator = (const Calculator& obj); // deleted

//------------------------------------------------------------------------------
/*! @brief   Calculator destructor.
 */

   ~Calculator ();

//------------------------------------------------------------------------------
/*! @brief   Execution process.
 *
 *  @return  error code
 */

    int Run ();

/*------------------------------------------------------------------------------
                   Private functions                                           *
*///----------------------------------------------------------------------------

private:

//------------------------------------------------------------------------------
/*! @brief   Calculating process.
 *
 *  @param   node_cur    Current node
 *
 *  @return  error code
 */

    int Calculate (Node<CalcNodeData>* node_cur);

//------------------------------------------------------------------------------
/*! @brief   Write calculated result to console or to file.
 */

    void Write ();

//------------------------------------------------------------------------------
};

//------------------------------------------------------------------------------
/*! @brief   Prints an error wih description to the console and to the log file.
 *
 *  @param   logname     Name of the log file
 *  @param   file        Name of the program file
 *  @param   line        Number of line with an error
 *  @param   function    Name of the function with an error
 *  @param   err         Error code
 */

void PrintError (const char* logname, const char* file, int line, const char* function, int err);

//------------------------------------------------------------------------------
/*! @brief   Get an answer from stdin (yes or no).
 *
 *  @return  true or false
 */

bool scanAns ();

//------------------------------------------------------------------------------
/*! @brief   Get a number from stdin.
 *
 *  @return  number
 */

NUM_TYPE scanNum ();

//------------------------------------------------------------------------------
/*! @brief   Get string equation from stdin.
 * 
 *  @return  string expression
 */

char* ScanExpr ();

//------------------------------------------------------------------------------
/*! @brief   Convert string expression to tree.
 * 
 *  @param   expr        String expression
 *  @param   tree        Equation tree
 *
 *  @return  -1 if error, 0 if ok
 */

int Expr2Tree (Expression& expr, Tree<CalcNodeData>& tree);

//------------------------------------------------------------------------------
/*! @brief   Parsing of expression beginning with plus and minus signs.
 * 
 *  @param   tree        Equation tree
 *  @param   expr        String expression
 *
 *  @return  error code
 */

Node<CalcNodeData>* pass_Plus_Minus (Tree<CalcNodeData>& tree, Expression& expr);

//------------------------------------------------------------------------------
/*! @brief   Parsing of expression with mulptiply and division signs.
 * 
 *  @param   tree        Equation tree
 *  @param   expr        String expression
 *
 *  @return  error code
 */

Node<CalcNodeData>* pass_Mul_Div (Tree<CalcNodeData>& tree, Expression& expr);

//------------------------------------------------------------------------------
/*! @brief   Parsing of expression with power signs.
 * 
 *  @param   tree        Equation tree
 *  @param   expr        String expression
 *
 *  @return  error code
 */

Node<CalcNodeData>* pass_Power (Tree<CalcNodeData>& tree, Expression& expr);

//------------------------------------------------------------------------------
/*! @brief   Parsing of expression with brackets.
 * 
 *  @param   tree        Equation tree
 *  @param   expr        String expression
 *
 *  @return  error code
 */

Node<CalcNodeData>* pass_Brackets (Tree<CalcNodeData>& tree, Expression& expr);

//------------------------------------------------------------------------------
/*! @brief   Parsing of expression with function.
 * 
 *  @param   tree        Equation tree
 *  @param   expr        String expression
 *
 *  @return  error code
 */

Node<CalcNodeData>* pass_Function (Tree<CalcNodeData>& tree, Expression& expr);

//------------------------------------------------------------------------------
/*! @brief   Parsing of expression with number.
 * 
 *  @param   tree        Equation tree
 *  @param   expr        String expression
 *
 *  @return  error code
 */

Node<CalcNodeData>* pass_Number (Tree<CalcNodeData>& tree, Expression& expr);

//------------------------------------------------------------------------------
/*! @brief   Function identifier.
 *
 *  @param   word        C string to be recognized
 *
 *  @return  function code if found else NOT_OK
 */

char findFunc (const char* word);

//------------------------------------------------------------------------------
/*! @brief   Prints an expression indicating an error.
 *
 *  @param   logname     Name of the log file
 *  @param   expr        Bad expression
 *  @param   len         Length of string error
 */

void PrintBadExpr (const char* logname, const Expression& expr, size_t len);

//------------------------------------------------------------------------------

#endif // CALCULATOR_H_INCLUDED
