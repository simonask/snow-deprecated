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

#include "parser.h"

/* User implementation prologue.  */
#line 47 "parser.yy"


#include "driver.h"
#include "scanner.h"

#undef yylex
#define yylex driver.lexer->lex



/* Line 317 of lalr1.cc.  */
#line 54 "parser.cpp"

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
  Parser::Parser (class Driver& driver_yyarg)
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
    #line 16 "parser.yy"
{
    yylloc.begin.filename = yylloc.end.filename = &driver.streamname;
}
  /* Line 547 of yacc.c.  */
#line 290 "parser.cpp"
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
	
    /* Line 675 of lalr1.cc.  */
#line 407 "parser.cpp"
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
  const signed char Parser::yypact_ninf_ = -46;
  const short int
  Parser::yypact_[] =
  {
       -46,    20,   276,   -46,   -46,   -46,   -46,   -46,   -46,   -46,
     -46,   348,   -46,   -46,    -6,   -46,   -46,   348,   348,   348,
     -46,   348,   348,   348,    15,    19,   -46,   348,   -46,   -46,
      24,   -46,   -46,   -46,   -14,    -1,   -45,   -28,   -46,   -46,
     -46,   500,   -28,   500,   -46,   276,   377,   402,   427,   326,
      70,    18,    18,   -46,   -46,   -41,   176,   449,   348,   348,
     348,    47,    49,    -6,     9,   348,    -6,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,    51,    34,   -46,   -46,
     -46,   348,    58,     4,   -46,   -46,   500,   500,   500,   -46,
     -46,   -46,   -46,   -43,   500,   500,   -46,   500,   521,   536,
     536,   536,   536,   536,   536,    70,    70,    70,    79,    79,
      -4,    -4,    18,    18,    18,    18,    16,    -6,   -46,    60,
      59,   276,   276,   226,   500,   -46,   -46,   348,   -46,   125,
       3,   276,   -46,   -46,    45,    45,   -46,   500,   -46,   -26,
     348,   348,   276,    80,   348,    89,    90,   -46,   500,   500,
     -46,   478,   -46,   -46,   276,   -46,   276
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
        17,     0,     2,     1,    53,    54,    55,    56,    57,    58,
      39,    34,    24,    25,     0,    17,    18,     0,     0,     0,
      17,     0,     0,     0,     0,    45,    17,     0,    19,     4,
       3,    21,    23,    22,    41,    42,    61,     0,    60,    51,
      59,    20,    36,    35,    26,    30,     0,     0,     0,     0,
      88,    73,    89,    37,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    17,    17,
      17,     0,     0,     0,    52,    90,    12,    11,     6,    38,
      40,    43,    62,     0,    48,    67,    44,    68,    76,    77,
      79,    82,    81,    80,    78,    85,    86,    87,    84,    83,
      69,    70,    71,    72,    74,    75,    64,     0,    17,     0,
       0,    13,    13,     0,     7,    47,    50,     0,    63,     0,
      27,    33,    17,     8,    15,    15,     5,    49,    65,     0,
       0,     0,    31,     0,     0,     0,     0,    66,    29,    28,
      17,     0,    10,     9,    16,    17,    14
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Parser::yypgoto_[] =
  {
       -46,   -46,   -46,   -46,   -31,   -40,   -15,   -46,   -46,   -46,
     -46,   -46,   -46,   -46,   -46,   -46,   -13,    91,   -46,   -35,
     -46,    29,   -46,    12
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     1,    28,    29,   144,   155,     2,    30,    31,    32,
     129,    87,   130,    33,    34,    35,    36,    37,    55,   103,
      38,    39,    40,    41
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned char
  Parser::yytable_[] =
  {
        45,    44,    63,    10,   137,    49,    92,    64,    93,    65,
     138,    56,     4,     5,     6,     7,     8,     9,    10,    66,
       3,   137,   150,    43,    53,   151,    67,   157,    54,    46,
      47,    48,    61,    50,    51,    52,    82,    83,    84,    57,
      24,    85,    86,    58,    21,    62,    59,    60,    22,   127,
     101,   128,    23,   106,    26,    24,    99,    25,   100,    26,
     126,    27,   102,    85,    86,   153,   154,   135,   139,   143,
      96,    97,    98,   131,   132,   133,   104,   105,   142,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   160,   162,
     163,   145,    42,   134,   149,   156,    78,    79,    80,    81,
      82,    83,    84,   141,   140,    85,    86,    80,    81,    82,
      83,    84,   136,     0,    85,    86,     0,   152,     4,     5,
       6,     7,     8,     9,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,     0,     0,     0,   147,
     166,   104,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,   158,   159,    22,     0,   161,     0,    23,     0,
       0,    24,     0,    25,     0,    26,     0,    27,   148,     4,
       5,     6,     7,     8,     9,    10,     0,    11,    12,    13,
      14,     0,    15,     0,    16,    17,     0,     0,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,    22,     0,     0,     0,    23,
       0,     0,    24,     0,    25,     0,    26,    94,    27,     4,
       5,     6,     7,     8,     9,    10,   146,    11,    12,    13,
      14,     0,    15,     0,    16,    17,     0,     0,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,    22,     0,     0,     0,    23,
       0,     0,    24,     0,    25,     0,    26,     0,    27,     4,
       5,     6,     7,     8,     9,    10,     0,    11,    12,    13,
      14,     0,    15,     0,    16,    17,     0,     0,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,    22,     0,     0,     0,    23,
       0,     0,    24,     0,    25,     0,    26,     0,    27,     4,
       5,     6,     7,     8,     9,    10,     0,    11,    12,    13,
      14,     0,    15,     0,    16,    17,     0,     0,    18,    91,
      20,     4,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    21,     0,     0,     0,    22,     0,     0,     0,    23,
       0,     0,    24,     0,    25,     0,    26,     0,    27,     0,
       0,     0,     0,    21,     0,     0,     0,    22,     0,     0,
       0,    23,     0,     0,    24,    88,    25,     0,    26,     0,
      27,     0,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,     0,    78,    79,    80,    81,    82,    83,    84,
      89,     0,    85,    86,     0,     0,     0,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
      80,    81,    82,    83,    84,    90,     0,    85,    86,     0,
       0,     0,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,     0,    78,    79,    80,    81,    82,    83,    84,
       0,     0,    85,    86,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,    80,    81,    82,
      83,    84,     0,     0,    85,    86,   165,     0,     0,     0,
       0,     0,    95,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,    80,    81,    82,    83,
      84,     0,     0,    85,    86,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,    80,    81,
      82,    83,    84,     0,     0,    85,    86,    69,    70,    71,
      72,    73,    74,    75,    76,    77,     0,    78,    79,    80,
      81,    82,    83,    84,     0,     0,    85,    86,    75,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,     0,
       0,    85,    86
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
        15,    14,    47,     9,    47,    20,    47,    52,    49,    54,
      53,    26,     3,     4,     5,     6,     7,     8,     9,    47,
       0,    47,    19,    11,     9,    22,    54,    53,     9,    17,
      18,    19,    46,    21,    22,    23,    40,    41,    42,    27,
      46,    45,    46,    19,    35,    46,    22,    23,    39,    15,
      63,    17,    43,    66,    50,    46,     9,    48,     9,    50,
       9,    52,    53,    45,    46,    20,    21,     9,    52,    10,
      58,    59,    60,    88,    89,    90,    64,    65,    18,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    18,    10,
      10,   132,    11,    91,   139,   145,    36,    37,    38,    39,
      40,    41,    42,   128,   127,    45,    46,    38,    39,    40,
      41,    42,    93,    -1,    45,    46,    -1,   142,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,   137,
     165,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,   150,   151,    39,    -1,   154,    -1,    43,    -1,
      -1,    46,    -1,    48,    -1,    50,    -1,    52,    53,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    12,    13,
      14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    48,    -1,    50,    51,    52,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    48,    -1,    50,    -1,    52,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    12,    13,
      14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    48,    -1,    50,    -1,    52,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    12,    13,
      14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,
      24,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    48,    -1,    50,    -1,    52,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    43,    -1,    -1,    46,    18,    48,    -1,    50,    -1,
      52,    -1,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    42,
      18,    -1,    45,    46,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    36,    37,
      38,    39,    40,    41,    42,    18,    -1,    45,    46,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    42,
      -1,    -1,    45,    46,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    -1,    -1,    45,    46,    18,    -1,    -1,    -1,
      -1,    -1,    53,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    -1,    -1,    45,    46,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    -1,    -1,    45,    46,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    -1,    -1,    45,    46,    32,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    -1,
      -1,    45,    46
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,    56,    61,     0,     3,     4,     5,     6,     7,     8,
       9,    11,    12,    13,    14,    16,    18,    19,    22,    23,
      24,    35,    39,    43,    46,    48,    50,    52,    57,    58,
      62,    63,    64,    68,    69,    70,    71,    72,    75,    76,
      77,    78,    72,    78,    71,    61,    78,    78,    78,    61,
      78,    78,    78,     9,     9,    73,    61,    78,    19,    22,
      23,    46,    46,    47,    52,    54,    47,    54,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    36,    37,
      38,    39,    40,    41,    42,    45,    46,    66,    18,    18,
      18,    23,    47,    49,    51,    53,    78,    78,    78,     9,
       9,    71,    53,    74,    78,    78,    71,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,     9,    15,    17,    65,
      67,    61,    61,    61,    78,     9,    76,    47,    53,    52,
      71,    61,    18,    10,    59,    59,    10,    78,    53,    74,
      19,    22,    61,    20,    21,    60,    60,    53,    78,    78,
      18,    78,    10,    10,    61,    18,    61
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,    61,    62,    60,   280,   281,
     282,   283,   124,    38,    94,   126,   284,   285,    43,    45,
      42,    47,    37,   286,   287,   288,    46,    44,    91,    93,
     123,   125,    40,    41,    58
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
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78
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
       4,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     3,     5,     6,     3,     3,     3,
       3,     3,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       3
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Parser::yytname_[] =
  {
    "END_FILE", "error", "$undefined", "INTEGER", "FLOAT", "STRING", "TRUE",
  "FALSE", "NIL", "IDENTIFIER", "END", "RETURN", "BREAK", "CONTINUE",
  "THROW", "CATCH", "TRY", "FINALLY", "EOL", "UNLESS", "ELSE", "ELSIF",
  "IF", "WHILE", "DO", "'='", "'>'", "'<'", "LOG_OR", "LOG_AND", "LTE",
  "GTE", "'|'", "'&'", "'^'", "'~'", "RSHFT", "LSHFT", "'+'", "'-'", "'*'",
  "'/'", "'%'", "LOG_NOT", "NEG", "POW", "'.'", "','", "'['", "']'", "'{'",
  "'}'", "'('", "')'", "':'", "$accept", "program", "statement",
  "conditional", "elsif_cond", "else_cond", "sequence", "function",
  "command", "throw_cmd", "catch_stmt", "catch_sqnc", "finally_stmt",
  "return_cmd", "instance_var", "local_var", "variable", "variables",
  "parameters", "arguments", "closure", "scope", "literal", "expression", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        56,     0,    -1,    61,    -1,    62,    -1,    58,    -1,    23,
      78,    18,    61,    10,    -1,    62,    23,    78,    -1,    24,
      61,    23,    78,    -1,    16,    61,    66,    67,    10,    -1,
      22,    78,    18,    61,    59,    60,    10,    -1,    19,    78,
      18,    61,    59,    60,    10,    -1,    62,    22,    78,    -1,
      62,    19,    78,    -1,    -1,    59,    21,    78,    18,    61,
      -1,    -1,    20,    18,    61,    -1,    -1,    61,    18,    -1,
      61,    57,    -1,    78,    -1,    63,    -1,    68,    -1,    64,
      -1,    12,    -1,    13,    -1,    14,    71,    -1,    15,    71,
      -1,    15,    71,    22,    78,    -1,    15,    71,    19,    78,
      -1,    -1,    66,    65,    18,    61,    -1,    -1,    17,    61,
      -1,    11,    -1,    11,    78,    -1,    11,    72,    -1,    46,
       9,    -1,    69,    46,     9,    -1,     9,    -1,    70,    46,
       9,    -1,    69,    -1,    70,    -1,    71,    47,    71,    -1,
      72,    47,    71,    -1,    -1,     9,    -1,    73,    47,     9,
      -1,    78,    -1,    74,    47,    78,    -1,    48,    73,    49,
      76,    -1,    76,    -1,    50,    61,    51,    -1,     3,    -1,
       4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,
      77,    -1,    75,    -1,    71,    -1,    71,    52,    53,    -1,
      71,    52,    74,    53,    -1,    78,    46,     9,    -1,    78,
      46,     9,    52,    53,    -1,    78,    46,     9,    52,    74,
      53,    -1,    71,    54,    78,    -1,    72,    54,    78,    -1,
      78,    38,    78,    -1,    78,    39,    78,    -1,    78,    40,
      78,    -1,    78,    41,    78,    -1,    39,    78,    -1,    78,
      42,    78,    -1,    78,    45,    78,    -1,    78,    25,    78,
      -1,    78,    26,    78,    -1,    78,    31,    78,    -1,    78,
      27,    78,    -1,    78,    30,    78,    -1,    78,    29,    78,
      -1,    78,    28,    78,    -1,    78,    37,    78,    -1,    78,
      36,    78,    -1,    78,    32,    78,    -1,    78,    33,    78,
      -1,    78,    34,    78,    -1,    35,    78,    -1,    43,    78,
      -1,    52,    78,    53,    -1
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
     182,   184,   186,   190,   195,   199,   205,   212,   216,   220,
     224,   228,   232,   236,   239,   243,   247,   251,   255,   259,
     263,   267,   271,   275,   279,   283,   287,   291,   295,   298,
     301
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  Parser::yyrline_[] =
  {
         0,    59,    59,    62,    63,    64,    65,    66,    67,    70,
      71,    72,    73,    76,    77,    80,    81,    84,    85,    86,
      89,    90,    93,    94,    95,    96,    99,   102,   103,   104,
     107,   108,   111,   112,   115,   116,   117,   120,   121,   124,
     125,   128,   129,   132,   133,   135,   136,   137,   140,   141,
     144,   145,   148,   151,   152,   153,   154,   155,   156,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190
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
       2,     2,     2,     2,     2,     2,     2,    42,    33,     2,
      52,    53,    40,    38,    47,    39,    46,    41,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    54,     2,
      27,    25,    26,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    48,     2,    49,    34,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    50,    32,    51,    35,     2,     2,     2,
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
      28,    29,    30,    31,    36,    37,    43,    44,    45
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 582;
  const int Parser::yynnts_ = 24;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 3;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 55;

  const unsigned int Parser::yyuser_token_number_max_ = 288;
  const Parser::token_number_type Parser::yyundef_token_ = 2;

} // namespace snow

#line 193 "parser.yy"


void snow::Parser::error(const Parser::location_type& l, const std::string& m)
{
    driver.error(l, m);
}

int main() {
    snow::Driver driver = snow::Driver();
    bool result = driver.parse_stream(std::cin);
    
    std::string dispres;
    if (result) { dispres = "No errors."; } else { dispres = "Error!"; }
    std::cout << "Result: " << dispres << std::endl;
}
