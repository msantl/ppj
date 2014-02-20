
int f(int a, int b);
int main(void) {
  int x = 3;
  int y = x;
  return f(f(x,y),y);
}
int f(int a, int b){
  return 12;
}