int f(int n) {
  if (n == 1)
    return 1;
  else
    return n * f(n - 1);
}
int main(void) {
    return f(5);
}
