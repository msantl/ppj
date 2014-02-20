
int f(int a, int b);
int main(void) {
  int x = 3;
  int y = x;
  return f(x,y);
}
int f(int a, int b){
  
  return f(a,b);
}