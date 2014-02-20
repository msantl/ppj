
int f(void);
int main(void) {
  int x = 3;
  int y = x;
  return f();
}
int f(void){
  return 12;
}