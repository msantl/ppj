KR_VOID 1 void
IDN 1 foo
L_ZAGRADA 1 (
KR_INT 1 int
IDN 1 x
ZAREZ 1 ,
KR_INT 1 int
IDN 1 c
ZAREZ 1 ,
KR_CHAR 1 char
IDN 1 h
D_ZAGRADA 1 )
TOCKAZAREZ 1 ;
KR_INT 3 int
IDN 3 f
L_ZAGRADA 3 (
KR_VOID 3 void
D_ZAGRADA 3 )
L_VIT_ZAGRADA 3 {
KR_RETURN 4 return
BROJ 4 0
TOCKAZAREZ 4 ;
D_VIT_ZAGRADA 5 }
KR_INT 7 int
IDN 7 x
OP_PRIDRUZI 7 =
BROJ 7 3
TOCKAZAREZ 7 ;
KR_INT 11 int
IDN 11 main
L_ZAGRADA 11 (
KR_VOID 11 void
D_ZAGRADA 11 )
L_VIT_ZAGRADA 11 {
KR_INT 12 int
IDN 12 a
OP_PRIDRUZI 12 =
BROJ 12 5
TOCKAZAREZ 12 ;
KR_CONST 13 const
KR_CHAR 13 char
IDN 13 c
OP_PRIDRUZI 13 =
ZNAK 13 'i'
TOCKAZAREZ 13 ;
KR_INT 14 int
IDN 14 niz
L_UGL_ZAGRADA 14 [
BROJ 14 3
D_UGL_ZAGRADA 14 ]
TOCKAZAREZ 14 ;
KR_VOID 15 void
IDN 15 foo
L_ZAGRADA 15 (
KR_INT 15 int
IDN 15 x
ZAREZ 15 ,
KR_INT 15 int
IDN 15 c
ZAREZ 15 ,
KR_CHAR 15 char
IDN 15 h
D_ZAGRADA 15 )
TOCKAZAREZ 15 ;
KR_INT 16 int
IDN 16 x
OP_PRIDRUZI 16 =
BROJ 16 5
TOCKAZAREZ 16 ;
KR_INT 17 int
IDN 17 y
OP_PRIDRUZI 17 =
IDN 17 x
PLUS 17 +
BROJ 17 1
TOCKAZAREZ 17 ;
IDN 18 a
OP_PRIDRUZI 18 =
IDN 18 a
PLUS 18 +
BROJ 18 3
TOCKAZAREZ 18 ;
KR_IF 20 if
L_ZAGRADA 20 (
IDN 20 a
OP_GT 20 >
BROJ 20 2
D_ZAGRADA 20 )
L_VIT_ZAGRADA 21 {
KR_INT 22 int
IDN 22 a
TOCKAZAREZ 22 ;
KR_INT 23 int
IDN 23 b
TOCKAZAREZ 23 ;
IDN 24 a
OP_PRIDRUZI 24 =
IDN 24 b
TOCKAZAREZ 24 ;
IDN 25 x
OP_PRIDRUZI 25 =
BROJ 25 4
TOCKAZAREZ 25 ;
D_VIT_ZAGRADA 26 }
KR_RETURN 27 return
IDN 27 f
L_ZAGRADA 27 (
D_ZAGRADA 27 )
TOCKAZAREZ 27 ;
D_VIT_ZAGRADA 28 }
KR_INT 30 int
IDN 30 fact
L_ZAGRADA 30 (
KR_INT 30 int
IDN 30 bzvz
D_ZAGRADA 30 )
TOCKAZAREZ 30 ;
KR_VOID 32 void
IDN 32 foo
L_ZAGRADA 32 (
KR_INT 32 int
IDN 32 x
ZAREZ 32 ,
KR_INT 32 int
IDN 32 c
ZAREZ 32 ,
KR_CHAR 32 char
IDN 32 h
D_ZAGRADA 32 )
L_VIT_ZAGRADA 32 {
KR_INT 33 int
IDN 33 i
OP_PRIDRUZI 33 =
IDN 33 fact
L_ZAGRADA 33 (
IDN 33 x
D_ZAGRADA 33 )
TOCKAZAREZ 33 ;
KR_INT 34 int
IDN 34 a
OP_PRIDRUZI 34 =
IDN 34 a
PLUS 34 +
BROJ 34 1
TOCKAZAREZ 34 ;
KR_VOID 35 void
IDN 35 foo3
L_ZAGRADA 35 (
KR_VOID 35 void
D_ZAGRADA 35 )
TOCKAZAREZ 35 ;
KR_CONST 37 const
KR_INT 37 int
IDN 37 niz1
L_UGL_ZAGRADA 37 [
BROJ 37 5
D_UGL_ZAGRADA 37 ]
OP_PRIDRUZI 37 =
L_VIT_ZAGRADA 37 {
BROJ 37 1
ZAREZ 37 ,
BROJ 37 2
ZAREZ 37 ,
BROJ 37 3
D_VIT_ZAGRADA 37 }
TOCKAZAREZ 37 ;
KR_INT 38 int
IDN 38 niz
L_UGL_ZAGRADA 38 [
BROJ 38 10
D_UGL_ZAGRADA 38 ]
OP_PRIDRUZI 38 =
L_VIT_ZAGRADA 38 {
BROJ 38 1
ZAREZ 38 ,
BROJ 38 2
D_VIT_ZAGRADA 38 }
TOCKAZAREZ 38 ;
KR_FOR 40 for
L_ZAGRADA 40 (
IDN 40 i
OP_PRIDRUZI 40 =
BROJ 40 0
TOCKAZAREZ 40 ;
IDN 40 i
OP_LT 40 <
BROJ 40 5
TOCKAZAREZ 40 ;
IDN 40 i
OP_INC 40 ++
D_ZAGRADA 40 )
KR_BREAK 41 break
TOCKAZAREZ 41 ;
KR_WHILE 43 while
L_ZAGRADA 43 (
BROJ 43 1
D_ZAGRADA 43 )
L_VIT_ZAGRADA 44 {
KR_BREAK 45 break
TOCKAZAREZ 45 ;
IDN 46 i
OP_PRIDRUZI 46 =
IDN 46 i
PLUS 46 +
BROJ 46 2
TOCKAZAREZ 46 ;
D_VIT_ZAGRADA 47 }
KR_RETURN 50 return
TOCKAZAREZ 50 ;
D_VIT_ZAGRADA 52 }
KR_CHAR 54 char
IDN 54 proba3
L_ZAGRADA 54 (
KR_VOID 54 void
D_ZAGRADA 54 )
L_VIT_ZAGRADA 54 {
IDN 55 x
OP_PRIDRUZI 55 =
BROJ 55 4
TOCKAZAREZ 55 ;
KR_IF 56 if
L_ZAGRADA 56 (
BROJ 56 0
D_ZAGRADA 56 )
L_VIT_ZAGRADA 57 {
IDN 58 x
OP_PRIDRUZI 58 =
BROJ 58 6
TOCKAZAREZ 58 ;
KR_RETURN 59 return
ZNAK 59 'a'
TOCKAZAREZ 59 ;
D_VIT_ZAGRADA 60 }
KR_ELSE 60 else
L_VIT_ZAGRADA 60 {
KR_RETURN 61 return
L_ZAGRADA 61 (
KR_CHAR 61 char
D_ZAGRADA 61 )
BROJ 61 97
TOCKAZAREZ 61 ;
D_VIT_ZAGRADA 63 }
D_VIT_ZAGRADA 64 }
KR_CHAR 66 char
IDN 66 proba
L_ZAGRADA 66 (
KR_VOID 66 void
D_ZAGRADA 66 )
L_VIT_ZAGRADA 66 {
KR_RETURN 67 return
L_ZAGRADA 67 (
KR_CHAR 67 char
D_ZAGRADA 67 )
BROJ 67 97
TOCKAZAREZ 67 ;
D_VIT_ZAGRADA 68 }
KR_INT 71 int
IDN 71 proba2
L_ZAGRADA 71 (
KR_VOID 71 void
D_ZAGRADA 71 )
L_VIT_ZAGRADA 71 {
KR_RETURN 73 return
ZNAK 73 'a'
TOCKAZAREZ 73 ;
D_VIT_ZAGRADA 74 }
KR_INT 76 int
IDN 76 fact
L_ZAGRADA 76 (
KR_INT 76 int
IDN 76 n
D_ZAGRADA 76 )
L_VIT_ZAGRADA 76 {
IDN 77 foo
L_ZAGRADA 77 (
BROJ 77 1
ZAREZ 77 ,
BROJ 77 2
ZAREZ 77 ,
ZNAK 77 'h'
D_ZAGRADA 77 )
TOCKAZAREZ 77 ;
KR_IF 78 if
L_ZAGRADA 78 (
IDN 78 n
OP_GT 78 >
BROJ 78 0
D_ZAGRADA 78 )
KR_RETURN 79 return
IDN 79 n
OP_PUTA 79 *
IDN 79 fact
L_ZAGRADA 79 (
IDN 79 n
MINUS 79 -
BROJ 79 1
D_ZAGRADA 79 )
TOCKAZAREZ 79 ;
KR_ELSE 80 else
KR_RETURN 81 return
BROJ 81 1
TOCKAZAREZ 81 ;
D_VIT_ZAGRADA 82 }
KR_VOID 84 void
IDN 84 foo3
L_ZAGRADA 84 (
KR_VOID 84 void
D_ZAGRADA 84 )
L_VIT_ZAGRADA 84 {
KR_INT 85 int
IDN 85 i
OP_PRIDRUZI 85 =
BROJ 85 8
TOCKAZAREZ 85 ;
KR_RETURN 87 return
TOCKAZAREZ 87 ;
D_VIT_ZAGRADA 88 }
KR_VOID 90 void
IDN 90 f2
L_ZAGRADA 90 (
KR_INT 90 int
IDN 90 x
ZAREZ 90 ,
KR_INT 90 int
IDN 90 a
L_UGL_ZAGRADA 90 [
D_UGL_ZAGRADA 90 ]
D_ZAGRADA 90 )
L_VIT_ZAGRADA 90 {
IDN 91 x
OP_PRIDRUZI 91 =
IDN 91 x
PLUS 91 +
BROJ 91 1
TOCKAZAREZ 91 ;
IDN 92 a
L_UGL_ZAGRADA 92 [
BROJ 92 0
D_UGL_ZAGRADA 92 ]
OP_PRIDRUZI 92 =
IDN 92 a
L_UGL_ZAGRADA 92 [
BROJ 92 0
D_UGL_ZAGRADA 92 ]
PLUS 92 +
BROJ 92 1
TOCKAZAREZ 92 ;
D_VIT_ZAGRADA 93 }
KR_INT 104 int
IDN 104 main2
L_ZAGRADA 104 (
KR_VOID 104 void
D_ZAGRADA 104 )
L_VIT_ZAGRADA 104 {
KR_INT 105 int
IDN 105 x
OP_PRIDRUZI 105 =
BROJ 105 2147483647
ZAREZ 105 ,
IDN 105 y
TOCKAZAREZ 105 ;
L_ZAGRADA 107 (
KR_INT 107 int
D_ZAGRADA 107 )
ZNAK 107 'a'
TOCKAZAREZ 107 ;
L_ZAGRADA 108 (
KR_CONST 108 const
KR_CHAR 108 char
D_ZAGRADA 108 )
IDN 108 x
TOCKAZAREZ 108 ;
L_ZAGRADA 109 (
KR_CONST 109 const
KR_INT 109 int
D_ZAGRADA 109 )
ZNAK 109 'a'
TOCKAZAREZ 109 ;
L_ZAGRADA 110 (
KR_CHAR 110 char
D_ZAGRADA 110 )
L_ZAGRADA 110 (
L_ZAGRADA 110 (
KR_CONST 110 const
KR_INT 110 int
D_ZAGRADA 110 )
BROJ 110 300
PLUS 110 +
L_ZAGRADA 110 (
KR_INT 110 int
D_ZAGRADA 110 )
ZNAK 110 'a'
D_ZAGRADA 110 )
TOCKAZAREZ 110 ;
L_ZAGRADA 111 (
KR_INT 111 int
D_ZAGRADA 111 )
L_ZAGRADA 111 (
KR_CHAR 111 char
D_ZAGRADA 111 )
L_ZAGRADA 111 (
KR_CONST 111 const
KR_INT 111 int
D_ZAGRADA 111 )
L_ZAGRADA 111 (
KR_CONST 111 const
KR_CHAR 111 char
D_ZAGRADA 111 )
L_ZAGRADA 111 (
IDN 111 x
PLUS 111 +
IDN 111 y
D_ZAGRADA 111 )
TOCKAZAREZ 111 ;
KR_RETURN 113 return
BROJ 113 0
TOCKAZAREZ 113 ;
D_VIT_ZAGRADA 114 }
