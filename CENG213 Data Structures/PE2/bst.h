#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include <functional>
#include <cstddef>
#include <ostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <cmath>
#include <utility>
#include <iostream>   // you may use it for debugging your code
using namespace std;

class DefaultBalanceCondition    // default BST, do not change
{
  public:
    bool operator() (int current_height, int ideal_height) const
    {
      return true;
    }
};


template <typename Key, typename Object,
          typename BalanceCondition=DefaultBalanceCondition,
	  typename Comparator=std::less<Key> >
class BinarySearchTree
{
  public:  /* DO NOT CHANGE */
    struct Node   // core structure of BinarySearchTree
    {
       Key key;         // unique key
       Object data;     // data to be stored, characterized by key
       Node * left;     // pointer to left subtree
       Node * right;    // pointer to right subtree
       size_t height;   // height of the node
       size_t subsize;  // size of the subtree node roots including itself

       Node(const Key &, const Object &, 
            Node *, Node *, size_t =0, size_t =1);   // Node constructor
    };

  public:  /* DO NOT CHANGE PROTOTYPES */
    BinarySearchTree();  // zero-parameter constructor
    BinarySearchTree(const std::list<std::pair<Key, Object> > &); // list is always sorted!
    ~BinarySearchTree();  // destructor

  public:  /* DO NOT CHANGE PROTOTYPES */
    void insert(const Key &, const Object &);  // insert new Key-Object
    void remove(const Key &);  // remove Node characterized by Key
    void toCompleteBST();  // convert into a complete BST

  public:  /* DO NOT CHANGE PROTOTYPES */
    Node * find(const Key &) const; // single item
    std::list<Node *> find(const Key &, const Key &) const; // range queries
    int height() const;  // return the height of the tree
    size_t size() const; // return the number of items in the tree
    bool empty() const;  //return whether the tree is empty or not
    Node * getRoot() const; // return a pointer to the root of the tree
    void print(std::ostream &) const;  // print tree structure into an output stream

  private:  /* DO NOT CHANGE DATA TYPES AND IDENTIFIERS */
    Node * root;                     // designated root
    size_t numNodes;                 // size
    Comparator isLessThan;           // operator upon which BST nodes are arranged
    BalanceCondition isBalanced;     // signals whether the signal is balanced
  
  private:  /* DO NOT CHANGE PROTOTYPES */
    Node * find(const Key &, Node *) const;
    int height(Node *) const;
    int subsize(Node *) const;
    void print(Node *, std::ostream &) const;
    Node *helperBST(Key *keyvalues,Object *objvalues,int,int);
    int helperheight(Node *);
    int helpersubsize(Node *);
    Node * helpertoComplete(Node **,Node *,int start,int end);
    void helperinsert(Node *&,Node *);
    Node *helperremove(const Key & k,Node *&curr,int);
    Node **findmin(Node *&);


    // Define your const private utility functions below this line

  private:
    void makeEmpty(Node * &);       // utility for destructor
    
    // Define your private utility functions below this line


  private: /* DO NOT CHANGE PROTOTYPES: compiler defaults are blocked */  
    BinarySearchTree(const BinarySearchTree &);
    const BinarySearchTree & operator=(const BinarySearchTree &);

  private:  // static utility functions
    template <typename T> //static utility function
    static const T & max(const T &, const T &);

    // Define more below this line if needed

};

#endif


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::Node::
Node(const K & _k, const O & _d, Node * _l, Node * _r, size_t _h, size_t _s)
  : key(_k), data(_d), left(_l), right(_r), height(_h), subsize(_s)
{
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::BinarySearchTree()
  : root(NULL), numNodes(0)  //comparator(C() ??)
{
}


// IMPLEMENT

template <typename K, typename O, typename B, typename C>
int BinarySearchTree<K,O,B,C>::helpersubsize(Node *rooot){
    if(rooot == NULL){
        return 0;
    }
        return 1 + helpersubsize(rooot->left) + helpersubsize(rooot->right);


}
template <typename K, typename O, typename B, typename C>
int BinarySearchTree<K,O,B,C>::helperheight(Node *rooot) {
    if(rooot == NULL){
        return -1;
    }
    int lt = helperheight(rooot->left);
    int rt = helperheight(rooot->right);
    if(lt > rt){
        return 1 + lt;
    }
    else{
        return 1 + rt;
    }
}

template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::helperBST(K *keyvalues,O *objvalues,int start,int end){
    if(start > end){
        return NULL;
    }
    //cout << "asd";
    int mid,size = end - start + 1,height,lower,higher;
    height = (log2(size)),lower = pow(2,height -1) -1,higher = pow(2,height) -1;
    if(lower + higher < size){
        mid = higher + start;
    }
    else{
        mid = size - lower + start -1;
    }
    //cout << mid << " " << start << " " << end << " " << height << endl;
    Node *rooot = new  Node(keyvalues[mid],objvalues[mid],NULL,NULL,0,1);
    //rooot->key = keyvalues[mid];
    //rooot->data = objvalues[mid];
    rooot->left = helperBST(keyvalues,objvalues,start,mid-1);
    rooot->right = helperBST(keyvalues,objvalues,mid+1,end);
    rooot->height = helperheight(rooot);
    rooot->subsize = helpersubsize(rooot);

    return rooot;
}
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::BinarySearchTree(const std::list<std::pair<K,O> > & datalist)
   : root(NULL), numNodes(0)  // change it as you'd like
{
       //cout << "1" << endl;
    typename std::list<std::pair<K,O> >::const_iterator itr = datalist.begin();
    int size = datalist.size(),i = 0;
    K *keyvalues = new K[size];
    O *objvalues = new O[size];
    for ( ; itr != datalist.end(); ++itr)
    {
        keyvalues[i] = itr->first;
        objvalues[i] = itr->second;
        //cout << keyvalues[i] << " " << objvalues[i] << endl;
        i++;
    }
    numNodes = size;
    root = helperBST(keyvalues,objvalues,0,size-1);
    //cout << root->left->data << endl;
    //cout << root->key << " " << root->data << " " << root->subsize << " " << root->height;
    delete [] keyvalues;
    delete [] objvalues;
    //makeEmpty(root);
}
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::helpertoComplete(Node **arry,Node *curr,int start,int end){
    if(start > end){

        return NULL;
    }
    int mid,size = end - start + 1,height,lower,higher;
    height = (int)(log2(size)),lower = pow(2,height -1) -1,higher = pow(2,height) -1;
    if(lower + higher < size){
        mid = higher + start;
    }
    else{
        mid = size - lower + start -1;
    }
    //cout << mid << ":" << arry[mid]->data << ":" << root->data << endl;
    curr = arry[mid];
    curr->left == NULL;
    curr->right == NULL;
    curr->left = helpertoComplete(arry,curr->left,start,mid-1);
    curr->right = helpertoComplete(arry,curr->right,mid+1,end);
    curr->height = helperheight(curr);
    curr->subsize = helpersubsize(curr);
    return curr;

}
// IMPLEMENT
template <typename K, typename O, typename B, typename C>
void BinarySearchTree<K,O,B,C>::toCompleteBST(){
    stack<Node*> stack;
    Node *curr = root;
    int i = 0;
    Node **arry = new Node*[root->subsize];
    while (!stack.empty() || curr != nullptr)
    {
        if (curr != nullptr){

            stack.push(curr);
            curr = curr->left;
        }
        else{
            curr = stack.top();
            stack.pop();
            arry[i] = curr;
            i++;
            curr = curr->right;
        }
    }

    root = helpertoComplete(arry,root,0,root->subsize-1);
    delete[] arry;
}
template <typename K, typename O, typename B, typename C>
void BinarySearchTree<K,O,B,C>::helperinsert(Node *&p,Node *curr){
    if(curr == NULL){
        return ;
    }
    else{
        if(isLessThan(curr->key,p->key)){
            (curr->subsize)++;
            helperinsert(p,curr->right);
        }
        else if(isLessThan(p->key,curr->key)){
            (curr->subsize)++;
            helperinsert(p,curr->left);
        }
        else{
            return ;
        }
    }
    curr->height = helperheight(curr);
    root = curr;
    //cout << log2(root->subsize) << ":"<< root->height << ":" << root->key << ":" << root->subsize << endl;
    //cout << root->key << endl;
    //cout << isBalanced(root->right->height,(int)log2(root->right->subsize)) << endl;
    //print(cout);
    if(numNodes == 2 && !isBalanced(root->height,(int)log2(root->subsize))){
        //cout << (int)log2(root->subsize) << ":"<< root->height << ":" << root->key << ":" << root->subsize << "b" << endl;
        toCompleteBST();
    }
    else if(root->right && root->right->subsize > 1 && !isBalanced(root->right->height,(int)log2(root->right->subsize))){
        //cout << (int)log2(root->subsize) << ":"<< root->height << ":" << root->key << ":" << root->subsize << "asd" << endl;
        root = curr->right;
        //cout << root->key << endl;
        //cout << curr->key << endl;
        toCompleteBST();
        curr->right = root;
        root = curr;
        //cout << root->key << endl;
        //cout << curr->key << endl;
        //cout << root->left->key << endl;
        //cout << root->right->key << endl;
    }
    else if(root->left && root->left->subsize> 1 && !isBalanced(root->left->height,(int)log2(root->left->subsize))){
        //cout << (int)log2(root->subsize) << ":"<< root->height << ":" << root->key << ":" << root->subsize << endl;
        root = curr->left;
        //print(cout);
        toCompleteBST();
        //print(cout);
        curr->left = root;
        root = curr;
    }

    //(curr->subsize)++;
    //cout << curr->key << endl;
    curr->height = helperheight(curr);
    //curr->subsize = helpersubsize(curr);
    //cout << curr->key << ":" << curr->subsize<< endl;

    //cout << root->key;
}
// IMPLEMENT
template <typename K, typename O, typename B, typename C>
void BinarySearchTree<K,O,B,C>::insert(const K & k, const O & x){
    if(root == NULL){
        root = new Node(k,x,NULL,NULL,0,1);
        numNodes++;
        return ;
    }
    Node *temp;
    if((temp =find(k)) != NULL){
        temp->data = x;
    }
    else{
        temp = new Node(k,x,NULL,NULL,0,1);
        numNodes++;
        Node *curr = root;
        //root->right->right->right = temp;
        while(curr){
            if(isLessThan(curr->key,temp->key) && curr->right){
                curr = curr->right;
            }
            else if(isLessThan(temp->key,curr->key) && curr->left){
                curr = curr->left;
            }
            else{
                break;
            }
        }
        if(isLessThan(temp->key,curr->key)){
            curr->left = temp;
        }
        else{
            curr->right = temp;
        }
        helperinsert(temp,root);
        if(!isBalanced(root->height,(int)log2(root->subsize))){
            toCompleteBST();
        }
        //toCompleteBST();
    }
}
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node ** BinarySearchTree<K,O,B,C>::findmin(Node *&min){
    if(min->left == NULL){
        return &min;
    }
    else{
        return findmin(min->left);
    }

}
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node * BinarySearchTree<K,O,B,C>::helperremove(const K & k,Node *&curr,int flag){
    if(!flag && curr == NULL){
        return NULL;
    }
    else if(!flag && curr->left == NULL){
        //cout << curr->key << endl;
        //3curr = NULL;
    }
    else if(!flag && curr->left){
        helperremove(k,curr->left,0);
    }
    if(flag && (curr == NULL)){
        //cout << root->key << endl;
        return NULL;
    }
    else if(flag && isLessThan(curr->key,k)){
        //cout << curr->key << ":"  <<endl;
        helperremove(k,curr->right,1);
        //print(cout);
    }
    else if(flag && isLessThan(k,curr->key)){
        helperremove(k,curr->left,1);
    }
    else if(flag && !isLessThan(curr->key,k) && !isLessThan(k,curr->key)){ // k = curr->key
        //cout << curr->key << "er"<<endl;
        if(curr->left == NULL && curr->right == NULL){
            //cout << curr->key << ":" << endl;
            Node *tmp = curr;
            curr = NULL;
            delete tmp;
            numNodes--;
        }
        else if(curr->left && curr->right == NULL){
            Node *tmp = curr;
            curr = curr->left;
            delete tmp;
            numNodes--;
        }
        else if(curr->left == NULL && curr->right){
            Node *tmp = curr;
            curr = curr->right;
            delete tmp;
            numNodes--;
        }
        else{
            //cout << curr->key << ":" << endl;
            flag = 0;
            Node **lowest = (findmin(curr->right));
            //cout << *lowest << ":" << *(findmin(curr->right)) << ":" << curr->right->left->left <<  endl;
            Node *tmp = curr;
            Node *tmp2 = (*lowest)->right;
            //cout << curr->key << ":" << (*lowest)->key << ":" << endl;
            (*lowest)->left = curr ->left;
            //cout << lowest->left->key << endl;
            if( isLessThan(curr->right->key,(*lowest)->key) || isLessThan((*lowest)->key,curr->right->key)){   //curr->right->key != (*lowest)->key)
                (*lowest)->right = curr->right;
            }
            /*else{
                (*lowest)->right = NULL;
            }*/
            //cout << lowest->right->key << endl;
            curr = *lowest;
            //cout << (*lowest)->key << endl;
            *lowest = tmp2;
            //cout << curr->right->left->left->key << endl;
            //cout << lowest->key << endl;
            //cout << curr->key << endl;
            //cout << tmp->key << endl;
            delete tmp;
            numNodes--;
            //cout << curr->right->key << endl;
            //cout << curr->key << ":" << endl;
            helperremove(k,curr->right,0);
            //cout << curr->key << "::" << endl;
            //cout << root->left->key << ":" << endl;
        }
    }
    if(curr){
        //print(cout);
        //cout << root->key << "a" << endl;
        //cout << curr->right << "::" << endl;
        //cout << "asd" << endl;
        curr->height = helperheight(curr);
        curr->subsize = helpersubsize(curr);
        root = curr;
        //cout << curr->key << "q" << endl;
        if(numNodes == 2 && !isBalanced(curr->height,(int)log2(curr->subsize))){
            //cout << curr->key << "a" << endl;
            root = curr;
            //cout << (int)log2(root->subsize) << ":"<< root->height << ":" << root->key << ":" << root->subsize << "b" << endl;
            toCompleteBST();
        }
        else if(curr->right && curr->right->subsize > 1 && !isBalanced(curr->right->height,(int)log2(curr->right->subsize))){
            //cout << curr->key << "b" << endl;
            //cout << (int)log2(root->subsize) << ":"<< root->height << ":" << root->key << ":" << root->subsize << "c" << endl;
            root = curr->right;
            toCompleteBST();
            curr->right = root;
            root = curr;
        }
        else if(curr->left && curr->left->subsize> 1 && !isBalanced(curr->left->height,(int)log2(curr->left->subsize))){
            //cout << curr->key << "c" << endl;
            //cout << (int)log2(root->subsize) << ":"<< root->height << ":" << root->key << ":" << root->subsize <<"d" << endl;
            root = curr->left;
            //print(cout);
            toCompleteBST();
            //print(cout);
            curr->left = root;
            root = curr;
        }
        curr->height = helperheight(curr);
        //print(cout);
    }
}

// IMPLEMENT
template <typename K, typename O, typename B, typename C>
void BinarySearchTree<K,O,B,C>::remove(const K & k){
    if(root == NULL){
        return ;
    }
    else if(!isLessThan(root->key,k) && !isLessThan(k,root->key) && numNodes == 1){
        //cout << "asd" << endl;
        delete root;
        root = NULL;
        numNodes--;
    }
    else {
        Node *dummyroot = root;
        //dummyroot->left = dummyroot->right = root;
        helperremove(k, dummyroot,1);
        if(!isBalanced(root->height,(int)log2(root->subsize))){
            //print(cout);
            //cout << dummyroot->left->key;
            toCompleteBST();
            //print(cout);
            root->height = helperheight(root);
        }
        //root = dummyroot->left;
        //delete dummyroot;
    }

}



// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::find(const K & key) const
{
  return find(key, root);
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::find(const K & key, Node * t) const
{
  if (t == NULL)
  {
    return NULL;
  }
  else if (isLessThan(key, t->key))
  {
    return find(key, t->left);
  }
  else if (isLessThan(t->key, key))
  {
    return find(key, t->right);
  }
  else //found
  {
    return t;
  }
}


// IMPLEMENT

template <typename K, typename O, typename B, typename C>
std::list<typename BinarySearchTree<K,O,B,C>::Node *>
BinarySearchTree<K,O,B,C>::find(const K & lower, const K & upper) const
{
  std::list<Node *> ranges;
    stack<Node *> stack;
    Node *curr = root;
    while (!stack.empty() || curr != NULL)
    {
        if(curr != NULL && (curr->key >= lower)){
            if(curr->key <= upper){
                stack.push(curr);
            }
            curr = curr->left;
        }
        else if(curr != NULL && curr->key < lower){
            curr = curr->right;
        }
        else{
            if(!stack.empty()){
                curr = stack.top();
                ranges.push_back(curr);
                stack.pop();
                curr = curr->right;
            }

        }
    }
  return ranges;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::getRoot() const
{
  return root;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::height() const
{
  return height(root);
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::height(Node * t) const
{
  return (t == NULL) ? -1 : t->height;
}


// private utility
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::subsize(Node * t) const
{
  return (t == NULL) ? 0 : t->subsize;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
size_t
BinarySearchTree<K,O,B,C>::size() const
{
  return numNodes;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
bool
BinarySearchTree<K,O,B,C>::empty() const
{
  return numNodes == 0;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::makeEmpty(Node * & t)
{
  if (t != NULL)
  {
      //cout << "asd" << endl;
    makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;

    --numNodes;
  }
  
  t = NULL;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::~BinarySearchTree()
{
  makeEmpty(root);
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::print(std::ostream & out) const
{
  print(root, out);
  out << '\n';
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::print(Node * t, std::ostream & out) const
{
  if (t != NULL && t->left != NULL) 
  {
    out << '[';
    print( t->left, out );
  }
  else if (t != NULL && t->left == NULL && t->right != NULL)
  {
    out << "[";
  }

  if (t != NULL)
  {
    if (t->left == NULL && t->right == NULL)
    {
      out << '(' << (t->key) << ')';
    }
    else if (t->left != NULL || t->right != NULL)
    {
      out << '{' << (t->key) << ",H" << t->height << ",S" << t->subsize << '}';
    }
  }
  
  if (t != NULL && t->right != NULL)
  {
    print( t->right, out );
    out << ']';
  }
  else if (t != NULL && t->left != NULL && t->right == NULL)
  {
    out << "]";
  }
}


// do not change
template <typename K, typename O, typename B, typename C>
template <typename T>
const T &
BinarySearchTree<K,O,B,C>::max(const T & el1, const T & el2)
{
  return el1 > el2 ? el1 : el2;
}

