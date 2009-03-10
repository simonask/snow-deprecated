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
#line 60 "Parser.yy"


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
    #line 19 "Parser.yy"
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
	  case 37:
#line 133 "Parser.yy"
    { (yyval.identifier) = (yysemantic_stack_[(2) - (2)].identifier); ;}
    break;

  case 38:
#line 134 "Parser.yy"
    { (yyval.identifier) = (yysemantic_stack_[(3) - (3)].identifier); ;}
    break;

  case 39:
#line 137 "Parser.yy"
    { (yyval.identifier) = (yysemantic_stack_[(1) - (1)].identifier); ;}
    break;

  case 40:
#line 138 "Parser.yy"
    { (yyval.identifier) = (yysemantic_stack_[(3) - (3)].identifier); ;}
    break;

  case 41:
#line 141 "Parser.yy"
    { (yyval.identifier) = (yysemantic_stack_[(1) - (1)].identifier); ;}
    break;

  case 42:
#line 142 "Parser.yy"
    { (yyval.identifier) = (yysemantic_stack_[(1) - (1)].identifier); ;}
    break;

  case 43:
#line 145 "Parser.yy"
    { (yyval.identifier) = (yysemantic_stack_[(3) - (1)].identifier); ;}
    break;

  case 44:
#line 146 "Parser.yy"
    { (yyval.identifier) = (yysemantic_stack_[(3) - (1)].identifier); ;}
    break;

  case 53:
#line 164 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 54:
#line 165 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 55:
#line 166 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 56:
#line 167 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 57:
#line 168 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 58:
#line 169 "Parser.yy"
    { (yyval.literal) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 59:
#line 172 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(3) - (1)].identifier); ;}
    break;

  case 60:
#line 173 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(4) - (1)].identifier); ;}
    break;

  case 64:
#line 179 "Parser.yy"
    { (yyval.assignment) = new ast::Assignment((yysemantic_stack_[(3) - (1)].identifier), (yysemantic_stack_[(3) - (3)].node)); ;}
    break;

  case 65:
#line 180 "Parser.yy"
    { (yyval.assignment) = new ast::Assignment((yysemantic_stack_[(3) - (1)].identifier), (yysemantic_stack_[(3) - (3)].node)); ;}
    break;

  case 87:
#line 210 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].literal); ;}
    break;

  case 88:
#line 211 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 89:
#line 212 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].identifier); ;}
    break;

  case 90:
#line 213 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 91:
#line 214 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].assignment); ;}
    break;

  case 92:
#line 215 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 93:
#line 216 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 94:
#line 217 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 95:
#line 218 "Parser.yy"
    { (yyval.node) = (yysemantic_stack_[(3) - (2)].node); ;}
    break;


    /* Line 675 of lalr1.cc.  */
#line 542 "Parser.cpp"
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
  const signed char Parser::yypact_ninf_ = -56;
  const short int
  Parser::yypact_[] =
  {
       -56,     2,   232,   -56,    24,   -56,   -56,    50,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,    -4,    -2,   -56,    24,
     -56,    24,    24,    24,   -56,    24,    24,    24,   -56,   -56,
     -22,   -56,   -56,   -56,    -1,    40,     6,    18,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   490,    18,   490,   -56,
     232,   -56,   -56,    -9,   185,   338,   370,   402,   434,   279,
     177,     9,     9,    24,    24,    24,    45,    47,     7,    50,
      24,    50,    24,    52,    24,    24,    24,    24,    24,    24,
      24,    24,    24,    24,    24,    24,    24,    24,    24,    24,
      24,    24,     4,    60,    56,   -56,   -56,   -56,   -56,   -56,
      24,   490,   490,   490,   -56,   -56,   -56,   -21,   490,   -56,
     490,   -56,   490,    59,   514,   305,   305,   305,   305,   305,
     305,   177,   177,   177,   224,   224,    28,    28,     9,     9,
       9,     9,    50,   -56,    58,    81,   -56,   -56,   232,   232,
     129,   490,   -56,    24,   319,   -20,   232,   -56,   -56,    51,
      51,   -56,   490,   -56,   -19,    24,    24,   232,    61,    24,
      82,    84,   -56,   490,   490,   -56,   466,   -56,   -56,   232,
     -56,   232
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
        17,     0,     2,     1,    34,    24,    25,     0,    17,    53,
      54,    55,    56,    57,    58,    39,     0,    45,    17,     0,
      18,     0,     0,     0,    17,     0,     0,     0,    19,     4,
       3,    21,    23,    22,    41,    42,    89,     0,    88,    51,
      87,    90,    91,    92,    93,    94,    20,    36,    35,    26,
      30,    37,    46,     0,     0,     0,     0,     0,     0,     0,
      86,    70,    80,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    32,     0,     0,    52,    95,    17,    17,    17,
       0,    12,    11,     6,    38,    40,    59,     0,    48,    43,
      64,    44,    65,    61,    73,    74,    76,    79,    78,    77,
      75,    83,    84,    85,    82,    81,    66,    67,    68,    69,
      71,    72,     0,    17,     0,     0,    50,    47,    13,    13,
       0,     7,    60,     0,     0,    27,    33,    17,     8,    15,
      15,     5,    49,    62,     0,     0,     0,    31,     0,     0,
       0,     0,    63,    29,    28,    17,     0,    10,     9,    16,
      17,    14
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Parser::yypgoto_[] =
  {
       -56,   -56,   -56,   -56,   -51,   -55,    -8,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,    -6,    92,   -56,   -46,
     -56,    30,   -56,   -56,   -56,   -56,   -56,   -56,    29
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     1,    28,    29,   149,   160,     2,    30,    31,    32,
     134,    92,   135,    33,    34,    35,    36,    37,    53,   107,
      38,    39,    40,    41,    42,    43,    44,    45,    46
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned char
  Parser::yytable_[] =
  {
        50,    49,     3,   142,    63,   162,   155,    64,    65,   156,
      54,    93,   132,    51,   133,    52,    59,    66,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    73,    18,    68,
      19,   106,   143,    48,   143,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    94,    18,    73,    19,    55,    25,
      56,    57,    58,    26,    60,    61,    62,    27,    67,    69,
      70,    91,   104,   109,   105,   111,    25,    15,    16,   113,
      26,    71,    72,   137,    27,    88,    89,    90,   158,   159,
      91,    18,   144,   147,   148,   167,   165,   168,   150,   138,
     139,   140,   101,   102,   103,   161,    47,   108,   154,   110,
       0,   112,     0,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,     0,   136,     0,   146,   145,     0,     0,   141,
       0,     0,   151,     4,     5,     6,     7,     0,     8,   157,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     0,
      18,     0,    19,     0,    20,    21,     0,   169,    22,    23,
      24,     0,   171,     0,     0,     0,     0,     0,     0,     0,
       0,    25,   152,   108,     0,    26,     0,     0,     0,    27,
       0,     0,     0,     0,   163,   164,     0,     0,   166,     4,
       5,     6,     7,     0,     8,    73,     9,    10,    11,    12,
      13,    14,    15,    16,    17,     0,    18,    95,    19,     0,
      20,    21,     0,     0,    22,    23,    24,     0,     0,     0,
      84,    85,    86,    87,    88,    89,    90,    25,     0,    91,
       0,    26,     0,     0,     0,    27,     4,     5,     6,     7,
       0,     8,    73,     9,    10,    11,    12,    13,    14,    15,
      16,    17,     0,    18,     0,    19,     0,    20,    21,     0,
       0,    22,    23,    24,     0,     0,     0,     0,     0,    86,
      87,    88,    89,    90,    25,     0,    91,     0,    26,     0,
       0,     0,    27,     4,     5,     6,     7,     0,     8,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     0,
      18,     0,    19,     0,    20,    21,     0,     0,    22,   100,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    25,     0,    73,     0,    26,     0,     0,     0,    27,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     0,
      18,     0,    19,   153,    81,    82,    83,     0,    84,    85,
      86,    87,    88,    89,    90,     0,    73,    91,     0,     0,
       0,    25,    96,     0,     0,    26,     0,     0,     0,    27,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
       0,    84,    85,    86,    87,    88,    89,    90,    73,     0,
      91,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,     0,    84,    85,    86,    87,    88,    89,    90,
      73,     0,    91,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,     0,    84,    85,    86,    87,    88,
      89,    90,    73,     0,    91,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,     0,    84,    85,    86,
      87,    88,    89,    90,    73,     0,    91,     0,     0,     0,
       0,   170,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    73,    84,
      85,    86,    87,    88,    89,    90,     0,     0,    91,     0,
       0,     0,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    73,    84,    85,    86,    87,    88,    89,    90,
       0,     0,    91,     0,     0,     0,     0,    75,    76,    77,
      78,    79,    80,    81,    82,    83,     0,    84,    85,    86,
      87,    88,    89,    90,     0,     0,    91
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
         8,     7,     0,    24,    26,    24,    26,    29,    30,    29,
      18,    20,     8,    17,    10,    17,    24,    18,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    18,    21,    23,
      23,    24,    53,     4,    53,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    53,    21,    18,    23,    19,    42,
      21,    22,    23,    46,    25,    26,    27,    50,    18,    53,
      54,    52,    17,    69,    17,    71,    42,    17,    18,    17,
      46,    53,    54,    17,    50,    47,    48,    49,    27,    28,
      52,    21,    23,    25,     3,     3,    25,     3,   139,    97,
      98,    99,    63,    64,    65,   150,     4,    68,   144,    70,
      -1,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    -1,    93,    -1,   133,   132,    -1,    -1,   100,
      -1,    -1,     3,     4,     5,     6,     7,    -1,     9,   147,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      21,    -1,    23,    -1,    25,    26,    -1,   165,    29,    30,
      31,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,   143,   144,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,   155,   156,    -1,    -1,   159,     4,
       5,     6,     7,    -1,     9,    18,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    -1,    21,    22,    23,    -1,
      25,    26,    -1,    -1,    29,    30,    31,    -1,    -1,    -1,
      43,    44,    45,    46,    47,    48,    49,    42,    -1,    52,
      -1,    46,    -1,    -1,    -1,    50,     4,     5,     6,     7,
      -1,     9,    18,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    -1,    25,    26,    -1,
      -1,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    45,
      46,    47,    48,    49,    42,    -1,    52,    -1,    46,    -1,
      -1,    -1,    50,     4,     5,     6,     7,    -1,     9,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      21,    -1,    23,    -1,    25,    26,    -1,    -1,    29,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    18,    -1,    46,    -1,    -1,    -1,    50,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      21,    -1,    23,    24,    39,    40,    41,    -1,    43,    44,
      45,    46,    47,    48,    49,    -1,    18,    52,    -1,    -1,
      -1,    42,    24,    -1,    -1,    46,    -1,    -1,    -1,    50,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    45,    46,    47,    48,    49,    18,    -1,
      52,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    45,    46,    47,    48,    49,
      18,    -1,    52,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    45,    46,    47,
      48,    49,    18,    -1,    52,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    18,    -1,    52,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    18,    43,
      44,    45,    46,    47,    48,    49,    -1,    -1,    52,    -1,
      -1,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    18,    43,    44,    45,    46,    47,    48,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    -1,    -1,    52
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,    56,    61,     0,     4,     5,     6,     7,     9,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    21,    23,
      25,    26,    29,    30,    31,    42,    46,    50,    57,    58,
      62,    63,    64,    68,    69,    70,    71,    72,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    72,    83,    71,
      61,    17,    17,    73,    61,    83,    83,    83,    83,    61,
      83,    83,    83,    26,    29,    30,    18,    18,    23,    53,
      54,    53,    54,    18,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    43,    44,    45,    46,    47,    48,
      49,    52,    66,    20,    53,    22,    24,    25,    25,    25,
      30,    83,    83,    83,    17,    17,    24,    74,    83,    71,
      83,    71,    83,    17,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,     8,    10,    65,    67,    76,    17,    61,    61,
      61,    83,    24,    53,    23,    71,    61,    25,     3,    59,
      59,     3,    83,    24,    74,    26,    29,    61,    27,    28,
      60,    60,    24,    83,    83,    25,    83,     3,     3,    61,
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
      66,    66,    67,    67,    68,    68,    68,    69,    69,    70,
      70,    71,    71,    72,    72,    73,    73,    73,    74,    74,
      75,    75,    76,    77,    77,    77,    77,    77,    77,    78,
      78,    78,    78,    78,    79,    79,    80,    80,    80,    80,
      80,    80,    80,    81,    81,    81,    81,    81,    81,    81,
      81,    82,    82,    82,    82,    82,    82,    83,    83,    83,
      83,    83,    83,    83,    83,    83
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     1,     1,     5,     3,     4,     5,     7,
       7,     3,     3,     0,     5,     0,     3,     0,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     4,     4,
       0,     4,     0,     2,     1,     2,     2,     2,     3,     1,
       3,     1,     1,     3,     3,     0,     1,     3,     1,     3,
       4,     1,     3,     1,     1,     1,     1,     1,     1,     3,
       4,     3,     5,     6,     3,     3,     3,     3,     3,     3,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     3,     3,     3,     3,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     3
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
  "return_cmd", "instance_var", "local_var", "variable", "variables",
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
      83,    25,    61,     3,    -1,    62,    30,    83,    -1,    31,
      61,    30,    83,    -1,     9,    61,    66,    67,     3,    -1,
      29,    83,    25,    61,    59,    60,     3,    -1,    26,    83,
      25,    61,    59,    60,     3,    -1,    62,    29,    83,    -1,
      62,    26,    83,    -1,    -1,    59,    28,    83,    25,    61,
      -1,    -1,    27,    25,    61,    -1,    -1,    61,    25,    -1,
      61,    57,    -1,    83,    -1,    63,    -1,    68,    -1,    64,
      -1,     5,    -1,     6,    -1,     7,    71,    -1,     8,    71,
      -1,     8,    71,    29,    83,    -1,     8,    71,    26,    83,
      -1,    -1,    66,    65,    25,    61,    -1,    -1,    10,    61,
      -1,     4,    -1,     4,    83,    -1,     4,    72,    -1,    18,
      17,    -1,    69,    18,    17,    -1,    17,    -1,    70,    18,
      17,    -1,    69,    -1,    70,    -1,    71,    53,    71,    -1,
      72,    53,    71,    -1,    -1,    17,    -1,    73,    53,    17,
      -1,    83,    -1,    74,    53,    83,    -1,    19,    73,    20,
      76,    -1,    76,    -1,    21,    61,    22,    -1,    11,    -1,
      12,    -1,    13,    -1,    14,    -1,    15,    -1,    16,    -1,
      71,    23,    24,    -1,    71,    23,    74,    24,    -1,    83,
      18,    17,    -1,    83,    18,    17,    23,    24,    -1,    83,
      18,    17,    23,    74,    24,    -1,    71,    54,    83,    -1,
      72,    54,    83,    -1,    83,    45,    83,    -1,    83,    46,
      83,    -1,    83,    47,    83,    -1,    83,    48,    83,    -1,
      46,    83,    -1,    83,    49,    83,    -1,    83,    52,    83,
      -1,    83,    32,    83,    -1,    83,    33,    83,    -1,    83,
      38,    83,    -1,    83,    34,    83,    -1,    83,    37,    83,
      -1,    83,    36,    83,    -1,    83,    35,    83,    -1,    50,
      83,    -1,    83,    44,    83,    -1,    83,    43,    83,    -1,
      83,    39,    83,    -1,    83,    40,    83,    -1,    83,    41,
      83,    -1,    42,    83,    -1,    77,    -1,    75,    -1,    71,
      -1,    78,    -1,    79,    -1,    80,    -1,    81,    -1,    82,
      -1,    23,    83,    24,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     7,     9,    15,    19,    24,    30,
      38,    46,    50,    54,    55,    61,    62,    66,    67,    70,
      73,    75,    77,    79,    81,    83,    85,    88,    91,    96,
     101,   102,   107,   108,   111,   113,   116,   119,   122,   126,
     128,   132,   134,   136,   140,   144,   145,   147,   151,   153,
     157,   162,   164,   168,   170,   172,   174,   176,   178,   180,
     184,   189,   193,   199,   206,   210,   214,   218,   222,   226,
     230,   233,   237,   241,   245,   249,   253,   257,   261,   265,
     269,   272,   276,   280,   284,   288,   292,   295,   297,   299,
     301,   303,   305,   307,   309,   311
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  Parser::yyrline_[] =
  {
         0,    72,    72,    75,    76,    77,    78,    79,    80,    83,
      84,    85,    86,    89,    90,    93,    94,    97,    98,    99,
     102,   103,   106,   107,   108,   109,   112,   115,   116,   117,
     120,   121,   124,   125,   128,   129,   130,   133,   134,   137,
     138,   141,   142,   145,   146,   148,   149,   150,   153,   154,
     157,   158,   161,   164,   165,   166,   167,   168,   169,   172,
     173,   174,   175,   176,   179,   180,   183,   184,   185,   186,
     187,   188,   189,   192,   193,   194,   195,   196,   197,   198,
     199,   202,   203,   204,   205,   206,   207,   210,   211,   212,
     213,   214,   215,   216,   217,   218
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
  const int Parser::yylast_ = 566;
  const int Parser::yynnts_ = 29;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 3;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 55;

  const unsigned int Parser::yyuser_token_number_max_ = 288;
  const Parser::token_number_type Parser::yyundef_token_ = 2;

} // namespace snow

#line 221 "Parser.yy"


void snow::Parser::error(const Parser::location_type& l, const std::string& m)
{
    driver.error(l, m);
}

int main() {
    snow::Driver driver = snow::Driver();
    bool result = driver.parse_stream(std::cin);
    return result;
}
