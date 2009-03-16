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
# if YYENABLE_NLS
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
    #line 20 "Parser.yy"
{
    yylloc.begin.filename = yylloc.end.filename = &driver.streamname;
}
  /* Line 547 of yacc.c.  */
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
    { (yyval.function_defintion) = new ast::FunctionDefinition((yysemantic_stack_[(1) - (1)].sequence)); ;}
    break;

  case 17:
#line 105 "Parser.yy"
    { (yyval.sequence) = new ast::Sequence; ;}
    break;

  case 18:
#line 106 "Parser.yy"
    { (yyval.sequence) = (yysemantic_stack_[(2) - (1)].sequence); ;}
    break;

  case 19:
#line 107 "Parser.yy"
    { (yyval.sequence) = (yysemantic_stack_[(2) - (1)].sequence); (yysemantic_stack_[(2) - (1)].sequence)->add((yysemantic_stack_[(2) - (2)].node)); ;}
    break;

  case 20:
#line 110 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 21:
#line 111 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 34:
#line 136 "Parser.yy"
    { (yyval.node) = new ast::Return; ;}
    break;

  case 35:
#line 137 "Parser.yy"
    { (yyval.node) = new ast::Return((yysemantic_stack_[(2) - (2)].node)); ;}
    break;

  case 36:
#line 140 "Parser.yy"
    { (yyval.node) = new ast::Get(new ast::Identifier("self"), (yysemantic_stack_[(2) - (2)].identifier)); ;}
    break;

  case 37:
#line 141 "Parser.yy"
    { (yyval.node) = new ast::Get((yysemantic_stack_[(3) - (1)].identifier), (yysemantic_stack_[(3) - (3)].identifier)); ;}
    break;

  case 38:
#line 142 "Parser.yy"
    { (yyval.node) = new ast::Get((yysemantic_stack_[(3) - (1)].node), (yysemantic_stack_[(3) - (3)].identifier)); ;}
    break;

  case 39:
#line 145 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].identifier); ;}
    break;

  case 40:
#line 148 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 41:
#line 149 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 42:
#line 152 "Parser.yy"
    { (yyval.list) = new std::list<ast::Node*>; (yyval.list)->push_back((yysemantic_stack_[(1) - (1)].identifier)); ;}
    break;

  case 43:
#line 153 "Parser.yy"
    { (yysemantic_stack_[(3) - (1)].list)->push_back((yysemantic_stack_[(3) - (3)].identifier)); ;}
    break;

  case 44:
#line 156 "Parser.yy"
    { (yyval.sequence) = new ast::Sequence((yysemantic_stack_[(1) - (1)].node)); ;}
    break;

  case 45:
#line 157 "Parser.yy"
    { (yyval.sequence) = (yysemantic_stack_[(3) - (1)].sequence); (yyval.sequence)->add((yysemantic_stack_[(3) - (3)].node)); ;}
    break;

  case 46:
#line 160 "Parser.yy"
    { (yyval.function_defintion) = (yysemantic_stack_[(4) - (4)].function_defintion);
                                                              for (auto iter = (yysemantic_stack_[(4) - (2)].list)->begin(); iter != (yysemantic_stack_[(4) - (2)].list)->end(); iter++)
                                                                  (yysemantic_stack_[(4) - (4)].function_defintion)->add_argument(static_cast<ast::Identifier*>(*iter));
                                                            ;}
    break;

  case 47:
#line 164 "Parser.yy"
    { (yyval.function_defintion) = (yysemantic_stack_[(1) - (1)].function_defintion); ;}
    break;

  case 48:
#line 167 "Parser.yy"
    { (yyval.function_defintion) = new ast::FunctionDefinition((yysemantic_stack_[(3) - (2)].sequence)); ;}
    break;

  case 49:
#line 170 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 50:
#line 171 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 51:
#line 172 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 52:
#line 173 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 53:
#line 174 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 54:
#line 175 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 55:
#line 178 "Parser.yy"
    { (yyval.node) = new ast::Call(static_cast<ast::Get*>((yysemantic_stack_[(3) - (1)].node))->self, static_cast<ast::Get*>((yysemantic_stack_[(3) - (1)].node))->member); ;}
    break;

  case 56:
#line 179 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node)); ;}
    break;

  case 57:
#line 180 "Parser.yy"
    { (yyval.node) = new ast::Call(static_cast<ast::Get*>((yysemantic_stack_[(4) - (1)].node))->self, static_cast<ast::Get*>((yysemantic_stack_[(4) - (1)].node))->member, (yysemantic_stack_[(4) - (3)].sequence)); ;}
    break;

  case 58:
#line 181 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(4) - (1)].node), (yysemantic_stack_[(4) - (3)].sequence)); ;}
    break;

  case 59:
#line 182 "Parser.yy"
    { (yyval.node) = new ast::Get((yysemantic_stack_[(3) - (1)].node), (yysemantic_stack_[(3) - (3)].identifier)); ;}
    break;

  case 60:
#line 183 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(5) - (1)].node), (yysemantic_stack_[(5) - (3)].identifier)); ;}
    break;

  case 61:
#line 184 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(6) - (1)].node), (yysemantic_stack_[(6) - (3)].identifier), (yysemantic_stack_[(6) - (5)].sequence)); ;}
    break;

  case 62:
#line 187 "Parser.yy"
    { (yyval.node) = new ast::Assignment(static_cast<ast::Identifier*>((yysemantic_stack_[(3) - (1)].node)), (yysemantic_stack_[(3) - (3)].node)); ;}
    break;

  case 63:
#line 188 "Parser.yy"
    { (yyval.node) = new ast::Set(static_cast<ast::Get*>((yysemantic_stack_[(3) - (1)].node)), (yysemantic_stack_[(3) - (3)].node)); ;}
    break;

  case 64:
#line 191 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("+"), args); ;}
    break;

  case 65:
#line 193 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));                   
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("-"), args); ;}
    break;

  case 66:
#line 195 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("*"), args); ;}
    break;

  case 67:
#line 197 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("/"), args); ;}
    break;

  case 68:
#line 199 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(2) - (2)].node));
                                                              (yyval.node) = new ast::Call(new ast::Literal("0", ast::Literal::INTEGER_TYPE), 
                                                                   new ast::Identifier("-"), args); ;}
    break;

  case 69:
#line 202 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("%"), args); ;}
    break;

  case 70:
#line 204 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("**"), args); ;}
    break;

  case 71:
#line 208 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("="), args); ;}
    break;

  case 72:
#line 210 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier(">"), args); ;}
    break;

  case 73:
#line 212 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier(">="), args); ;}
    break;

  case 74:
#line 214 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("<"), args); ;}
    break;

  case 75:
#line 216 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("<="), args); ;}
    break;

  case 76:
#line 218 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("and"), args); ;}
    break;

  case 77:
#line 220 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("or"), args); ;}
    break;

  case 78:
#line 222 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(2) - (2)].node), new ast::Identifier("not")); ;}
    break;

  case 79:
#line 225 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("<<"), args); ;}
    break;

  case 80:
#line 227 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier(">>"), args); ;}
    break;

  case 81:
#line 229 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("|"), args); ;}
    break;

  case 82:
#line 231 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("&"), args); ;}
    break;

  case 83:
#line 233 "Parser.yy"
    { RefPtr<ast::Sequence> args = new ast::Sequence((yysemantic_stack_[(3) - (3)].node));
                                                              (yyval.node) = new ast::Call((yysemantic_stack_[(3) - (1)].node), new ast::Identifier("^"), args); ;}
    break;

  case 84:
#line 235 "Parser.yy"
    { (yyval.node) = new ast::Call((yysemantic_stack_[(2) - (1)].node), new ast::Identifier("~")); ;}
    break;

  case 85:
#line 238 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 86:
#line 239 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].function_defintion); ;}
    break;

  case 87:
#line 240 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 88:
#line 241 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 89:
#line 242 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 90:
#line 243 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 91:
#line 244 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 92:
#line 245 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 93:
#line 246 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(3) - (2)].node); ;}
    break;


    /* Line 675 of lalr1.cc.  */
#line 760 "Parser.cpp"
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
  const signed char Parser::yypact_ninf_ = -67;
  const short int
  Parser::yypact_[] =
  {
       -67,     7,   231,   -67,   332,   -67,   -67,    53,   -67,   -67,
     -67,   -67,   -67,   -67,   -67,    10,    -3,    58,   -67,   332,
     -67,   332,   332,   332,   -67,   332,   332,   332,   -67,   -67,
     -21,   -67,   -67,   -67,   -12,     3,   -67,   -67,   -67,   -67,
     -67,   -67,   -67,   -67,   -67,   503,   503,    45,   -67,   -67,
     231,    62,   -67,   -67,    24,   184,   351,   383,   415,   447,
     278,   176,    48,    48,   332,   332,   332,    63,     6,   332,
      22,   332,    64,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,    59,   -67,    55,    65,   -67,   -67,   -67,   -67,   -67,
     332,   503,   503,   503,   -67,   -67,    96,   503,   503,   -67,
     113,   503,   -10,   527,   541,   541,   541,   541,   541,   541,
     176,   176,   176,    13,    13,   133,   133,    48,    48,    48,
      48,    53,   -67,    60,    80,   -67,   -67,   231,   231,   129,
     503,   -67,   332,   -67,   318,   -14,   231,   -67,   -67,    46,
      46,   -67,   503,   -67,   139,   332,   332,   231,    61,   332,
      81,    84,   -67,   503,   503,   -67,   479,   -67,   -67,   231,
     -67,   231
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
        17,     0,     2,     1,    34,    24,    25,     0,    17,    49,
      50,    51,    52,    53,    54,    39,     0,     0,    17,     0,
      18,     0,     0,     0,    17,     0,     0,     0,    19,     4,
       3,    21,    23,    22,    40,    41,    87,    86,    47,    85,
      88,    89,    90,    91,    92,    20,    35,    40,    41,    26,
      30,     0,    36,    42,     0,     0,     0,     0,     0,     0,
       0,    84,    68,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    37,     0,     0,    48,    93,    17,    17,    17,
       0,    12,    11,     6,    38,    55,     0,    44,    63,    56,
       0,    62,    59,    71,    72,    74,    77,    76,    75,    73,
      81,    82,    83,    80,    79,    64,    65,    66,    67,    69,
      70,     0,    17,     0,     0,    46,    43,    13,    13,     0,
       7,    57,     0,    58,     0,    27,    33,    17,     8,    15,
      15,     5,    45,    60,     0,     0,     0,    31,     0,     0,
       0,     0,    61,    29,    28,    17,     0,    10,     9,    16,
      17,    14
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Parser::yypgoto_[] =
  {
       -67,   -67,   -67,   -67,   -50,   -55,    -8,   -67,   -67,   -67,
     -67,   -67,   -67,   -67,    -6,    -5,    -4,   -67,   -66,   -67,
      26,   -67,   -67,   -67,   -67,   -67,   -67,    28
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     1,    28,    29,   149,   160,     2,    30,    31,    32,
     133,    91,   134,    33,    34,    35,    36,    54,   106,    37,
      38,    39,    40,    41,    42,    43,    44,    45
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned char
  Parser::yytable_[] =
  {
        50,    47,    48,    49,   110,    64,    67,     3,    65,    66,
      55,    68,   155,   144,    52,   156,    60,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    70,    18,    51,    19,
     105,    72,    46,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    69,    18,    93,    19,   109,    56,    25,    57,
      58,    59,    26,    61,    62,    63,    27,    71,    85,    86,
      87,    88,    89,    67,    25,    90,    72,   131,    26,   132,
      15,    16,    27,   158,   159,    53,    18,    94,   154,    92,
     104,   112,   136,   148,   167,   147,   165,   168,   150,   137,
     138,   139,   101,   102,   103,   161,   107,   108,   107,   111,
      90,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   135,
     141,     0,     0,     0,   146,    47,    48,   145,   140,     0,
       0,     0,   151,     4,     5,     6,     7,   143,     8,   157,
       9,    10,    11,    12,    13,    14,    15,    16,    17,   142,
      18,    72,    19,     0,    20,    21,     0,   169,    22,    23,
      24,     0,   171,   162,     0,     0,   142,     0,     0,     0,
     152,    25,   107,     0,     0,    26,     0,     0,     0,    27,
      87,    88,    89,   163,   164,    90,     0,   166,     4,     5,
       6,     7,   142,     8,    72,     9,    10,    11,    12,    13,
      14,    15,    16,    17,     0,    18,    95,    19,     0,    20,
      21,     0,     0,    22,    23,    24,     0,     0,     0,    83,
      84,    85,    86,    87,    88,    89,    25,     0,    90,     0,
      26,     0,     0,     0,    27,     4,     5,     6,     7,     0,
       8,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,     0,    18,     0,    19,     0,    20,    21,     0,     0,
      22,    23,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,    26,     0,     0,
       0,    27,     4,     5,     6,     7,     0,     8,     0,     9,
      10,    11,    12,    13,    14,    15,    16,    17,     0,    18,
       0,    19,     0,    20,    21,     0,     0,    22,   100,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,    26,     0,     0,     0,    27,     9,
      10,    11,    12,    13,    14,    15,    16,    17,     0,    18,
       0,    19,   153,     9,    10,    11,    12,    13,    14,    15,
      16,    17,     0,    18,     0,    19,     0,     0,     0,     0,
      25,     0,     0,     0,    26,     0,     0,     0,    27,    72,
       0,     0,     0,     0,    25,    96,     0,     0,    26,     0,
       0,     0,    27,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,     0,    83,    84,    85,    86,    87,    88,
      89,    72,     0,    90,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,     0,    83,    84,    85,    86,
      87,    88,    89,    72,     0,    90,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,     0,    83,    84,
      85,    86,    87,    88,    89,    72,     0,    90,     0,     0,
       0,     0,    99,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,     0,
      83,    84,    85,    86,    87,    88,    89,    72,     0,    90,
       0,     0,     0,     0,   170,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    72,    83,    84,    85,    86,    87,    88,    89,     0,
       0,    90,     0,     0,     0,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    72,    83,    84,    85,    86,
      87,    88,    89,     0,     0,    90,     0,     0,     0,    72,
      74,    75,    76,    77,    78,    79,    80,    81,    82,     0,
      83,    84,    85,    86,    87,    88,    89,     0,     0,    90,
      80,    81,    82,     0,    83,    84,    85,    86,    87,    88,
      89,     0,     0,    90
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
         8,     7,     7,     7,    70,    26,    18,     0,    29,    30,
      18,    23,    26,    23,    17,    29,    24,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    23,    21,    18,    23,
      24,    18,     4,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    54,    21,    20,    23,    24,    19,    42,    21,
      22,    23,    46,    25,    26,    27,    50,    54,    45,    46,
      47,    48,    49,    18,    42,    52,    18,     8,    46,    10,
      17,    18,    50,    27,    28,    17,    21,    53,   144,    17,
      17,    17,    17,     3,     3,    25,    25,     3,   138,    97,
      98,    99,    64,    65,    66,   150,    68,    69,    70,    71,
      52,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    93,
      24,    -1,    -1,    -1,   132,   131,   131,   131,   100,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    24,     9,   147,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    53,
      21,    18,    23,    -1,    25,    26,    -1,   165,    29,    30,
      31,    -1,   170,    24,    -1,    -1,    53,    -1,    -1,    -1,
     142,    42,   144,    -1,    -1,    46,    -1,    -1,    -1,    50,
      47,    48,    49,   155,   156,    52,    -1,   159,     4,     5,
       6,     7,    53,     9,    18,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    22,    23,    -1,    25,
      26,    -1,    -1,    29,    30,    31,    -1,    -1,    -1,    43,
      44,    45,    46,    47,    48,    49,    42,    -1,    52,    -1,
      46,    -1,    -1,    -1,    50,     4,     5,     6,     7,    -1,
       9,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    -1,    21,    -1,    23,    -1,    25,    26,    -1,    -1,
      29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,     4,     5,     6,     7,    -1,     9,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    -1,    21,
      -1,    23,    -1,    25,    26,    -1,    -1,    29,    30,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    -1,    21,
      -1,    23,    24,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    -1,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    18,
      -1,    -1,    -1,    -1,    42,    24,    -1,    -1,    46,    -1,
      -1,    -1,    50,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    45,    46,    47,    48,
      49,    18,    -1,    52,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    45,    46,
      47,    48,    49,    18,    -1,    52,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      45,    46,    47,    48,    49,    18,    -1,    52,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    45,    46,    47,    48,    49,    18,    -1,    52,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    18,    43,    44,    45,    46,    47,    48,    49,    -1,
      -1,    52,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    18,    43,    44,    45,    46,
      47,    48,    49,    -1,    -1,    52,    -1,    -1,    -1,    18,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    45,    46,    47,    48,    49,    -1,    -1,    52,
      39,    40,    41,    -1,    43,    44,    45,    46,    47,    48,
      49,    -1,    -1,    52
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,    56,    61,     0,     4,     5,     6,     7,     9,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    21,    23,
      25,    26,    29,    30,    31,    42,    46,    50,    57,    58,
      62,    63,    64,    68,    69,    70,    71,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    82,    69,    70,    71,
      61,    18,    17,    17,    72,    61,    82,    82,    82,    82,
      61,    82,    82,    82,    26,    29,    30,    18,    23,    54,
      23,    54,    18,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    43,    44,    45,    46,    47,    48,    49,
      52,    66,    17,    20,    53,    22,    24,    25,    25,    25,
      30,    82,    82,    82,    17,    24,    73,    82,    82,    24,
      73,    82,    17,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,     8,    10,    65,    67,    75,    17,    61,    61,    61,
      82,    24,    53,    24,    23,    71,    61,    25,     3,    59,
      59,     3,    82,    24,    73,    26,    29,    61,    27,    28,
      60,    60,    24,    82,    82,    25,    82,     3,     3,    61,
      25,    61
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
         0,    55,    56,    57,    57,    57,    57,    57,    57,    58,
      58,    58,    58,    59,    59,    60,    60,    61,    61,    61,
      62,    62,    63,    63,    63,    63,    64,    65,    65,    65,
      66,    66,    67,    67,    68,    68,    69,    69,    69,    70,
      71,    71,    72,    72,    73,    73,    74,    74,    75,    76,
      76,    76,    76,    76,    76,    77,    77,    77,    77,    77,
      77,    77,    78,    78,    79,    79,    79,    79,    79,    79,
      79,    80,    80,    80,    80,    80,    80,    80,    80,    81,
      81,    81,    81,    81,    81,    82,    82,    82,    82,    82,
      82,    82,    82,    82
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     1,     1,     5,     3,     4,     5,     7,
       7,     3,     3,     0,     5,     0,     3,     0,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     4,     4,
       0,     4,     0,     2,     1,     2,     2,     3,     3,     1,
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
  "command", "throw_cmd", "catch_stmt", "catch_sqnc", "finally_stmt",
  "return_cmd", "scoped_var", "local_var", "variable", "parameters",
  "arguments", "closure", "scope", "literal", "function_call",
  "assignment", "mathematical_operation", "logical_operation",
  "bitwise_operation", "expression", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        56,     0,    -1,    61,    -1,    62,    -1,    58,    -1,    30,
      82,    25,    61,     3,    -1,    62,    30,    82,    -1,    31,
      61,    30,    82,    -1,     9,    61,    66,    67,     3,    -1,
      29,    82,    25,    61,    59,    60,     3,    -1,    26,    82,
      25,    61,    59,    60,     3,    -1,    62,    29,    82,    -1,
      62,    26,    82,    -1,    -1,    59,    28,    82,    25,    61,
      -1,    -1,    27,    25,    61,    -1,    -1,    61,    25,    -1,
      61,    57,    -1,    82,    -1,    63,    -1,    68,    -1,    64,
      -1,     5,    -1,     6,    -1,     7,    71,    -1,     8,    71,
      -1,     8,    71,    29,    82,    -1,     8,    71,    26,    82,
      -1,    -1,    66,    65,    25,    61,    -1,    -1,    10,    61,
      -1,     4,    -1,     4,    82,    -1,    18,    17,    -1,    17,
      18,    17,    -1,    69,    18,    17,    -1,    17,    -1,    69,
      -1,    70,    -1,    17,    -1,    72,    53,    17,    -1,    82,
      -1,    73,    53,    82,    -1,    19,    72,    20,    75,    -1,
      75,    -1,    21,    61,    22,    -1,    11,    -1,    12,    -1,
      13,    -1,    14,    -1,    15,    -1,    16,    -1,    69,    23,
      24,    -1,    70,    23,    24,    -1,    69,    23,    73,    24,
      -1,    70,    23,    73,    24,    -1,    82,    18,    17,    -1,
      82,    18,    17,    23,    24,    -1,    82,    18,    17,    23,
      73,    24,    -1,    70,    54,    82,    -1,    69,    54,    82,
      -1,    82,    45,    82,    -1,    82,    46,    82,    -1,    82,
      47,    82,    -1,    82,    48,    82,    -1,    46,    82,    -1,
      82,    49,    82,    -1,    82,    52,    82,    -1,    82,    32,
      82,    -1,    82,    33,    82,    -1,    82,    38,    82,    -1,
      82,    34,    82,    -1,    82,    37,    82,    -1,    82,    36,
      82,    -1,    82,    35,    82,    -1,    50,    82,    -1,    82,
      44,    82,    -1,    82,    43,    82,    -1,    82,    39,    82,
      -1,    82,    40,    82,    -1,    82,    41,    82,    -1,    42,
      82,    -1,    76,    -1,    74,    -1,    71,    -1,    77,    -1,
      78,    -1,    79,    -1,    80,    -1,    81,    -1,    23,    82,
      24,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     7,     9,    15,    19,    24,    30,
      38,    46,    50,    54,    55,    61,    62,    66,    67,    70,
      73,    75,    77,    79,    81,    83,    85,    88,    91,    96,
     101,   102,   107,   108,   111,   113,   116,   119,   123,   127,
     129,   131,   133,   135,   139,   141,   145,   150,   152,   156,
     158,   160,   162,   164,   166,   168,   172,   176,   181,   186,
     190,   196,   203,   207,   211,   215,   219,   223,   227,   230,
     234,   238,   242,   246,   250,   254,   258,   262,   266,   269,
     273,   277,   281,   285,   289,   292,   294,   296,   298,   300,
     302,   304,   306,   308
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  Parser::yyrline_[] =
  {
         0,    80,    80,    83,    84,    85,    86,    87,    88,    91,
      92,    93,    94,    97,    98,   101,   102,   105,   106,   107,
     110,   111,   114,   115,   116,   117,   120,   123,   124,   125,
     128,   129,   132,   133,   136,   137,   140,   141,   142,   145,
     148,   149,   152,   153,   156,   157,   160,   164,   167,   170,
     171,   172,   173,   174,   175,   178,   179,   180,   181,   182,
     183,   184,   187,   188,   191,   193,   195,   197,   199,   202,
     204,   208,   210,   212,   214,   216,   218,   220,   222,   225,
     227,   229,   231,   233,   235,   238,   239,   240,   241,   242,
     243,   244,   245,   246
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
  const int Parser::yylast_ = 593;
  const int Parser::yynnts_ = 28;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 3;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 55;

  const unsigned int Parser::yyuser_token_number_max_ = 288;
  const Parser::token_number_type Parser::yyundef_token_ = 2;

} // namespace snow

#line 249 "Parser.yy"


void snow::Parser::error(const Parser::location_type& l, const std::string& m)
{
    driver.error(l, m);
}

int main() {
    snow::Driver driver = snow::Driver();
    bool result = driver.parse_stream(std::cin);
    return result;
}
