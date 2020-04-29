#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int* mem;
int buf;
int* ptr;
char* code;
char* start;
int memsize;
int fsize;
int* stack;
int* ss;
int gnum(int* mem, int* ptr, int arg);
int stn();
void trace(int* i){
  while(i <= stack){
    printf("%d ", *i);
    i++;
  }
  printf("\n");
}
char* jump(int direction, char* code, char opening ,char closing){
  int paren = 0;

  do{

    if(*code == opening){
      paren++;
    }
    if(*code == closing){
      paren--;
    }

    code += direction;
  }while(paren);

  code -= direction;
  return code;
}


int power (int b, int n){
  int r = 1;
  for(int i = 0; i < n; i++){
    r *= b;
  }
  return r;
}
int and(int* a, int b){return (*a)&b;}
int or(int* a, int b){return (*a)|b;}
int xor(int* a, int b){return (*a)^b;}
int plus(int* a, int b){return (*a)+b;}
int minus(int* a, int b){return (*a)-b;}
int mul(int* a, int b){return (*a)*b;}
int divi(int* a, int b){return (*a)/b;}
int eq(int* a, int b){return (*a)==b;}
int less(int* a, int b){return (*a)<b;}
int greater(int* a, int b){return (*a)>b;}
int not(int* a, int b){return !b;}
int modulo(int* a, int b){return (*a)%b;}
int rightp(int* a, int b){return (a+b)-mem;}
int leftp(int* a, int b){return (a-b)-mem;}
int at(int* a, int b){return *(mem+b);}
int here(int* a, int b){return (int)(a - mem);}


typedef int (*funptr)(int*, int);
funptr map[256];
void initMap(){
  map['&'] = and;
  map['|'] = or;
  map['^'] = xor;
  map['+'] = plus;
  map['-'] = minus;
  map['*'] = mul;
  map['/'] = divi;
  map['='] = eq;
  map['<'] = less;
  map['>'] = greater;
  map['!'] = not;
  map['%'] = modulo;
  map[')'] = rightp;
  map['('] = leftp;
  map['@'] = at;
  map['v'] = here;
}
void g(){
  ptr = mem + gnum(mem, ptr, *ptr);

}
void p(){
  printf("%d", *ptr);

}
void a(){
  printf("%c", (char)*ptr);

}
void n(){
  printf("\n");

}
void i(){
  printf("input\n");
  scanf("%d", &buf);
  *ptr = buf;
}

void t(){
  for (int i = 0; i <= ptr-mem; i++){

    printf("%d %c\n", *(mem+i), (mem+i) == ptr ? '<' : ' ');
  }

}

void s(){

  *ptr = gnum(mem, ptr, *ptr);
}


void os(){
  if (*ptr){
    *stack = code - start;
    stack++;
  }
  else{
    code = jump(1,code,'[',']');
  }
}
void cs(){
  if (*ptr){

      code = start + *(stack-1);

  }
  else{
    stack--;
  }
}
void oc(){
  if (!*ptr)
  code = jump(1,code,'{','}');
}
void cc(){

}


typedef void (*spec)();

spec special[256];
void initSpecial(){
  special['g'] = g;
  special['p'] = p;
  special['n'] = n;
  special['i'] = i;
  special['a'] = a;
  special['['] = os;
  special[']'] = cs;
  special['s'] = s;
  special['~'] = t;
  special['{'] = oc;
  special['}'] = cc;
}



int stn(){  // string to number
  int result = 0;
  int size = 0;



  while(isdigit(*code)){
    code++;
    size++;
  }

  int p = 1;

  code--;
  while(isdigit(*code)){

    result += p * (int) (*code - '0');
    p *= 10;
    code--;

  }
  code += size;
  return result;
}

int gnum(int* mem, int* ptr, int arg){

  code++;
  char* start = code;


  if(map[*start] != NULL){
    return map[*start](ptr,gnum(mem, ptr, arg));
  }


  return stn();
}

void loopP(int toomuch){
  while (code-start < toomuch-1){
    (special[*code])();
    code++;
  }
}
int init(int argc, char** argv){
  initMap();
  initSpecial();
  stack = (int*)calloc(100, sizeof(int));
  ss = stack;
  if (argc <= 2){
    printf("format for use:\n ./a.out [file] [memory use]\n");
    return 1;
  }
  FILE* file = fopen(argv[1],"r");
  code = argv[2];
  memsize = stn();

  if (file == NULL){
    printf("could not find file\n");
    return 1;
  }

  mem = (int*) calloc(memsize, sizeof(int));
  ptr = mem;
  fseek(file, 0, SEEK_END); 
  fsize = ftell(file); 
  fseek(file, 0, SEEK_SET); 
  code = (char*) malloc(fsize * sizeof(char));
  start = code;
  char op;
  do
    {
      op = (char)fgetc(file);

      if(!isspace(op)){

        *code = op;
        code++;

      }


    } while(op != EOF);
  fclose(file);
  return 0;
}

int main(int argc, char** argv){
  if (init(argc, argv)){return 1;}
  int toomuch = code-start;
  code = start;

  loopP(toomuch);

  free(mem);
  free(start);
  free(ss);
  return 0;
}
