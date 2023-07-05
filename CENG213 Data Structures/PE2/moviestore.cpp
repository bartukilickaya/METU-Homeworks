#include "moviestore.h"
//#include "movie.h"
//#include "bst.h"


MovieStore::MovieStore( ) //implemented, do not change
{
}


// IMPLEMENT
void MovieStore::insert(const Movie & movie){
    primaryIndex.insert(movie.getID(),movie);
    secondaryIndex.insert(SKey(movie),&primaryIndex.find(movie.getID())->data);
    ternaryIndex.insert(SKey(movie),&primaryIndex.find(movie.getID())->data);
}


// IMPLEMENT
void MovieStore::remove(const std::string & id){
    MSTP::Node *p = primaryIndex.find(id);
    //cout << p->data.getDirector() << endl;
    if(p){
        secondaryIndex.remove(SKey(p->data));
        //secondaryIndex.remove(SKey(p->data.getTitle(),p->data.getDirector()));
        ternaryIndex.remove(SKey(p->data.getTitle(),p->data.getDirector()));
        primaryIndex.remove(id);
    }
}


// IMPLEMENT
void MovieStore::remove(const std::string & title,
                  const std::string & director) {
    MSTS::Node *p = secondaryIndex.find(SKey(title,director));
    remove(p->data->getID());
}


// IMPLEMENT
void MovieStore::removeAllMoviesWithTitle(const std::string & title) {
    MSTT:: Node *tmp;
    MSTT:: Node *curr = ternaryIndex.getRoot();
    stack<MSTT::Node *> s;
    MSTT:: Node **p = new MSTT::Node *[ternaryIndex.size()];
    int i = 0;
    while (!s.empty() || curr != nullptr)
    {
        if (curr != nullptr && title == curr->data->getTitle()){
            s.push(curr);
            curr = curr->left;
        }
        else if(curr != nullptr && curr->data->getTitle() > title){
            curr = curr->left;
        }
        else if(curr != nullptr && curr->data->getTitle() < title){
            curr = curr->right;
        }
        else{
            curr = s.top();
            s.pop();
            p[i] = curr;
            i++;
            curr = curr->right;
            //remove(tmp->data->getID());
        }
    }
    for(int j = 0 ; j < i; j++){
        remove(p[j]->data->getID());
    }
    delete []p;
}


// IMPLEMENT
void MovieStore::makeAvailable(const std::string & id){
MSTP::Node *p = primaryIndex.find(id);
if(p){
    p->data.setAvailable();
}
}


// IMPLEMENT
void MovieStore::makeUnavailable(const std::string & title,
                       const std::string & director){
MSTS::Node *p = secondaryIndex.find(SKey(title,director));
MSTP::Node *p2 = primaryIndex.find(p->data->getID());
if(p2){
    p2->data.setUnavailable();
}
}


// IMPLEMENT
void MovieStore::updateCompany(const std::string & director,
                       const std::string & Company){
MSTS::Node *curr = secondaryIndex.getRoot();
stack<MSTS::Node *> s;
while (!s.empty() || curr != nullptr)
{
    if(curr){
    }
    if (curr != nullptr && director == curr->data->getDirector()){
        s.push(curr);
        curr = curr->left;
    }
    else if(curr != nullptr && curr->data->getDirector() > director){
        curr = curr->left;
    }
    else if(curr != nullptr && curr->data->getDirector() < director){
        curr = curr->right;
    }
    else{
        curr = s.top();
        //cout << curr->data->getDirector() << ":" << endl;
        s.pop();
        primaryIndex.find(curr->data->getID())->data.setCompany(Company);
        curr = curr->right;
    }
}
}

// IMPLEMENT
void MovieStore::printMoviesWithID(const std::string & id1,
                          const std::string & id2,
                          unsigned short since) const {
MSTP::Node *curr = primaryIndex.getRoot();
stack<MSTP::Node *> s;
while (!s.empty() || curr != nullptr)
{
    if (curr != nullptr && curr->data.getID() <= id2 && curr->data.getID() >= id1){
        s.push(curr);
        curr = curr->left;
    }
    else if(curr != nullptr && curr->data.getID() > id2){
        curr = curr->left;
    }
    else if(curr != nullptr && curr->data.getID() < id1){
        curr = curr->right;
    }
    else{
        curr = s.top();
        s.pop();
        if(curr->data.getYear() >= since){
            cout << curr->data << endl;
        }
        curr = curr->right;
    }
}

}


// IMPLEMENT
void MovieStore::printMoviesOfDirector(const std::string & director,
                              const std::string & first,
                              const std::string & last) const {
    MSTS::Node *curr = secondaryIndex.getRoot();
    stack<MSTS::Node *> s;
    char first2[100] = "\0";
    char last2[100] = "\0";
    char director2[100] = "\0";
    char directortmp[100] = "\0";
    char title1[100] = "\0";
    int i;
    for(i = 0 ; i < first.length();i++){
        if(first[i] <97){
            first2[i] = first[i] + 32;
        }
        else{
            first2[i] = first[i];
        }
    }
    first2[i] = '\0';
    for(i = 0 ; i < last.length();i++){
        if(last[i] <97){
            last2[i] = last[i] + 32;
        }
        else{
            last2[i] = last[i];
        }
    }
    last2[i] = '\0';
    for(i = 0 ; i < director.length();i++){
        if(director[i] <97){
            director2[i] = director[i] + 32;
        }
        else{
            director2[i] = director[i];
        }
    }
    director2[i] = '\0';
    while (!s.empty() || curr != nullptr) {
        if(curr){
            for(i = 0 ; i < curr->data->getDirector().length();i++){
                if(curr->data->getDirector()[i] < 97){
                    directortmp[i] = curr->data->getDirector()[i] + 32;
                }
                else{
                    directortmp[i] = curr->data->getDirector()[i];
                }
            }
            directortmp[i] = '\0';
        }
        if (curr != NULL && ((strcmp(directortmp, director2) == 0))) {
            s.push(curr);
            curr = curr->left;
        }
        else if (curr != NULL && (strcmp(directortmp,director2) >= 1)) {
            curr = curr->left;
        } else if (curr != NULL && (strcmp(director2,directortmp) >= 1)) {
            curr = curr->right;
        } else {
            curr = s.top();
            for(i = 0 ; i < curr->data->getTitle().length();i++){
                if(curr->data->getTitle()[i] < 97){
                    title1[i] = curr->data->getTitle()[i] + 32;
                }
                else{
                    title1[i] = curr->data->getTitle()[i];
                }
            }
            title1[i] = '\0';
            if(strcmp(title1,first2) >= 0 && strcmp(last2,title1) >= 0){
                cout << *(curr->data) << ":::" <<endl;
            }
            s.pop();
            curr = curr->right;
        }
    }
}



void  // implemented, do not change
MovieStore::printPrimarySorted( ) const
{
  printPrimarySorted(primaryIndex.getRoot());
}


void  // implemented, do not change
MovieStore::printPrimarySorted(MSTP::Node * t) const
{
  if (t == NULL)
  {
    return;
  }

  printPrimarySorted(t->left);
  std::cout << t->data << std::endl;
  printPrimarySorted(t->right);
}


void  // implemented, do not change
MovieStore::printSecondarySorted( ) const
{
  printSecondarySorted(secondaryIndex.getRoot());
}


void  // implemented, do not change
MovieStore::printSecondarySorted(MSTS::Node * t) const
{
  if (t == NULL)
  {
    return;
  }

  printSecondarySorted(t->left);
  std::cout << *(t->data) << std::endl;
  printSecondarySorted(t->right);
}


void  // implemented, do not change
MovieStore::printTernarySorted( ) const
{
  printTernarySorted(ternaryIndex.getRoot());
}


void  // implemented, do not change
MovieStore::printTernarySorted(MSTT::Node * t) const
{
  if (t == NULL)
  {
    return;
  }

  printTernarySorted(t->left);
  std::cout << *(t->data) << std::endl;
  printTernarySorted(t->right);
}

