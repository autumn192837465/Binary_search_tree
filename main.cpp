#include <iostream>
#include <cstdlib>
using namespace std;




class node{
public:
	node();
	node(int value);
	~node();
	friend void gothrough(node *p);
    friend void serch(node *a);
private:
	node *left, *right; // the left child and the right child pointer
	int number; // record the node's number
	int is_threadl, is_threadr; //the flag to record whether the pointer is a thread or not

	friend class op_tree_totum;//you could access all valuables in class op_tree_totum
};

//ctor
node::node(){
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//ctor
node::node(int value){
	number = value;
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//dtor
node::~node(){

}

class op_tree_totum{

public:
	op_tree_totum();
	~op_tree_totum();
	void insertion(int s);
	void deletion(int s);
	void inorder_run();
	void reverseorder_run();
	int size();

private:
	node *root, *head, *tail;
	int num;//caculate how many nodes in the totum
};


//ctor
op_tree_totum::op_tree_totum(){
	head = new node();
	tail = new node();
	head->right = tail; //initialize the head node to connect to the tail node
	tail->left = head;
	root = NULL;
	num = 0;
}

//dtor
op_tree_totum::~op_tree_totum(){
	node *p = root;
	if(p!=NULL)	gothrough(p);
	num = 0;
	if (head != NULL)delete head;
	if (tail != NULL)delete tail;
}

void gothrough(node *p){
	if (p->is_threadl==0 && p->left!= NULL) gothrough(p->left);
	if (p->is_threadr==0 && p->right!= NULL) gothrough(p->right);
	delete p;
}

void op_tree_totum::insertion(int s){
	//TODO: fill in the code to do the insertion of the node with number s
    if(num==0){
        root= new node(s);
        num++;
        head->right=root;
        root->left=head;


        tail->left=root;
        root->right=tail;

    }
    else{
        node *temp=root;

        while(1){

            if(s>temp->number){
                if(temp->is_threadr){


                     node *right_thread=temp->right;
                     temp->is_threadr=0;

                     temp->right=new node(s);
                     //left thread
                     temp->right->left=temp;
                     //right->thread
                     temp->right->right=right_thread;

                     num++;
                     break;
                }
                temp=temp->right;
            }
            else if(s<temp->number){
                if(temp->is_threadl){
                     node *left_thread=temp->left;
                     temp->is_threadl=0;

                     temp->left=new node(s);
                     //right thread
                     temp->left->right=temp;
                     //left->thread
                     temp->left->left=left_thread;

                     num++;
                     break;
                }
                temp=temp->left;
            }
        }

        node *first=root;
        node *last=root;
        while(!first->is_threadl){
            first=first->left;
        }

        head->right=first;
        first->left=head;

        while(!last->is_threadr){
            last=last->right;
        }
        tail->left=last;
        last->right=tail;
    }


}

void op_tree_totum::deletion(int s){
	//TODO: fill in the code to do the deletion of the node with number s
    if(num==0){
        return;
    }
    else if(num==1){
        if(s==root->number){
            head->right=tail;
            tail->left=head;
            num--;
            delete root;
        }
    }
    else{
        node *last;
        node *temp=root;
        node *temp_child;
        bool from_right;
        while(1){
            if(s==temp->number) break;
            if(s>temp->number){
                if(temp->is_threadr){
                    return;
                }
                last=temp;
                temp=temp->right;
                from_right=0;
            }
            else{
                if(temp->is_threadl){
                    return;
                }
                last=temp;
                temp=temp->left;
                from_right=1;
            }
        }
        if(!temp->is_threadl&&!temp->is_threadr){
                node *child=temp->right;
                last=temp;
                while(!child->is_threadl)
                {
                    last=child;
                    child=child->left;
                }
                temp->number=child->number;

                if(last==temp && child->is_threadr){

                    temp->right=child->right;
                    if(child->right==tail)
                        tail->left=temp;
                    temp->is_threadr=1;

                    delete child;
                }
                else if(last==temp&&!child->is_threadr){

                    temp->right=child->right;
                    last=child->right;
                    while(!last->is_threadl) last=last->left;
                      last->left=temp;
                    delete child;
                }
                else if(!child->is_threadr){
                    last->left=child->right;
                    last=child->right;
                    while(!last->is_threadl) last=last->left;
                    last->left=temp;
                    delete child;
                }
                else if(child->is_threadr){
                    last->left=temp;
                    last->is_threadl=1;
                    child->right->left=temp;
                    delete child;
                }


        }
        else if(!temp->is_threadl && temp->is_threadr){

                if(s==root->number){
                    last=root;
                    root=root->left;
                    temp_child=root;
                    while(!temp_child->is_threadr) temp_child=temp_child->right;
                    temp_child->right=tail;
                    tail->left=temp_child;
                    delete last;

                }
                else if(!from_right){
                    last->right=temp->left;
                    temp_child=temp->left;
                    while(!temp_child->is_threadr) temp_child=temp_child->right;
                    temp_child->right=temp->right;
                    if(temp_child->right==tail)
                      tail->left=temp_child;

                    delete temp;
                }
                else{
                    last->left=temp->left;
                    temp_child=temp->left;
                    while(!temp_child->is_threadr) temp_child=temp_child->right;
                    temp_child->right=temp->right;
                    if(temp_child->right==tail)
                      tail->left=temp_child;

                    delete temp;
                }
        }
        else if(!temp->is_threadr && temp->is_threadl){
                if(s==root->number){
                    last=root;
                    root=root->right;
                    temp_child=root;
                    while(!temp_child->is_threadl) temp_child=temp_child->left;
                    temp_child->left=head;
                    head->right=temp_child;
                    delete last;

                }
                else if(!from_right){
                    last->right=temp->right;
                    temp_child=temp->right;
                    while(!temp_child->is_threadl) temp_child=temp_child->left;
                    temp_child->left=temp->left;
                    if(temp_child->left==head)
                      head->right=temp_child;
                    delete temp;
                }
                else{


                    last->left=temp->right;
                    temp_child=temp->right;
                    while(!temp_child->is_threadl) temp_child=temp_child->left;
                    temp_child->left=temp->left;
                    if(temp_child->left==head)
                      head->right=temp_child;

                    delete temp;
                }
        }
        else{
                if(!from_right){
                    last->right=temp->right;
                    if(temp->right==tail)
                      tail->left=last;
                    last->is_threadr=1;
                    delete temp;
                }
                else{

                    last->left=temp->left;
                    if(temp->left==head)
                      head->right=last;
                    last->is_threadl=1;


                    delete temp;
                }
        }
        num--;
    }
}

void op_tree_totum::inorder_run(){
	//TODO: travel the tree from the head node to the tail node and output the values
	//You should NOT use stack or recurtion
    if(num==0)
    {
      return;
    }
    else{
      node *p=head->right;
      while(p->right!=tail){
        cout<<p->number<<" ";
        if(p->is_threadr){
          p=p->right;
        }
        else{
          p=p->right;
          while(!p->is_threadl){
            p=p->left;
          }
        }
      }
      cout<<p->number<<endl;
    }


}

void op_tree_totum::reverseorder_run(){
	//TODO: travel the tree from the tail node to the head node and output the values
	//You should NOT use stack or recurtion
    if(num==0)
    {
      return;
    }
    else{
      node *p=tail->left;
      while(p->left!=head){
        cout<<p->number<<" ";
        if(p->is_threadl){
          p=p->left;

        }
        else{
          p=p->left;
          while(!p->is_threadr){
            p=p->right;
          }
        }
      }
      cout<<p->number<<endl;
    }

}

int op_tree_totum::size(){
	return num;
}

