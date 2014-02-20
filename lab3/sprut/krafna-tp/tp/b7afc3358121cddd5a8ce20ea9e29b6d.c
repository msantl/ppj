
int f(int a, int b);
int main(void) {
  int x = 3;
  int y = x;
  x = f(x, y);
  return x;
}
int f(int a, int b){
  return 12;
}