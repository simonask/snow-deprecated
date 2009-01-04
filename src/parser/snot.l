%{
#include <stdio.h>
%}

%%
[0-9]+                  printf("DECIMAL ");
[0-9]*\.[0-9]+          printf("FLOAT ");
0x[0-9A-Fa-f]+          printf("HEX ");
0b[0-1]+                printf("BINARY ");
0o[0-7]+                printf("OCTAL ");
(\".+?\")|(\'.+?\')     printf("STRING ");
true                    printf("TRUE ");
false                   printf("FALSE ");
null                    printf("NULL ");

if                      printf("IF ");
else                    printf("ELSE ");
switch                  printf("SWITCH ");
case                    printf("CASE ");
while                   printf("WHILE ");
break                   printf("BREAK ");
return                  printf("RETURN ");
end                     printf("END ");

[a-zA-Z][a-zA-Z0-9_!?]* printf("IDENTIFIER ");

[-+*/%]                 printf("%s ", yytext);

\*\*                    printf("POWER ");

&                       printf("AMPER ");

\|                      printf("BW_OR ");
\^                      printf("BW_XOR ");
~                       printf("BW_NOT ");

&&                      printf("LOG_AND ");
\|\|                    printf("LOG_OR ");
!                       printf("LOG_NOT ");

=                       printf("EQ ");
≠|!=                    printf("NEQ ");
\>                      printf("GT ");
\>=                     printf("GTE ");
\<                      printf("LT ");
\<=                     printf("LTE ");

,|=>                    printf("DELIM ");
:                       printf("ASSIGNMENT ");

[\.\(\)\[\]\{\}]        printf("%s ", yytext);

[ \t]+                  /* ignore whitespaces */
%%

int yywrap(void) {
    return 1;
}

int main(void) {
    yylex();
    return 0;
}