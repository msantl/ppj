int main(void) {
  int i = 10;

  while (i--) {
    if (i < 5) break;
    else continue;
  }

  return i;
}
