KR_VOID 1 void
IDN 1 fun
L_ZAGRADA 1 (
KR_INT 1 int
IDN 1 a
L_UGL_ZAGRADA 1 [
D_UGL_ZAGRADA 1 ]
D_ZAGRADA 1 )
L_VIT_ZAGRADA 1 {
KR_RETURN 2 return
TOCKAZAREZ 2 ;
D_VIT_ZAGRADA 3 }
KR_INT 5 int
IDN 5 main
L_ZAGRADA 5 (
KR_VOID 5 void
D_ZAGRADA 5 )
L_VIT_ZAGRADA 5 {
KR_INT 6 int
IDN 6 A
L_UGL_ZAGRADA 6 [
BROJ 6 512
D_UGL_ZAGRADA 6 ]
TOCKAZAREZ 6 ;
KR_CONST 7 const
KR_CHAR 7 char
IDN 7 tmp
L_UGL_ZAGRADA 7 [
D_UGL_ZAGRADA 7 ]
OP_PRIDRUZI 7 =
NIZ_ZNAKOVA 7 "te  \nst"
TOCKAZAREZ 7 ;
KR_INT 8 int
IDN 8 xYz
ZAREZ 8 ,
IDN 8 abc
TOCKAZAREZ 8 ;
IDN 10 xYz
OP_PRIDRUZI 10 =
BROJ 10 12345
TOCKAZAREZ 10 ;
IDN 11 abc
OP_PRIDRUZI 11 =
IDN 11 xYz
TOCKAZAREZ 11 ;
IDN 12 abc
OP_PRIDRUZI 12 =
IDN 12 abc
OP_INC 12 ++
PLUS 12 +
IDN 12 xYz
TOCKAZAREZ 12 ;
IDN 13 abc
OP_PRIDRUZI 13 =
BROJ 13 1
PLUS 13 +
BROJ 13 2
OP_PUTA 13 *
BROJ 13 3
OP_BIN_ILI 13 |
BROJ 13 4
OP_BIN_I 13 &
BROJ 13 5
TOCKAZAREZ 13 ;
KR_FOR 15 for
L_ZAGRADA 15 (
IDN 15 i
OP_PRIDRUZI 15 =
BROJ 15 0
TOCKAZAREZ 15 ;
IDN 15 i
OP_LT 15 <
BROJ 15 4
TOCKAZAREZ 15 ;
OP_INC 15 ++
IDN 15 i
D_ZAGRADA 15 )
L_VIT_ZAGRADA 15 {
IDN 16 tmp
L_UGL_ZAGRADA 16 [
IDN 16 i
D_UGL_ZAGRADA 16 ]
OP_PRIDRUZI 16 =
L_ZAGRADA 16 (
KR_CHAR 16 char
D_ZAGRADA 16 )
IDN 16 abc
TOCKAZAREZ 16 ;
KR_CONTINUE 17 continue
TOCKAZAREZ 17 ;
D_VIT_ZAGRADA 18 }
KR_IF 20 if
L_ZAGRADA 20 (
BROJ 20 1
OP_GTE 20 >=
BROJ 20 3
OP_I 20 &&
IDN 20 i
OP_GT 20 >
BROJ 20 2
D_ZAGRADA 20 )
IDN 21 fun
L_ZAGRADA 21 (
BROJ 21 35
D_ZAGRADA 21 )
TOCKAZAREZ 21 ;
KR_ELSE 22 else
KR_IF 22 if
L_ZAGRADA 22 (
IDN 22 i
OP_LT 22 <
BROJ 22 12
D_ZAGRADA 22 )
IDN 23 fun
L_ZAGRADA 23 (
BROJ 23 5
D_ZAGRADA 23 )
TOCKAZAREZ 23 ;
KR_ELSE 24 else
IDN 25 fun
L_ZAGRADA 25 (
BROJ 25 123
D_ZAGRADA 25 )
TOCKAZAREZ 25 ;
KR_RETURN 27 return
BROJ 27 0
TOCKAZAREZ 27 ;
D_VIT_ZAGRADA 28 }
