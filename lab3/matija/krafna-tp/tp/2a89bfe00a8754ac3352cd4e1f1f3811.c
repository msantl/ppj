int main(void) {
  
  int x = 2147483647, y;
  (int)'a';
  (const char)x;
  (const int)'a';
  (char)((const int)300 + (int)'a');
  (int)(char)(const int)(const char)(x + y);

  
  
               
  
  return 0;
}
