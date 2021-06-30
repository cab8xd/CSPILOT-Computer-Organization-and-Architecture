
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void display(int x) {

    printf("%d",x);
}

 void display(const char* x){

    printf("%s", x);
}

void display(double x){

    printf("%f", x);
}

void display(char x) {

        printf("%c", x);
}

/*
class outf{
    FILE* f;
  
public:
    outf(FILE *f){;};
    outf& display(int x) {
        printf("%d",x);
        return *this;
    }
    outf& display(const char* x){
        printf("%s", x);
        return *this;
    }
    outf& display(double x){
        printf("%f", x);
        return *this;
    }
    outf& display(char x) {
        printf("%c", x);
        return *this;
    }
};
*/
class outf{
    FILE* f;
    
public:
    outf(FILE *f){;};
    outf& operator|(int x) {
        printf("%d",x);
        return *this;
    }
    outf& operator|(const char* x){
        printf("%s", x);
        return *this;
    }
    outf& operator|(double x){
        printf("%f", x);
        return *this;
    }
    outf& operator|(char x) {
        printf("%c", x);
        return *this;
    }
};





int main() {
    outf sout = outf(stdout); // constructor; no `new` means it's on the stack
   // sout.display("The number ").display(3).display(" is ").display(1.5).display('*').display(2).display('\n');
     sout | "The number " | 3 | " is " | 1.5 | '*' | 2 | '\n';
    return 0;
}


//Stack Class



