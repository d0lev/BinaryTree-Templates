#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <stack>
#include <queue>


namespace ariel {


template <class T>
class BinaryTree {   
    

    protected:
        // inner class that represent a node from the binary tree 
         class node {
    
            public:
            T data;
            node* left; 
            node* right;
            node(T value):data(value),left(nullptr),right(nullptr){}
            void set_data(T value) {this->data = value;}
            T & get_data(){return this->data;}
            node* get_right(){return this->right;}
    };

    private:
    node* root;

    public:

    BinaryTree():root(nullptr){}

    BinaryTree(const BinaryTree & from) {
        root = new node(from.root->get_data());
        create_copy(*root,*(from.root));
    }
   
    void create_copy(node & source , node & dest) {
        if(dest.left) {
            source.left = new node(dest.left->get_data());
            create_copy(*source.left,*dest.left);
        }
         if(dest.right) {
            source.right = new node(dest.right->get_data());
            create_copy(*source.right,*dest.right);
        }
    }

     BinaryTree & operator = (const BinaryTree & from) {
        if (this == &from) {return *this;}
        delete(this->get_root());
        root = new node(from.root->get_data());
        create_copy(*root, *from.root);
        return *this;
    }
    BinaryTree & operator = (BinaryTree &&other) noexcept {
        root = other->get_root;
        other.root = nullptr;
        return *this;
    }

    BinaryTree(BinaryTree &&other) noexcept {
            root = other.root;
            other.root = nullptr;
    }


    ~BinaryTree() {del(root);}
    
    void del(node* root) {
        if(!root){return;}
        del(root->left);
        del(root->right);
        delete root; 
        root = nullptr;
    }



    BinaryTree & add_root(T data) {
        if(!root) {root = new node(data);}
        root->set_data(data);
        return *this;
    }

    node* find_by_key(node *root ,T key) {
        if(!root) {return nullptr;}
        if(root->get_data() == key) {return root;}
        // find the key in the leftsubtree
        node* found = find_by_key(root->left,key);
        if(found) {return found;}
        // otherwise find he key in the rightsubtree
        return find_by_key(root->right,key);
        
    }

    BinaryTree & add_left(const T & parent , const T & child) {
        if(!root) {throw std::invalid_argument{"The tree is empty"};}
        // find the parent of the candiate child
        node* found = find_by_key(root,parent);
        if (!found) {throw std::invalid_argument{"The parent not found"};}
        if (found->left == nullptr) {found->left = new node(child); return *this;}
        found->left->set_data(child);
        return *this;
    }
    BinaryTree & add_right(const T & parent , const T & child) {
       if(!root) {throw std::invalid_argument{"The tree is empty"};}
        // find the parent of the candiate child
        node* found = find_by_key(root,parent);
        if (!found) {throw std::invalid_argument{"The parent not found"};}
        if (found->right == nullptr) {found->right = new node(child); return *this;}
        found->right->set_data(child);
        return *this;
    }


   class inorder {   
        
        public:
        std::stack<node*> path;
        std::queue<node*> order;
        node* current;

       inorder(node* pos) {
        bool flag = true;
        if (pos) {
        current = pos;
        while(flag) {
           if(current) {
               path.push(current);
               current = current->left;
           }
           else {
                if(path.empty()) {break;}
                current = path.top();
                order.push(current);
                current = current->right;
                path.pop();
           }
       }
       current = order.front();
    }else {current = nullptr;}

 }

   inorder & operator ++ () { // ++ it
     order.pop();
     if (order.empty()) {current = nullptr; return *this;}
     current = order.front();
     return *this;
   }
    inorder operator ++ (int) { //it ++
    inorder temp = *this;
    order.pop();
    if (order.empty()) {current = nullptr; return temp;}
    current = order.front();
    return temp;
   }

   T* operator ->() const {return &(order.front()->get_data());}
   T & operator *() const {return current->get_data();}
   bool operator == (const inorder & other) const {return (this->current == other.current);}			
   bool operator != (const inorder & other) const {return this->current != other.current;}

};

   inorder begin_inorder() {
        return inorder(root);
   }

   inorder end_inorder() {
        return (nullptr);
   }

   inorder begin() {
        return inorder(root);
   }

   inorder end() {
        return (nullptr);
   }

    class preorder {   
        public:
        std::stack<node*> path;
        std::queue<node*> order;
        node* current;

       preorder(node* pos) {
        if (pos) {
        current = pos;
        path.push(current);
        while((!path.empty())) {
           current = path.top();
           order.push(current);    
           path.pop();
           current = order.back();
           if(current->right){path.push(current->right);}
           if(current->left){ path.push(current->left);}
        } 
        current = order.front();
    } else {current = nullptr;}

}

    preorder & operator ++ () {
    order.pop();
    if (order.empty()) {current = nullptr; return *this;}
    current = order.front();
    return *this;
   }
    preorder operator ++ (int) {
       preorder temp = *this;
       order.pop();
       if (order.empty()) {current = nullptr; return temp;}
       current = order.front();
       return temp;
   }
   T* operator ->() const {return &(order.front()->get_data());}
   T & operator *() const {return current->get_data();}
   bool operator == (const preorder & other) const {return current == other.current;}			
   bool operator != (const preorder & other) const {return current != other.current;}


    };
   
  preorder begin_preorder() {
        return preorder(root);
   }

   preorder end_preorder() {
        return (nullptr);
}

    class postorder {   
        
        public:
        std::queue<node*> order;
        node* current;

       postorder(node* pos){
       if(pos) {
       std::stack<node*> s1;
       std::stack<node*> s2;
       current = pos;
       s1.push(current);
       while(!s1.empty()) {
           s2.push(s1.top());
           current = s2.top();
           s1.pop();
           if(!current){break;}
           if(current->left){s1.push(current->left);}
           if(current->right){s1.push(current->right);}
       }
       while(!s2.empty()) {
           current = s2.top();
           order.push(current);
           current = order.back();
           s2.pop();
       }
       current = order.front();
       } else {current = nullptr;}
    }

   postorder & operator ++ () {
    order.pop();
    if (order.empty()) {current = nullptr; return *this;}
     current = order.front();
     return *this;
   }
   postorder operator ++ (int) {
       postorder temp = *this;
       order.pop();
       if (order.empty()) {current = nullptr; return temp;}
       current = order.front();
       return temp;
   }
   T* operator ->() const {return &(order.front()->get_data());}
   T & operator *() const {return current->get_data();}
   bool operator == (const postorder & other) const {return current == other.current;}			
   bool operator != (const postorder & other) const {return current != other.current;}

    };
   
   postorder begin_postorder() {
        return postorder(root);
}

    postorder end_postorder() {
        return (nullptr);
}


    node* get_root() const {
        return this->root;
}

    int numOfChilds(node* current) const {
        int num = 0;
        if(current->left && current->right) {num = 2; return num;}
        if(current->left || current->right) {num = 1; return num;}
        return num;
    }

friend std::ostream & operator << (std::ostream & out , const BinaryTree<T> & btree) {
    node* current = btree.get_root();
    std::queue<node*> q1;
    std::queue<node*>q2;

    q1.push(current);
    int len = 0;
    while(!q1.empty() || !q2.empty()) {
        while(!q1.empty()) {
            len = q1.size();
            current = q1.front();
            out << current->get_data() << "," << "{" + to_string(btree.numOfChilds(current)) +"}" << "  ";
            q1.pop();
            if(current->left) {q2.push(current->left);}
            if(current->right) {q2.push(current->right);}
        }
        out << "\n";

        while(!q2.empty()) {
            current = q2.front();
            len = q2.size();
            out << current->get_data() << "," << "{"+ to_string(btree.numOfChilds(current)) +"}" << "  ";
            q2.pop();
            if(current->left) {q1.push(current->left);}
            if(current->right) {q1.push(current->right);}
        }
        out << "\n";
    }
    return out;
}

    
};


}