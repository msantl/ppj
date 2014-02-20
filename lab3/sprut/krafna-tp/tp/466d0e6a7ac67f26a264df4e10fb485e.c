
void f2(int x, int a[]){
    x = x + 1;
    a[0] = a[0] + 1;
    return;
}

int main(void) {
  int a[5];           
  a[0]= 1;a[1]= 1;a[2]= 1;a[3]= 1;a[4]= 1; 
  f2(1,a);
  
  return 0;
}

