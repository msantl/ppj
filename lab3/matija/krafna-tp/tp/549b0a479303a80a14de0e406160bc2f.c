
int f(void);
int main(void) {
  f();
  {
    int x;
  }
  return printf("hello world!\n");
}
int f(void){
  return ;
}