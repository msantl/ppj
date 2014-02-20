/**
 * ppjC je programski jezik podskup jezika C definiran u dokumentu
 * https://github.com/fer-ppj/ppj-labosi/raw/master/upute/ppj-labos-upute.pdf
 *
 * ova skripta poziva ppjC kompajler (za sada samo analizator) pritiskom
 * na tipku [Ctrl+S], [Shift+Enter] ili [Alt+3] i prikazuje rezultat analize.
 *
 * ne garantiram tocnost leksera, sintaksnog niti semantickog analizatora koji
 * se ovdje pokrece.
 *
 * URL skripte prati verzije izvornog programa, tako da je moguca razmjena
 * izvornih programa u timu putem URL-ova.
 */
char x;
const int z = 9;
int printf(const char format[]) {
  /* i wish i could printf */
  return 0;
}
int fun( int a, char g [] );
int main(void) {
  int a = 4;
  {
    int a = 2;
    while (a==3)
    {
      break;
    }
  char jjj[2] = { 'a', 'b'};
  int c = fun ( 4 ,  jjj[0] );
  return printf("hello world!\n");
}

int fun( int a, char g [] )
{
  const int i = 8;
  char ch = 'h';
  return i + ch;
}



