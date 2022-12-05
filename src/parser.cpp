// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.h"


// Unqualified %code blocks.
#line 21 "parser.y"

    #include <iostream>
    #include <cstdlib>
    #include <fstream>

    #include "interpreter.h"
    #include "node.h"

    #undef yylex
    #define yylex scanner.yylex

#line 58 "/home/karnokg/Projects/pseudo/src/parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 6 "parser.y"
namespace  Pseudo  {
#line 151 "/home/karnokg/Projects/pseudo/src/parser.cpp"

  /// Build a parser object.
   Parser :: Parser  (Scanner& scanner_yyarg, Interpreter& driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

   Parser ::~ Parser  ()
  {}

   Parser ::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
   Parser ::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
    , location (that.location)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
   Parser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_MOVE_REF (location_type) l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
   Parser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v, YY_RVREF (location_type) l)
    : Base (t)
    , value (YY_MOVE (v))
    , location (YY_MOVE (l))
  {}


  template <typename Base>
   Parser ::symbol_kind_type
   Parser ::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
   Parser ::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
   Parser ::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
    location = YY_MOVE (s.location);
  }

  // by_kind.
   Parser ::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
   Parser ::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

   Parser ::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

   Parser ::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
   Parser ::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
   Parser ::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

   Parser ::symbol_kind_type
   Parser ::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


   Parser ::symbol_kind_type
   Parser ::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
   Parser ::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

   Parser ::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
   Parser ::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
   Parser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

   Parser ::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

   Parser ::symbol_kind_type
   Parser ::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

   Parser ::stack_symbol_type::stack_symbol_type ()
  {}

   Parser ::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value), YY_MOVE (that.location))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

   Parser ::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value), YY_MOVE (that.location))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }

   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
   Parser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YY_USE (yysym.kind ());
  }

#if YYDEBUG
  template <typename Base>
  void
   Parser ::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
   Parser ::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
   Parser ::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
   Parser ::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
   Parser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   Parser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   Parser ::debug_level_type
   Parser ::debug_level () const
  {
    return yydebug_;
  }

  void
   Parser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

   Parser ::state_type
   Parser ::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
   Parser ::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
   Parser ::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
   Parser ::operator() ()
  {
    return parse ();
  }

  int
   Parser ::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: stmts
#line 126 "parser.y"
    {
        driver.set_ast_root((yystack_[0].value.block));
        std::cout << "stmts <<EOF>>" << std::endl;
        std::cout << "program_block = stmts";
    }
#line 629 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 3: // stmts: stmt
#line 135 "parser.y"
    {
        (yylhs.value.block) = new NBlock();
        (yylhs.value.block)->statements.push_back((yystack_[0].value.stmt));
        std::cout << "stmts -> stmt" << std::endl;
    }
#line 639 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 4: // stmts: stmts stmt
#line 142 "parser.y"
    {
        (yystack_[1].value.block)->statements.push_back((yystack_[0].value.stmt));
        std::cout << "stmts -> stmts stmt" << std::endl;
    }
#line 648 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 5: // stmt: var_decl close
#line 150 "parser.y"
    {
        std::cout << "stmts => var_decl" << std::endl;
    }
#line 656 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 6: // stmt: func_decl close
#line 155 "parser.y"
    {
        std::cout << "stmts => func_decl" << std::endl;
    }
#line 664 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 7: // stmt: assign_stmt close
#line 160 "parser.y"
    {
        std::cout << "stmts => assign_stmt" << std::endl;
    }
#line 672 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 8: // stmt: expr close
#line 165 "parser.y"
    {
        std::cout << "stmts => expr" << std::endl;
    }
#line 680 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 9: // func_decl: T_FUNCTION type_specifier T_IDENTIFIER T_LPAREN func_decl_args T_RPAREN close block T_FUNCTION T_END
#line 172 "parser.y"
    {
        std::cout << "func_decl_stmt => T_FUNCTION type_specifier T_IDENTIFIER T_LPAREN func_decl_args T_RPAREN block T_FUNCTION T_END" << std::endl;
    }
#line 688 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 10: // func_decl_args: %empty
#line 179 "parser.y"
    { 
        std::cout << "function_decl_args => empty" << std::endl;
    }
#line 696 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 11: // func_decl_args: var_decl
#line 183 "parser.y"
    {
        std::cout << "func_decl_args => var_decl" << std::endl;
    }
#line 704 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 12: // func_decl_args: func_decl_args T_COMMA var_decl
#line 187 "parser.y"
    {
        std::cout << "funcition_decl_args => func_decl_args T_COMMA var_decl" << std::endl;
    }
#line 712 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 13: // var_decl: T_IDENTIFIER T_COLON type_specifier T_ASSIGN expr
#line 194 "parser.y"
    {
        std::cout << "var_decl => T_IDENTIFIER T_COLON type_specifier T_ASSIGN expression" << std::endl;
    }
#line 720 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 14: // var_decl: T_IDENTIFIER T_COLON type_specifier
#line 198 "parser.y"
    {
        std::cout << "var_decl => T_IDENTIFIER type_specifier T_IDENTIFIER" << std::endl;
    }
#line 728 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 15: // assign_stmt: T_IDENTIFIER T_ASSIGN expr
#line 205 "parser.y"
    {
        std::cout << "assign_statement -> T_IDENTIFIER T_ASSIGN expr" << std::endl;
    }
#line 736 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 16: // block: T_INDENT stmts T_OUTDENT
#line 213 "parser.y"
    {
        (yylhs.value.block) = (yystack_[1].value.block);
    }
#line 744 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 17: // block: T_INDENT T_OUTDENT
#line 217 "parser.y"
    {
        (yylhs.value.block) = new NBlock();
    }
#line 752 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 18: // close: T_COMMA
#line 224 "parser.y"
    {
        std::cout << "close -> comma" << std::endl;
    }
#line 760 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 19: // close: T_NEWLINE
#line 229 "parser.y"
    {
        std::cout << "close -> newline" << std::endl;
    }
#line 768 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 20: // expr: func_call
#line 235 "parser.y"
    {
    }
#line 775 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 21: // expr: T_IDENTIFIER
#line 239 "parser.y"
    {
    }
#line 782 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 22: // expr: numeric
#line 243 "parser.y"
    {

    }
#line 790 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 23: // expr: logical
#line 248 "parser.y"
    {
        std::cout << "expr -> logical" << std::endl;
    }
#line 798 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 24: // expr: expr T_ADD expr
#line 253 "parser.y"
    {
        std::cout << "expr -> expr T_ADD expr" << std::endl;
    }
#line 806 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 25: // expr: expr T_MINUS expr
#line 258 "parser.y"
    {
        std::cout << "expr -> expr T_MINUS expr" << std::endl;
    }
#line 814 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 26: // expr: expr T_MUL expr
#line 263 "parser.y"
    {
        std::cout << "expr -> expr T_MUL expr" << std::endl;
    }
#line 822 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 27: // expr: expr T_DIV expr
#line 268 "parser.y"
    {
        std::cout << "expr -> expr T_DIV expr" << std::endl;
    }
#line 830 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 28: // expr: expr T_MODULUS expr
#line 273 "parser.y"
    {
        std::cout << "expr -> expr T_MODULUS expr" << std::endl;
    }
#line 838 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 29: // expr: expr T_LESS expr
#line 278 "parser.y"
    {
        std::cout << "expr -> expr T_LESS expr" << std::endl;
    }
#line 846 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 30: // expr: expr T_GREATER expr
#line 283 "parser.y"
    {
        std::cout << "expr -> expr T_GREATER expr" << std::endl;
    }
#line 854 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 31: // expr: expr T_LESSEQ expr
#line 288 "parser.y"
    {
        std::cout << "expr -> expr T_LESSEQ expr" << std::endl;
    }
#line 862 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 32: // expr: expr T_GREATEREQ expr
#line 293 "parser.y"
    {
        std::cout << "expr -> expr T_GREATEREQ expr" << std::endl;
    }
#line 870 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 33: // expr: expr T_AND expr
#line 298 "parser.y"
    {
        std::cout << "expr -> expr T_AND expr" << std::endl;
    }
#line 878 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 34: // expr: expr T_OR expr
#line 303 "parser.y"
    {
        std::cout << "expr -> expr T_OR expr" << std::endl;
    }
#line 886 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 35: // expr: expr T_EQ expr
#line 308 "parser.y"
    {
        std::cout << "expr -> expr T_EQ expr" << std::endl;
    }
#line 894 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 36: // expr: expr T_NOTEQ expr
#line 313 "parser.y"
    {
        std::cout << "expr -> expr T_NOTEQ expr" << std::endl;
    }
#line 902 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 37: // expr: T_NOT expr
#line 318 "parser.y"
    {
        std::cout << "expr -> T_NOT expr" << std::endl;
    }
#line 910 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 38: // expr: T_LPAREN expr T_RPAREN
#line 322 "parser.y"
    {
    }
#line 917 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 39: // func_call: T_IDENTIFIER T_LPAREN call_args T_RPAREN
#line 327 "parser.y"
    {
        std::cout << "func_call => T_IDENTIFIER T_LPAREN call_args T_RPAREN" << std::endl;
    }
#line 925 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 40: // call_args: %empty
#line 334 "parser.y"
    { 
        std::cout << "call_args => epsilon" << std::endl;
    }
#line 933 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 41: // call_args: expr
#line 338 "parser.y"
    {
        std::cout << "call_args => expr" << std::endl;
    }
#line 941 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 42: // call_args: call_args T_COMMA expr
#line 342 "parser.y"
    {
        std::cout << "call_args => call_args T_COMMA expr" << std::endl;
    }
#line 949 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 43: // numeric: T_INTEGER_VAL
#line 349 "parser.y"
    {
        std::cout << "numeric => T_INTEGER_VAL" << std::endl;
    }
#line 957 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 44: // numeric: T_RATIONAL_VAL
#line 353 "parser.y"
    {
        std::cout << "numeric => T_RATIONAL_VAL" << std::endl;
    }
#line 965 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 45: // logical: T_TRUE
#line 360 "parser.y"
    {
        std::cout << "logical -> T_TRUE" << std::endl;
    }
#line 973 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 46: // logical: T_FALSE
#line 364 "parser.y"
    {
        std::cout << "logical -> T_FALSE" << std::endl;
    }
#line 981 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 47: // type_specifier: T_BOOLEAN
#line 370 "parser.y"
    {
        std::cout << "type_specifier -> T_BOOLEAN" << std::endl;
    }
#line 989 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 48: // type_specifier: T_INTEGER
#line 374 "parser.y"
    {
        std::cout << "type_specifier -> T_INTEGER" << std::endl;
    }
#line 997 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 49: // type_specifier: T_RATIONAL
#line 378 "parser.y"
    {
        std::cout << "type_specifier -> T_RATIONAL" << std::endl;
    }
#line 1005 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 50: // type_specifier: T_STRING
#line 382 "parser.y"
    {
        std::cout << "type_specifier -> T_STRING" << std::endl;
    }
#line 1013 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;

  case 51: // type_specifier: T_VOID
#line 386 "parser.y"
    {
        std::cout << "type_specifier -> T_VOID" << std::endl;
    }
#line 1021 "/home/karnokg/Projects/pseudo/src/parser.cpp"
    break;


#line 1025 "/home/karnokg/Projects/pseudo/src/parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
   Parser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
   Parser ::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", "T_PROGRAM", "T_VOID",
  "T_FUNCTION", "T_END", "T_IF", "T_ELSE", "T_THEN", "T_IF_END",
  "T_ITERATE", "T_WHILE", "T_FROM", "T_TO", "T_RETURN", "T_BOOLEAN",
  "T_INTEGER", "T_RATIONAL", "T_ARRAY", "T_PRINT", "T_STRING", "T_NEWLINE",
  "T_ASSIGN", "T_SWAP", "T_EQ", "T_NOTEQ", "T_LESS", "T_LESSEQ",
  "T_GREATER", "T_GREATEREQ", "T_ADD", "T_MINUS", "T_DIV", "T_MUL",
  "T_MODULUS", "T_AND", "T_OR", "T_LPAREN", "T_RPAREN", "T_LSBRACKET",
  "T_RSBRACKET", "T_COMMA", "T_DOT", "T_NOT", "T_COLON", "T_QUESTIONMARK",
  "T_IDENTIFIER", "T_INTEGER_VAL", "T_RATIONAL_VAL", "T_STRING_VAL",
  "T_TRUE", "T_FALSE", "T_INDENT", "T_OUTDENT", "T_ENDOFFILE", "$accept",
  "program", "stmts", "stmt", "func_decl", "func_decl_args", "var_decl",
  "assign_stmt", "block", "close", "expr", "func_call", "call_args",
  "numeric", "logical", "type_specifier", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  //  Parser ::context.
   Parser ::context::context (const  Parser & yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
   Parser ::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
   Parser ::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
   Parser ::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char  Parser ::yypact_ninf_ = -60;

  const signed char  Parser ::yytable_ninf_ = -1;

  const short
   Parser ::yypact_[] =
  {
      46,     3,    94,    94,   -20,   -60,   -60,   -60,   -60,    23,
      46,   -60,   -13,   -13,   -13,    74,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -21,    -8,    92,   -60,    94,    94,
       3,   -60,   -60,   -60,   -60,   -60,   -60,   -60,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,   -60,    14,   -60,   122,   122,   -31,    30,   156,   156,
       0,     0,     0,     0,   -19,   -19,   -60,   -60,   -60,   147,
     135,     7,   -60,    94,    94,    10,   -29,   -60,   122,   122,
     -13,     7,     4,   -60,    12,    53,   -60,    31,    56,   -60,
     -60
  };

  const signed char
   Parser ::yydefact_[] =
  {
       0,     0,     0,     0,    21,    43,    44,    45,    46,     0,
       2,     3,     0,     0,     0,     0,    20,    22,    23,    51,
      47,    48,    49,    50,     0,    21,     0,    37,     0,    40,
       0,     1,     4,    19,    18,     6,     5,     7,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     8,     0,    38,    15,    41,     0,    14,    35,    36,
      29,    31,    30,    32,    24,    25,    27,    26,    28,    33,
      34,    10,    39,     0,     0,     0,     0,    11,    42,    13,
       0,     0,     0,    12,     0,     0,    17,     0,     0,    16,
       9
  };

  const signed char
   Parser ::yypgoto_[] =
  {
     -60,   -60,   -17,   -10,   -60,   -60,   -59,   -60,   -60,    -9,
      -1,   -60,   -60,   -60,   -60,    35
  };

  const signed char
   Parser ::yydefgoto_[] =
  {
       0,     9,    10,    11,    12,    76,    13,    14,    85,    35,
      15,    16,    56,    17,    18,    24
  };

  const signed char
   Parser ::yytable_[] =
  {
      32,    26,    27,    28,    36,    37,    51,    19,    72,    33,
      80,    73,    77,    81,    46,    47,    48,     1,    29,    20,
      21,    22,    83,    31,    23,    30,    52,    54,    55,    34,
      29,    44,    45,    46,    47,    48,     1,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
       2,     1,    71,    74,    75,    30,     3,    84,    88,     4,
       5,     6,    90,     7,     8,    57,    86,    87,     0,     2,
       0,    82,    78,    79,     0,     3,     0,    32,     4,     5,
       6,     0,     7,     8,     2,    89,     0,     0,     0,     0,
       3,     0,     0,     4,     5,     6,    33,     7,     8,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,     0,     0,     0,     0,    34,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
       0,    53,     2,     0,     0,     0,     0,     0,     3,     0,
       0,    25,     5,     6,     0,     7,     8,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    40,    41,    42,    43,    44,    45,    46,
      47,    48
  };

  const signed char
   Parser ::yycheck_[] =
  {
      10,     2,     3,    23,    13,    14,    15,     4,    39,    22,
      39,    42,    71,    42,    33,    34,    35,     5,    38,    16,
      17,    18,    81,     0,    21,    45,    47,    28,    29,    42,
      38,    31,    32,    33,    34,    35,     5,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      38,     5,    38,    23,    47,    45,    44,    53,     5,    47,
      48,    49,     6,    51,    52,    30,    54,    84,    -1,    38,
      -1,    80,    73,    74,    -1,    44,    -1,    87,    47,    48,
      49,    -1,    51,    52,    38,    54,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    48,    49,    22,    51,    52,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    -1,    -1,    -1,    -1,    42,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      -1,    39,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    48,    49,    -1,    51,    52,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    27,    28,    29,    30,    31,    32,    33,
      34,    35
  };

  const signed char
   Parser ::yystos_[] =
  {
       0,     5,    38,    44,    47,    48,    49,    51,    52,    57,
      58,    59,    60,    62,    63,    66,    67,    69,    70,     4,
      16,    17,    18,    21,    71,    47,    66,    66,    23,    38,
      45,     0,    59,    22,    42,    65,    65,    65,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    65,    47,    39,    66,    66,    68,    71,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    38,    39,    42,    23,    47,    61,    62,    66,    66,
      39,    42,    65,    62,    53,    64,    54,    58,     5,    54,
       6
  };

  const signed char
   Parser ::yyr1_[] =
  {
       0,    56,    57,    58,    58,    59,    59,    59,    59,    60,
      61,    61,    61,    62,    62,    63,    64,    64,    65,    65,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    67,
      68,    68,    68,    69,    69,    70,    70,    71,    71,    71,
      71,    71
  };

  const signed char
   Parser ::yyr2_[] =
  {
       0,     2,     1,     1,     2,     2,     2,     2,     2,    10,
       0,     1,     3,     5,     3,     3,     3,     2,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     3,     4,
       0,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1
  };




#if YYDEBUG
  const short
   Parser ::yyrline_[] =
  {
       0,   125,   125,   134,   141,   149,   154,   159,   164,   171,
     178,   182,   186,   193,   197,   204,   212,   216,   223,   228,
     234,   238,   242,   247,   252,   257,   262,   267,   272,   277,
     282,   287,   292,   297,   302,   307,   312,   317,   321,   326,
     333,   337,   341,   348,   352,   359,   363,   369,   373,   377,
     381,   385
  };

  void
   Parser ::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
   Parser ::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

   Parser ::symbol_kind_type
   Parser ::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
    };
    // Last valid token kind.
    const int code_max = 310;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 6 "parser.y"
} //  Pseudo 
#line 1585 "/home/karnokg/Projects/pseudo/src/parser.cpp"

#line 612 "parser.y"


void Pseudo::Parser::error(const location_type& l, const std::string& err_message)
{
    std::cerr << "Error: " << err_message << " at " << l << std::endl;
}
