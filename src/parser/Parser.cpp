/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++

   Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

// Take the name prefix into account.
#define yylex   snowlex

#include "Parser.h"

/* User implementation prologue.  */
#line 68 "Parser.yy"


#include "Driver.h"
#include "Scanner.h"

#undef yylex
#define yylex driver.lexer->lex



/* Line 317 of lalr1.cc.  */
#line 54 "Parser.cpp"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG							\
  for (bool yydebugcond_ = yydebug_; yydebugcond_; yydebugcond_ = false)	\
    (*yycdebug_)

/* Enable debugging if requested.  */
#if YYDEBUG

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab

namespace snow
{
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  Parser::Parser (Driver& driver_yyarg)
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
      driver (driver_yyarg)
  {
  }

  Parser::~Parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  Parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  Parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif /* ! YYDEBUG */

  void
  Parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  Parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }


  int
  Parser::parse ()
  {
    /// Look-ahead and look-ahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the look-ahead.
    semantic_type yylval;
    /// Location of the look-ahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    #line 21 "Parser.yy"
{
    yylloc.begin.filename = yylloc.end.filename = &driver.streamname;
}
  /* Line 555 of yacc.c.  */
#line 290 "Parser.cpp"
    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;
    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without look-ahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a look-ahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Accept?  */
    if (yyn == yyfinal_)
      goto yyacceptlab;

    /* Shift the look-ahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted unless it is eof.  */
    if (yychar != yyeof_)
      yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:
#line 80 "Parser.yy"
    { (yyval.function_defintion) = new ast::FunctionDefinition; (yyval.function_defintion)->sequence = (yysemantic_stack_[(1) - (1)].sequence); driver.scope = (yyval.function_defintion); ;}
    break;

  case 3:
#line 83 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 4:
#line 84 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 5:
#line 85 "Parser.yy"
    { (yyval.node) = new ast::Loop((yysemantic_stack_[(5) - (4)].sequence), (yysemantic_stack_[(5) - (2)].node)); ;}
    break;

  case 6:
#line 86 "Parser.yy"
    { (yyval.node) = new ast::Loop((yysemantic_stack_[(3) - (3)].node), (yysemantic_stack_[(3) - (1)].node)); ;}
    break;

  case 7:
#line 87 "Parser.yy"
    { (yyval.node) = new ast::Loop((yysemantic_stack_[(4) - (4)].node), (yysemantic_stack_[(4) - (2)].sequence)); ;}
    break;

  case 10:
#line 93 "Parser.yy"
    { (yyval.node) = new ast::IfCondition((yysemantic_stack_[(3) - (3)].node), (yysemantic_stack_[(3) - (1)].node)); ;}
    break;

  case 11:
#line 94 "Parser.yy"
    { (yyval.node) = new ast::IfCondition((yysemantic_stack_[(3) - (3)].node), (yysemantic_stack_[(3) - (1)].node), true); ;}
    break;

  case 16:
#line 105 "Parser.yy"
    { (yyval.sequence) = new ast::Sequence; ;}
    break;

  case 17:
#line 106 "Parser.yy"
    { (yyval.sequence) = (yysemantic_stack_[(2) - (1)].sequence); ;}
    break;

  case 18:
#line 107 "Parser.yy"
    { (yyval.sequence) = (yysemantic_stack_[(2) - (1)].sequence); (yysemantic_stack_[(2) - (1)].sequence)->add((yysemantic_stack_[(2) - (2)].node)); ;}
    break;

  case 19:
#line 110 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 20:
#line 111 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 21:
#line 114 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 22:
#line 116 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 23:
#line 117 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 24:
#line 136 "Parser.yy"
    { (yyval.node) = new ast::Return; ;}
    break;

  case 25:
#line 137 "Parser.yy"
    { (yyval.node) = new ast::Return((yysemantic_stack_[(2) - (2)].node)); ;}
    break;

  case 26:
#line 140 "Parser.yy"
    { (yyval.node) = new ast::Get(new ast::Identifier("self"), (yysemantic_stack_[(2) - (2)].identifier)); ;}
    break;

  case 27:
#line 141 "Parser.yy"
    { (yyval.node) = new ast::Get((yysemantic_stack_[(3) - (1)].identifier), (yysemantic_stack_[(3) - (3)].identifier)); ;}
    break;

  case 28:
#line 142 "Parser.yy"
    { (yyval.node) = new ast::Get((yysemantic_stack_[(3) - (1)].node), (yysemantic_stack_[(3) - (3)].identifier)); ;}
    break;

  case 29:
#line 145 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].identifier); ;}
    break;

  case 30:
#line 148 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 31:
#line 149 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 32:
#line 152 "Parser.yy"
    { (yyval.list) = new std::list<ast::Node*>; (yyval.list)->push_back((yysemantic_stack_[(1) - (1)].identifier)); ;}
    break;

  case 33:
#line 153 "Parser.yy"
    { (yysemantic_stack_[(3) - (1)].list)->push_back((yysemantic_stack_[(3) - (3)].identifier)); ;}
    break;

  case 34:
#line 156 "Parser.yy"
    { (yyval.sequence) = new ast::Sequence((yysemantic_stack_[(1) - (1)].node)); ;}
    break;

  case 35:
#line 157 "Parser.yy"
    { (yyval.sequence) = (yysemantic_stack_[(3) - (1)].sequence); (yyval.sequence)->add((yysemantic_stack_[(3) - (3)].node)); ;}
    break;

  case 36:
#line 160 "Parser.yy"
    { (yyval.function_defintion) = (yysemantic_stack_[(4) - (4)].function_defintion);
                                                              for (auto iter = (yysemantic_stack_[(4) - (2)].list)->begin(); iter != (yysemantic_stack_[(4) - (2)].list)->end(); iter++)
                                                                  (yysemantic_stack_[(4) - (4)].function_defintion)->add_argument(static_cast<ast::Identifier*>(*iter));
                                                            ;}
    break;

  case 37:
#line 164 "Parser.yy"
    { (yyval.function_defintion) = (yysemantic_stack_[(1) - (1)].function_defintion); ;}
    break;

  case 38:
#line 167 "Parser.yy"
    { (yyval.function_defintion) = new ast::FunctionDefinition; (yyval.function_defintion)->sequence = (yysemantic_stack_[(3) - (2)].sequence); ;}
    break;

  case 39:
#line 170 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 40:
#line 171 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 41:
#line 172 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 42:
#line 173 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 43:
#line 174 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 44:
#line 175 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 45:
#line 178 "Parser.yy"
    { (yyval.node) = new ast::Call(static_cast<ast::Get*>((yysemantic_stack_[(3) - (1)].node))->self, static_cast<ast::Get*>((yysemantic_stack_[(3) - (1)].node))->member); ;}
    break;

  case 46:
#line 179 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node)); ;}
    break;

  case 47:
#line 180 "Parser.yy"
    { (yyval.node) = new ast::Call(static_cast<ast::Get*>((yysemantic_stack_[(4) - (1)].node))->self, static_cast<ast::Get*>((yysemantic_stack_[(4) - (1)].node))->member, (yysemantic_stack_[(4) - (3)].sequence)); ;}
    break;

  case 48:
#line 181 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(4) - (1)].node), (yysemantic_stack_[(4) - (3)].sequence)); ;}
    break;

  case 49:
#line 182 "Parser.yy"
    { (yyval.node) = new ast::Get((yysemantic_stack_[(3) - (1)].node), (yysemantic_stack_[(3) - (3)].identifier)); ;}
    break;

  case 50:
#line 183 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(5) - (1)].node), (yysemantic_stack_[(5) - (3)].identifier)); ;}
    break;

  case 51:
#line 184 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(6) - (1)].node), (yysemantic_stack_[(6) - (3)].identifier), (yysemantic_stack_[(6) - (5)].sequence)); ;}
    break;

  case 52:
#line 187 "Parser.yy"
    { (yyval.node) = new ast::Assignment(static_cast<ast::Identifier*>((yysemantic_stack_[(3) - (1)].node)), (yysemantic_stack_[(3) - (3)].node)); ;}
    break;

  case 53:
#line 188 "Parser.yy"
    { (yyval.node) = new ast::Set(dynamic_cast<ast::Get*>((yysemantic_stack_[(3) - (1)].node)), (yysemantic_stack_[(3) - (3)].node)); ;}
    break;

  case 54:
#line 191 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("+"), args); ;}
    break;

  case 55:
#line 193 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("-"), args); ;}
    break;

  case 56:
#line 195 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("*"), args); ;}
    break;

  case 57:
#line 197 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("/"), args); ;}
    break;

  case 58:
#line 199 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(2) - (2)].node), new ast::Identifier("-")); ;}
    break;

  case 59:
#line 200 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("%"), args); ;}
    break;

  case 60:
#line 202 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("**"), args); ;}
    break;

  case 61:
#line 206 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("="), args); ;}
    break;

  case 62:
#line 208 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier(">"), args); ;}
    break;

  case 63:
#line 210 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier(">="), args); ;}
    break;

  case 64:
#line 212 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("<"), args); ;}
    break;

  case 65:
#line 214 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("<="), args); ;}
    break;

  case 66:
#line 216 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("and"), args); ;}
    break;

  case 67:
#line 218 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("or"), args); ;}
    break;

  case 68:
#line 220 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(2) - (2)].node), new ast::Identifier("not")); ;}
    break;

  case 69:
#line 223 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("<<"), args); ;}
    break;

  case 70:
#line 225 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier(">>"), args); ;}
    break;

  case 71:
#line 227 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("|"), args); ;}
    break;

  case 72:
#line 229 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("&"), args); ;}
    break;

  case 73:
#line 231 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("^"), args); ;}
    break;

  case 74:
#line 233 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(2) - (1)].node), new ast::Identifier("~")); ;}
    break;

  case 75:
#line 236 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 76:
#line 237 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].function_defintion); ;}
    break;

  case 77:
#line 238 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 78:
#line 239 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 79:
#line 240 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 80:
#line 241 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 81:
#line 242 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 82:
#line 243 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 83:
#line 244 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(3) - (2)].node); ;}
    break;


    /* Line 675 of lalr1.cc.  */
#line 808 "Parser.cpp"
	default: break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse look-ahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    if (yyn == yyfinal_)
      goto yyacceptlab;

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the look-ahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		   &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyeof_ && yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (int yystate)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char Parser::yypact_ninf_ = -85;
  const short int
  Parser::yypact_[] =
  {
       -85,    26,   186,   -85,    -9,   -85,   -85,   -85,   -85,   -85,
     -85,   -85,   -85,    11,    14,    15,   -85,    -9,   -85,    -9,
      -9,    -9,   -85,    -9,    -9,    -9,   -85,   -85,    10,   -85,
     -85,     7,    88,   -85,   -85,   -85,   -85,   -85,   -85,   -85,
     -85,   -85,   494,   494,    17,   -85,   -85,    79,   143,   342,
     374,   406,   438,   229,    38,   -17,   -17,    -9,    -9,    -9,
      21,   269,    -9,   283,    -9,    25,    -9,    -9,    -9,    -9,
      -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,
      -9,    -9,    -9,    -9,   -85,    22,    27,   -85,   -85,   -85,
     -85,   -85,    -9,   494,   494,   494,   -85,   -85,    -2,   494,
     494,   -85,    -1,   494,    23,   518,   309,   309,   309,   309,
     309,   309,    38,    38,    38,   132,   132,    82,    82,   -17,
     -17,   -17,   -17,   -85,   -85,   186,   186,    91,   494,   -85,
      -9,   -85,   323,     0,     0,   -85,   494,   -85,    65,    20,
      -9,    44,    45,   -85,   -85,   470,   -85,   -85,   186,   -85,
     186
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
        16,     0,     2,     1,    24,    22,    23,    39,    40,    41,
      42,    43,    44,    29,     0,     0,    16,     0,    17,     0,
       0,     0,    16,     0,     0,     0,    18,     4,     3,    20,
      21,    30,    31,    77,    76,    37,    75,    78,    79,    80,
      81,    82,    19,    25,     0,    26,    32,     0,     0,     0,
       0,     0,     0,     0,    74,    58,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,    38,    83,    16,
      16,    16,     0,    11,    10,     6,    28,    45,     0,    34,
      53,    46,     0,    52,    49,    61,    62,    64,    67,    66,
      65,    63,    71,    72,    73,    70,    69,    54,    55,    56,
      57,    59,    60,    36,    33,    12,    12,     0,     7,    47,
       0,    48,     0,    14,    14,     5,    35,    50,     0,     0,
       0,     0,     0,    51,    16,     0,     9,     8,    15,    16,
      13
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Parser::yypgoto_[] =
  {
       -85,   -85,   -85,   -85,   -77,   -84,     2,   -85,   -85,   -85,
     -85,   -85,   -85,   -85,   -52,   -85,    13,   -85,   -85,   -85,
     -85,   -85,   -85,    -4
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     1,    26,    27,   133,   141,     2,    28,    29,    30,
      31,    32,    33,    47,    98,    34,    35,    36,    37,    38,
      39,    40,    41,    42
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned char
  Parser::yytable_[] =
  {
        43,    65,     7,     8,     9,    10,    11,    12,    13,    14,
      15,   102,    16,    49,    17,    50,    51,    52,    48,    54,
      55,    56,   129,   131,    53,    60,     3,   139,   140,    44,
      61,    45,    46,    23,    84,    83,    57,    24,    96,    58,
      59,    25,   104,    16,   124,   144,   132,   146,   147,   134,
     142,   130,   130,    93,    94,    95,    65,    99,   100,    99,
     103,    62,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     138,    76,    77,    78,    79,    80,    81,    82,   128,   143,
      83,   125,   126,   127,   135,     4,     5,     6,   123,    85,
      65,     0,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    63,    16,     0,    17,     0,    18,    19,   130,     0,
      20,    21,    22,     0,     0,     0,   136,     0,    99,    80,
      81,    82,    86,    23,    83,     0,   145,    24,     0,     0,
       0,    25,    64,     0,     0,     0,   148,     4,     5,     6,
      65,   150,     0,     0,     7,     8,     9,    10,    11,    12,
      13,    14,    15,     0,    16,    87,    17,     0,    18,    19,
       0,     0,    20,    21,    22,     0,     0,    78,    79,    80,
      81,    82,     0,     0,    83,    23,     0,     0,     0,    24,
       4,     5,     6,    25,     0,     0,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,     0,    16,     0,    17,
       0,    18,    19,     0,     0,    20,    21,    22,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,     0,
       0,     0,    24,     4,     5,     6,    25,     0,     0,     0,
       7,     8,     9,    10,    11,    12,    13,    14,    15,     0,
      16,     0,    17,     0,    18,    19,     0,     0,    20,    92,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    23,     0,     0,     0,    24,     0,     0,     0,    25,
       7,     8,     9,    10,    11,    12,    13,    14,    15,     0,
      16,     0,    17,    97,     7,     8,     9,    10,    11,    12,
      13,    14,    15,     0,    16,     0,    17,   101,     0,     0,
       0,    23,     0,     0,     0,    24,     0,     0,     0,    25,
       0,     0,     0,     0,     0,    23,     0,    65,     0,    24,
       0,     0,     0,    25,     7,     8,     9,    10,    11,    12,
      13,    14,    15,     0,    16,     0,    17,   137,    73,    74,
      75,     0,    76,    77,    78,    79,    80,    81,    82,     0,
      65,    83,     0,     0,     0,    23,    88,     0,     0,    24,
       0,     0,     0,    25,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,    78,    79,    80,
      81,    82,    65,     0,    83,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,    80,    81,    82,    65,     0,    83,     0,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,    80,    81,    82,    65,     0,    83,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,    80,    81,    82,    65,     0,
      83,     0,     0,     0,     0,   149,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    65,    76,    77,    78,    79,    80,    81,    82,
       0,     0,    83,     0,     0,     0,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    65,    76,    77,    78,
      79,    80,    81,    82,     0,     0,    83,     0,     0,     0,
       0,    67,    68,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,    80,    81,    82,     0,     0,
      83
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
         4,    18,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    63,    21,    17,    23,    19,    20,    21,    16,    23,
      24,    25,    24,    24,    22,    18,     0,    27,    28,    18,
      23,    17,    17,    42,    17,    52,    26,    46,    17,    29,
      30,    50,    17,    21,    17,    25,    23,     3,     3,   126,
     134,    53,    53,    57,    58,    59,    18,    61,    62,    63,
      64,    54,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
     132,    43,    44,    45,    46,    47,    48,    49,    92,    24,
      52,    89,    90,    91,     3,     4,     5,     6,    85,    20,
      18,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    23,    21,    -1,    23,    -1,    25,    26,    53,    -1,
      29,    30,    31,    -1,    -1,    -1,   130,    -1,   132,    47,
      48,    49,    53,    42,    52,    -1,   140,    46,    -1,    -1,
      -1,    50,    54,    -1,    -1,    -1,   144,     4,     5,     6,
      18,   149,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    22,    23,    -1,    25,    26,
      -1,    -1,    29,    30,    31,    -1,    -1,    45,    46,    47,
      48,    49,    -1,    -1,    52,    42,    -1,    -1,    -1,    46,
       4,     5,     6,    50,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      -1,    25,    26,    -1,    -1,    29,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    46,     4,     5,     6,    50,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      21,    -1,    23,    -1,    25,    26,    -1,    -1,    29,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      21,    -1,    23,    24,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    -1,    23,    24,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    42,    -1,    18,    -1,    46,
      -1,    -1,    -1,    50,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    -1,    23,    24,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,    -1,
      18,    52,    -1,    -1,    -1,    42,    24,    -1,    -1,    46,
      -1,    -1,    -1,    50,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    45,    46,    47,
      48,    49,    18,    -1,    52,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    18,    -1,    52,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    45,    46,    47,    48,    49,    18,    -1,    52,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    45,    46,    47,    48,    49,    18,    -1,
      52,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    18,    43,    44,    45,    46,    47,    48,    49,
      -1,    -1,    52,    -1,    -1,    -1,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    18,    43,    44,    45,
      46,    47,    48,    49,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    45,    46,    47,    48,    49,    -1,    -1,
      52
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,    56,    61,     0,     4,     5,     6,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    21,    23,    25,    26,
      29,    30,    31,    42,    46,    50,    57,    58,    62,    63,
      64,    65,    66,    67,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    78,    18,    17,    17,    68,    61,    78,
      78,    78,    78,    61,    78,    78,    78,    26,    29,    30,
      18,    23,    54,    23,    54,    18,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    43,    44,    45,    46,
      47,    48,    49,    52,    17,    20,    53,    22,    24,    25,
      25,    25,    30,    78,    78,    78,    17,    24,    69,    78,
      78,    24,    69,    78,    17,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    71,    17,    61,    61,    61,    78,    24,
      53,    24,    23,    59,    59,     3,    78,    24,    69,    27,
      28,    60,    60,    24,    25,    78,     3,     3,    61,    25,
      61
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,    46,    91,
      93,   123,   125,    40,    41,   273,   274,   275,   276,   277,
     278,   279,    61,    62,    60,   280,   281,   282,   283,   124,
      38,    94,   126,   284,   285,    43,    45,    42,    47,    37,
     286,   287,   288,    44,    58
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    55,    56,    57,    57,    57,    57,    57,    58,    58,
      58,    58,    59,    59,    60,    60,    61,    61,    61,    62,
      62,    63,    63,    63,    64,    64,    65,    65,    65,    66,
      67,    67,    68,    68,    69,    69,    70,    70,    71,    72,
      72,    72,    72,    72,    72,    73,    73,    73,    73,    73,
      73,    73,    74,    74,    75,    75,    75,    75,    75,    75,
      75,    76,    76,    76,    76,    76,    76,    76,    76,    77,
      77,    77,    77,    77,    77,    78,    78,    78,    78,    78,
      78,    78,    78,    78
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     1,     1,     5,     3,     4,     7,     7,
       3,     3,     0,     5,     0,     3,     0,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     3,     3,     1,
       1,     1,     1,     3,     1,     3,     4,     1,     3,     1,
       1,     1,     1,     1,     1,     3,     3,     4,     4,     3,
       5,     6,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     3,     3,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     3
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Parser::yytname_[] =
  {
    "END_FILE", "error", "$undefined", "END", "RETURN", "BREAK", "CONTINUE",
  "THROW", "CATCH", "TRY", "FINALLY", "INTEGER", "FLOAT", "STRING", "TRUE",
  "FALSE", "NIL", "IDENTIFIER", "'.'", "'['", "']'", "'{'", "'}'", "'('",
  "')'", "EOL", "UNLESS", "ELSE", "ELSIF", "IF", "WHILE", "DO", "'='",
  "'>'", "'<'", "LOG_OR", "LOG_AND", "LTE", "GTE", "'|'", "'&'", "'^'",
  "'~'", "RSHFT", "LSHFT", "'+'", "'-'", "'*'", "'/'", "'%'", "LOG_NOT",
  "NEG", "POW", "','", "':'", "$accept", "program", "statement",
  "conditional", "elsif_cond", "else_cond", "sequence", "function",
  "command", "return_cmd", "scoped_var", "local_var", "variable",
  "parameters", "arguments", "closure", "scope", "literal",
  "function_call", "assignment", "mathematical_operation",
  "logical_operation", "bitwise_operation", "expression", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        56,     0,    -1,    61,    -1,    62,    -1,    58,    -1,    30,
      78,    25,    61,     3,    -1,    62,    30,    78,    -1,    31,
      61,    30,    78,    -1,    29,    78,    25,    61,    59,    60,
       3,    -1,    26,    78,    25,    61,    59,    60,     3,    -1,
      62,    29,    78,    -1,    62,    26,    78,    -1,    -1,    59,
      28,    78,    25,    61,    -1,    -1,    27,    25,    61,    -1,
      -1,    61,    25,    -1,    61,    57,    -1,    78,    -1,    63,
      -1,    64,    -1,     5,    -1,     6,    -1,     4,    -1,     4,
      78,    -1,    18,    17,    -1,    17,    18,    17,    -1,    65,
      18,    17,    -1,    17,    -1,    65,    -1,    66,    -1,    17,
      -1,    68,    53,    17,    -1,    78,    -1,    69,    53,    78,
      -1,    19,    68,    20,    71,    -1,    71,    -1,    21,    61,
      22,    -1,    11,    -1,    12,    -1,    13,    -1,    14,    -1,
      15,    -1,    16,    -1,    65,    23,    24,    -1,    66,    23,
      24,    -1,    65,    23,    69,    24,    -1,    66,    23,    69,
      24,    -1,    78,    18,    17,    -1,    78,    18,    17,    23,
      24,    -1,    78,    18,    17,    23,    69,    24,    -1,    66,
      54,    78,    -1,    65,    54,    78,    -1,    78,    45,    78,
      -1,    78,    46,    78,    -1,    78,    47,    78,    -1,    78,
      48,    78,    -1,    46,    78,    -1,    78,    49,    78,    -1,
      78,    52,    78,    -1,    78,    32,    78,    -1,    78,    33,
      78,    -1,    78,    38,    78,    -1,    78,    34,    78,    -1,
      78,    37,    78,    -1,    78,    36,    78,    -1,    78,    35,
      78,    -1,    50,    78,    -1,    78,    44,    78,    -1,    78,
      43,    78,    -1,    78,    39,    78,    -1,    78,    40,    78,
      -1,    78,    41,    78,    -1,    42,    78,    -1,    72,    -1,
      70,    -1,    67,    -1,    73,    -1,    74,    -1,    75,    -1,
      76,    -1,    77,    -1,    23,    78,    24,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     7,     9,    15,    19,    24,    32,
      40,    44,    48,    49,    55,    56,    60,    61,    64,    67,
      69,    71,    73,    75,    77,    79,    82,    85,    89,    93,
      95,    97,    99,   101,   105,   107,   111,   116,   118,   122,
     124,   126,   128,   130,   132,   134,   138,   142,   147,   152,
     156,   162,   169,   173,   177,   181,   185,   189,   193,   196,
     200,   204,   208,   212,   216,   220,   224,   228,   232,   235,
     239,   243,   247,   251,   255,   258,   260,   262,   264,   266,
     268,   270,   272,   274
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  Parser::yyrline_[] =
  {
         0,    80,    80,    83,    84,    85,    86,    87,    91,    92,
      93,    94,    97,    98,   101,   102,   105,   106,   107,   110,
     111,   114,   116,   117,   136,   137,   140,   141,   142,   145,
     148,   149,   152,   153,   156,   157,   160,   164,   167,   170,
     171,   172,   173,   174,   175,   178,   179,   180,   181,   182,
     183,   184,   187,   188,   191,   193,   195,   197,   199,   200,
     202,   206,   208,   210,   212,   214,   216,   218,   220,   223,
     225,   227,   229,   231,   233,   236,   237,   238,   239,   240,
     241,   242,   243,   244
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "), ";
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    49,    40,     2,
      23,    24,    47,    45,    53,    46,    18,    48,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    54,     2,
      34,    32,    33,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    19,     2,    20,    41,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    21,    39,    22,    42,     2,     2,     2,
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
      15,    16,    17,    25,    26,    27,    28,    29,    30,    31,
      35,    36,    37,    38,    43,    44,    50,    51,    52
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 570;
  const int Parser::yynnts_ = 24;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 3;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 55;

  const unsigned int Parser::yyuser_token_number_max_ = 288;
  const Parser::token_number_type Parser::yyundef_token_ = 2;

} // namespace snow

#line 247 "Parser.yy"


void snow::Parser::error(const Parser::location_type& l, const std::string& m)
{
    driver.error(l, m);
}
