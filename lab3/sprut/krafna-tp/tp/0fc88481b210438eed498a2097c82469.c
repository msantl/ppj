int printf(const char format[]) {
  const int x = 2;
  const char y = '0';
  int z = x + y;
  return 0;
}

int main(void) {
  return printf("hello world!\n");
}
