int main(void){
int x=3;
int z;

{
int x=5;
int y=x+1;
z=y+1;
}
z=x+1;
return 0;
}