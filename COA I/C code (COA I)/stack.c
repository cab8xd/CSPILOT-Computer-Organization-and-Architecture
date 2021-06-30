#include <stdio.h>

// do not change this class at all
class stack_node {
private:
    static int allocated_nodes;
public:
    stack_node *next;
    int value;
    
    stack_node(int v, stack_node *n) {
        value = v;
        next = n;
        stack_node::allocated_nodes += 1;
    }
    ~stack_node() { stack_node::allocated_nodes -= 1; }
    
    static int existing() { return allocated_nodes; }
};
int stack_node::allocated_nodes = 0;





// implement this class's unimplemented methods
class stack {
public:
    stack_node *head;
    
    stack() { head = NULL; }
    ~stack(){
        while(head != NULL)
        {
        this->pop();
        }
        
    }
    
    
    void push(int value){
        stack_node *newNode = new stack_node(value, head);
        head = newNode;
    }
    int pop(){
          int ret = head->value;
        stack_node *nNode = head->next;
      
        //set the head equal
   
       
        delete head;
        head = nNode;
        
        return ret;
    }
};

int main() {
    stack *a = new stack();
    for(int i=0; i<10; i+=1) a->push(i*i);
    for(int i=0; i<5; i+=1) printf("%d\n", a->pop());
    printf("Remaining nodes: %d\n", stack_node::existing());
    delete a;
    printf("Remaining nodes: %d\n", stack_node::existing());
    return 0;
};
