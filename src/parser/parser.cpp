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
#line 48 "Parser.yy"


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
    #line 17 "Parser.yy"
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
	
    /* Line 675 of lalr1.cc.  */
#line 407 "Parser.cpp"
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
  const signed char Parser::yypact_ninf_ = -30;
  const short int
  Parser::yypact_[] =
  {
       -30,     4,   222,   -30,   -30,   -30,   -30,   -30,   -30,   -30,
     -30,   322,   -30,   -30,    12,   -30,    -2,     1,   -30,   322,
     -30,   322,   322,   322,   -30,   322,   322,   322,   -30,   -30,
      -6,   -30,   -30,   -30,     0,     7,    65,    17,   -30,   -30,
     -30,   493,    17,   493,   -30,   222,   -30,   -30,    -1,   117,
     341,   373,   405,   437,   270,   214,   -12,   -12,   322,   322,
     322,    13,    19,     8,    12,   322,    12,   322,    37,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,    52,    53,    57,
     -30,   -30,   -30,   -30,   -30,   322,   493,   493,   493,   -30,
     -30,   -30,    59,   493,   -30,   493,   -30,   493,    61,   517,
      16,    16,    16,    16,    16,    16,   214,   214,   214,   -10,
     -10,   304,   304,   -12,   -12,   -12,   -12,    12,   -30,    50,
      66,   -30,   -30,   222,   222,   174,   493,   -30,   322,   300,
     -24,   222,   -30,   -30,    45,    45,   -30,   493,   -30,    62,
     322,   322,   222,    84,   322,    80,   100,   -30,   493,   493,
     -30,   469,   -30,   -30,   222,   -30,   222
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
        17,     0,     2,     1,    53,    54,    55,    56,    57,    58,
      39,    34,    24,    25,     0,    17,     0,    45,    17,     0,
      18,     0,     0,     0,    17,     0,     0,     0,    19,     4,
       3,    21,    23,    22,    41,    42,    61,     0,    60,    51,
      59,    20,    36,    35,    26,    30,    37,    46,     0,     0,
       0,     0,     0,     0,     0,    88,    73,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    32,     0,     0,
      52,    90,    17,    17,    17,     0,    12,    11,     6,    38,
      40,    62,     0,    48,    43,    67,    44,    68,    64,    76,
      77,    79,    82,    81,    80,    78,    85,    86,    87,    84,
      83,    69,    70,    71,    72,    74,    75,     0,    17,     0,
       0,    50,    47,    13,    13,     0,     7,    63,     0,     0,
      27,    33,    17,     8,    15,    15,     5,    49,    65,     0,
       0,     0,    31,     0,     0,     0,     0,    66,    29,    28,
      17,     0,    10,     9,    16,    17,    14
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Parser::yypgoto_[] =
  {
       -30,   -30,   -30,   -30,   -23,   -29,   -15,   -30,   -30,   -30,
     -30,   -30,   -30,   -30,   -30,   -30,   -13,   121,   -30,    -5,
     -30,    49,   -30,    22
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     1,    28,    29,   144,   155,     2,    30,    31,    32,
     129,    87,   130,    33,    34,    35,    36,    37,    48,   102,
      38,    39,    40,    41
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned char
  Parser::yytable_[] =
  {
        45,    44,   150,    49,     3,   151,    68,    46,    68,    54,
      47,     4,     5,     6,     7,     8,     9,    10,    61,    88,
      58,    10,    99,    59,    60,    62,    16,    17,   100,    18,
      16,    19,   101,    43,    68,    81,    82,    83,    84,    85,
      86,    50,    86,    51,    52,    53,   108,    55,    56,    57,
      25,   104,    89,   106,    26,    76,    77,    78,    27,    79,
      80,    81,    82,    83,    84,    85,   132,   127,    86,   128,
      66,    67,   153,   154,    18,   142,   143,   133,   134,   135,
      96,    97,    98,   137,   139,   103,   157,   105,    63,   107,
     162,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   160,
     163,   145,   138,   141,   140,   138,   156,   136,    64,    65,
       4,     5,     6,     7,     8,     9,    10,   152,    11,    12,
      13,    14,    42,    15,   149,    16,    17,   131,    18,    90,
      19,     0,    20,    21,     0,   164,    22,    23,    24,     0,
     166,     0,     0,     0,     0,     0,     0,     0,     0,    25,
     147,   103,     0,    26,     0,     0,     0,    27,     0,     0,
       0,     0,   158,   159,     0,     0,   161,     4,     5,     6,
       7,     8,     9,    10,   146,    11,    12,    13,    14,     0,
      15,     0,    16,    17,     0,    18,     0,    19,     0,    20,
      21,     0,     0,    22,    23,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
      26,     0,     0,     0,    27,     4,     5,     6,     7,     8,
       9,    10,    68,    11,    12,    13,    14,     0,    15,     0,
      16,    17,     0,    18,     0,    19,     0,    20,    21,     0,
       0,    22,    23,    24,     0,     0,     0,    79,    80,    81,
      82,    83,    84,    85,    25,     0,    86,     0,    26,     0,
       0,     0,    27,     4,     5,     6,     7,     8,     9,    10,
       0,    11,    12,    13,    14,     0,    15,     0,    16,    17,
       0,    18,     0,    19,     0,    20,    21,     0,     0,    22,
      95,    24,     0,     4,     5,     6,     7,     8,     9,    10,
       0,     0,    25,     0,     0,     0,    26,     0,    16,    17,
      27,    18,    68,    19,   148,     4,     5,     6,     7,     8,
       9,    10,     0,     0,     0,     0,     0,     0,     0,     0,
      16,    17,    25,    18,     0,    19,    26,     0,     0,     0,
      27,    83,    84,    85,     0,     0,    86,     0,     0,    68,
       0,     0,     0,     0,    25,    91,     0,     0,    26,     0,
       0,     0,    27,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,    81,    82,    83,    84,
      85,    68,     0,    86,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,    81,    82,
      83,    84,    85,    68,     0,    86,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,     0,    79,    80,
      81,    82,    83,    84,    85,    68,     0,    86,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,    81,    82,    83,    84,    85,    68,     0,    86,
       0,     0,     0,     0,   165,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    68,    79,    80,    81,    82,    83,    84,    85,     0,
       0,    86,     0,     0,     0,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    68,    79,    80,    81,    82,
      83,    84,    85,     0,     0,    86,     0,     0,     0,     0,
      70,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,    81,    82,    83,    84,    85,     0,     0,    86
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
        15,    14,    26,    18,     0,    29,    18,     9,    18,    24,
       9,     3,     4,     5,     6,     7,     8,     9,    18,    20,
      26,     9,     9,    29,    30,    18,    18,    19,     9,    21,
      18,    23,    24,    11,    18,    45,    46,    47,    48,    49,
      52,    19,    52,    21,    22,    23,     9,    25,    26,    27,
      42,    64,    53,    66,    46,    39,    40,    41,    50,    43,
      44,    45,    46,    47,    48,    49,     9,    15,    52,    17,
      53,    54,    27,    28,    21,    25,    10,    92,    93,    94,
      58,    59,    60,    24,    23,    63,    24,    65,    23,    67,
      10,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    25,
      10,   134,    53,   128,   127,    53,   145,    95,    53,    54,
       3,     4,     5,     6,     7,     8,     9,   142,    11,    12,
      13,    14,    11,    16,   139,    18,    19,    88,    21,    22,
      23,    -1,    25,    26,    -1,   160,    29,    30,    31,    -1,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,
     138,   139,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,   150,   151,    -1,    -1,   154,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    -1,
      16,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    25,
      26,    -1,    -1,    29,    30,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,     3,     4,     5,     6,     7,
       8,     9,    18,    11,    12,    13,    14,    -1,    16,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    25,    26,    -1,
      -1,    29,    30,    31,    -1,    -1,    -1,    43,    44,    45,
      46,    47,    48,    49,    42,    -1,    52,    -1,    46,    -1,
      -1,    -1,    50,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    12,    13,    14,    -1,    16,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    25,    26,    -1,    -1,    29,
      30,    31,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    42,    -1,    -1,    -1,    46,    -1,    18,    19,
      50,    21,    18,    23,    24,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    42,    21,    -1,    23,    46,    -1,    -1,    -1,
      50,    47,    48,    49,    -1,    -1,    52,    -1,    -1,    18,
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
      47,    48,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    45,    46,    47,    48,    49,    -1,    -1,    52
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,    56,    61,     0,     3,     4,     5,     6,     7,     8,
       9,    11,    12,    13,    14,    16,    18,    19,    21,    23,
      25,    26,    29,    30,    31,    42,    46,    50,    57,    58,
      62,    63,    64,    68,    69,    70,    71,    72,    75,    76,
      77,    78,    72,    78,    71,    61,     9,     9,    73,    61,
      78,    78,    78,    78,    61,    78,    78,    78,    26,    29,
      30,    18,    18,    23,    53,    54,    53,    54,    18,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    43,
      44,    45,    46,    47,    48,    49,    52,    66,    20,    53,
      22,    24,    25,    25,    25,    30,    78,    78,    78,     9,
       9,    24,    74,    78,    71,    78,    71,    78,     9,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    15,    17,    65,
      67,    76,     9,    61,    61,    61,    78,    24,    53,    23,
      71,    61,    25,    10,    59,    59,    10,    78,    24,    74,
      26,    29,    61,    27,    28,    60,    60,    24,    78,    78,
      25,    78,    10,    10,    61,    25,    61
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
  "THROW", "CATCH", "TRY", "FINALLY", "'.'", "'['", "']'", "'{'", "'}'",
  "'('", "')'", "EOL", "UNLESS", "ELSE", "ELSIF", "IF", "WHILE", "DO",
  "'='", "'>'", "'<'", "LOG_OR", "LOG_AND", "LTE", "GTE", "'|'", "'&'",
  "'^'", "'~'", "RSHFT", "LSHFT", "'+'", "'-'", "'*'", "'/'", "'%'",
  "LOG_NOT", "NEG", "POW", "','", "':'", "$accept", "program", "statement",
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
        56,     0,    -1,    61,    -1,    62,    -1,    58,    -1,    30,
      78,    25,    61,    10,    -1,    62,    30,    78,    -1,    31,
      61,    30,    78,    -1,    16,    61,    66,    67,    10,    -1,
      29,    78,    25,    61,    59,    60,    10,    -1,    26,    78,
      25,    61,    59,    60,    10,    -1,    62,    29,    78,    -1,
      62,    26,    78,    -1,    -1,    59,    28,    78,    25,    61,
      -1,    -1,    27,    25,    61,    -1,    -1,    61,    25,    -1,
      61,    57,    -1,    78,    -1,    63,    -1,    68,    -1,    64,
      -1,    12,    -1,    13,    -1,    14,    71,    -1,    15,    71,
      -1,    15,    71,    29,    78,    -1,    15,    71,    26,    78,
      -1,    -1,    66,    65,    25,    61,    -1,    -1,    17,    61,
      -1,    11,    -1,    11,    78,    -1,    11,    72,    -1,    18,
       9,    -1,    69,    18,     9,    -1,     9,    -1,    70,    18,
       9,    -1,    69,    -1,    70,    -1,    71,    53,    71,    -1,
      72,    53,    71,    -1,    -1,     9,    -1,    73,    53,     9,
      -1,    78,    -1,    74,    53,    78,    -1,    19,    73,    20,
      76,    -1,    76,    -1,    21,    61,    22,    -1,     3,    -1,
       4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,
      77,    -1,    75,    -1,    71,    -1,    71,    23,    24,    -1,
      71,    23,    74,    24,    -1,    78,    18,     9,    -1,    78,
      18,     9,    23,    24,    -1,    78,    18,     9,    23,    74,
      24,    -1,    71,    54,    78,    -1,    72,    54,    78,    -1,
      78,    45,    78,    -1,    78,    46,    78,    -1,    78,    47,
      78,    -1,    78,    48,    78,    -1,    46,    78,    -1,    78,
      49,    78,    -1,    78,    52,    78,    -1,    78,    32,    78,
      -1,    78,    33,    78,    -1,    78,    38,    78,    -1,    78,
      34,    78,    -1,    78,    37,    78,    -1,    78,    36,    78,
      -1,    78,    35,    78,    -1,    78,    44,    78,    -1,    78,
      43,    78,    -1,    78,    39,    78,    -1,    78,    40,    78,
      -1,    78,    41,    78,    -1,    42,    78,    -1,    50,    78,
      -1,    23,    78,    24,    -1
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
         0,    60,    60,    63,    64,    65,    66,    67,    68,    71,
      72,    73,    74,    77,    78,    81,    82,    85,    86,    87,
      90,    91,    94,    95,    96,    97,   100,   103,   104,   105,
     108,   109,   112,   113,   116,   117,   118,   121,   122,   125,
     126,   129,   130,   133,   134,   136,   137,   138,   141,   142,
     145,   146,   149,   152,   153,   154,   155,   156,   157,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191
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
  const int Parser::yylast_ = 569;
  const int Parser::yynnts_ = 24;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 3;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 55;

  const unsigned int Parser::yyuser_token_number_max_ = 288;
  const Parser::token_number_type Parser::yyundef_token_ = 2;

} // namespace snow

#line 194 "Parser.yy"


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
    
    return result;
}
