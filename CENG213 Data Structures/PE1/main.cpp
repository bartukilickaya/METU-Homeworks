#include <iostream>
#include "SocialNetwork.h"
#include "Profile.h"
#include "Node.h"
using namespace std;

Post::Post() {

}

Post::Post(const std::string &message) {
    static int postId = 1;

    this->message = message;
    this->postId = postId;

    postId += 1;
}

const std::string &Post::getMessage() const {
    return this->message;
}

int Post::getPostId() const {
    return this->postId;
}

void Post::setMessage(const std::string &message) {
    this->message = message;
}

void Post::setPostId(int postId) {
    this->postId = postId;
}

std::ostream &operator<<(std::ostream &os, const Post &post) {
    os << "message: " << post.message;
    os << " postId: " << post.postId;
    return os;
}

bool Post::operator==(const Post &rhs) const {
    /* TODO */
    if(this->message == rhs.message && this->postId == rhs.postId){
        return true;
    }
    return false;

}

bool Post::operator!=(const Post &rhs) const {
    return !(rhs == *this);
}

bool Post::operator<(const Post &rhs) const {
    /* TODO */
    if(this->postId < rhs.postId){
        return true;
    }
    return false;
}

bool Post::operator>(const Post &rhs) const {
    return rhs < *this;
}

bool Post::operator<=(const Post &rhs) const {
    return !(rhs < *this);
}

bool Post::operator>=(const Post &rhs) const {
    return !(*this < rhs);
}

Profile::Profile() {

}

Profile::Profile(const std::string &firstname, const std::string &lastname, const std::string &email) {
    this->firstname = firstname;
    this->lastname = lastname;
    this->email = email;
}

const std::string &Profile::getFirstname() const {
    return this->firstname;
}

const std::string &Profile::getLastname() const {
    return this->lastname;
}

const std::string &Profile::getEmail() const {
    return this->email;
}

LinkedList<Profile *> &Profile::getFriends() {
    return this->friends;
}

LinkedList<Post *> &Profile::getLikes() {
    return this->likes;
}

LinkedList<Post *> &Profile::getPosts() {
    return this->posts;
}

void Profile::setFirstname(const std::string &firstname) {
    this->firstname = firstname;
}

void Profile::setLastname(const std::string &lastname) {
    this->lastname = lastname;
}

void Profile::setEmail(const std::string &email) {
    this->email = email;
}

void Profile::addFriend(Profile *profile) {
    this->friends.insertAtTheTail(profile);
}

void Profile::addLike(Post *post) {
    this->likes.insertAtTheTail(post);
}

void Profile::addPost(Post *post) {
    this->posts.insertAtTheTail(post);
}

void Profile::dropFriend(Profile *profile) {
    this->friends.removeNode(profile);
}

void Profile::dropLike(Post *post) {
    this->likes.removeNode(post);
}

void Profile::dropPost(Post *post) {
    this->posts.removeNode(post);
}

std::ostream &operator<<(std::ostream &os, const Profile &profile) {
    os << "firstname: " << profile.firstname;
    os << " lastname: " << profile.lastname;
    os << " email: " << profile.email;

    os << " friends: [";
    Node<Profile *> *f = profile.friends.getFirstNode();
    while (f) {
        os << f->data->getEmail();
        if (f->next) os << ", ";
        f = f->next;
    }
    os << "]";

    os << " likes: [";
    Node<Post *> *l = profile.likes.getFirstNode();
    while (l) {
        os << l->data->getPostId();
        if (l->next) os << ", ";
        l = l->next;
    }
    os << "]";

    os << " posts: [";
    Node<Post *> *p = profile.posts.getFirstNode();
    while (p) {
        os << p->data->getPostId();
        if (p->next) os << ", ";
        p = p->next;
    }
    os << "]";

    return os;
}

bool Profile::operator==(const Profile &rhs) const {
    /* TODO */
    if(this->firstname == rhs.firstname && this->lastname == rhs.lastname && this->email == rhs.email){
        return true;
    }
    return false;
}

bool Profile::operator!=(const Profile &rhs) const {
    return !(rhs == *this);
}

bool Profile::operator<(const Profile &rhs) const {
    /* TODO */
    if(this->lastname < rhs.lastname){
        return true;
    }
    else if(this->lastname == rhs.lastname){
        if(this->firstname < rhs.firstname){
            return true;
        }
    }
    return false;

}

bool Profile::operator>(const Profile &rhs) const {
    return rhs < *this;
}

bool Profile::operator<=(const Profile &rhs) const {
    return !(rhs < *this);
}

bool Profile::operator>=(const Profile &rhs) const {
    return !(*this < rhs);
}
SocialNetwork::SocialNetwork() {

}

void SocialNetwork::addProfile(const std::string &firstname, const std::string &lastname, const std::string &email) {
    /* TODO */
    Profile newprofile(firstname,lastname,email);
    profiles.insertSorted(newprofile);

}

void SocialNetwork::addPost(const std::string &message, const std::string &email) {
    /* TODO */
    posts.insertAtTheTail(Post(message));
    Node<Post> *p = posts.getLastNode();
    Node <Profile> *curr_profile = profiles.getFirstNode();
    while(curr_profile){
        if(email == curr_profile->data.getEmail()){
            curr_profile->data.getPosts().insertAtTheTail(&(p->data));
        }
        curr_profile = curr_profile->next;
    }

}


void SocialNetwork::makeFriends(const std::string &email1, const std::string &email2) {
    /* TODO */
    Node<Profile> *currprofile = profiles.getFirstNode();
    Node<Profile> *p1 = nullptr,*p2 = nullptr;
    while(currprofile){
        if(currprofile->data.getEmail() == email1){
            p1 = currprofile;
        }
        currprofile = currprofile->next;
    }
    currprofile = profiles.getFirstNode();
    while(currprofile){
        if(currprofile->data.getEmail() == email2){
            p2 = currprofile;
        }
        currprofile = currprofile->next;
    }
    if(p1 && p2){
        p1->data.addFriend(&(p2->data));
        p2->data.addFriend(&(p1->data));
    }

}

void SocialNetwork::likePost(int postId, const std::string &email) {
    /* TODO */
    Node<Post> *psts = posts.getFirstNode();
    Node<Profile> *currprofile = profiles.getFirstNode();
    while(psts){
        if(psts->data.getPostId() == postId){
            break;
        }
        psts = psts->next;
    }
    while(currprofile){
        if(currprofile->data.getEmail() == email){
            break;
        }
        currprofile = currprofile->next;
    }
    if(currprofile && psts){
        currprofile->data.addLike(&(psts->data));
    }
}

void SocialNetwork::unlikePost(int postId, const std::string &email) {
    /* TODO */
    Node<Profile> *currprofile = profiles.getFirstNode();
    while(currprofile){
        if(currprofile->data.getEmail() == email){
            break;
        }
        currprofile = currprofile->next;
    }
    if(currprofile){
        Node<Post *> *likes = currprofile->data.getLikes().getFirstNode();
        while(likes){
            if(likes->data->getPostId() == postId){
                break;
            }
            likes = likes->next;
        }
        if(likes){
            currprofile->data.dropLike(likes->data);
        }
    }
}

LinkedList<Profile *> SocialNetwork::getMutualFriends(const std::string &email1, const std::string &email2) {
    /* TODO */
    Node<Profile> *person1 = nullptr,*person2 = nullptr,*currprofile = profiles.getFirstNode();
    LinkedList<Profile *> mutualfriends;
    while(currprofile){
        if(currprofile->data.getEmail() == email1){
            person1 = currprofile;
        }
        if(currprofile->data.getEmail() == email2){
            person2 = currprofile;
        }
        currprofile = currprofile->next;
    }
    if(person1 && person2){
        Node <Profile *> *friends1 = person1->data.getFriends().getFirstNode();
        Node <Profile *> *friends2 = person2->data.getFriends().getFirstNode();
        while(friends1){
            friends2 = person2->data.getFriends().getFirstNode();
            while(friends2){
                if(friends1->data == friends2->data){
                    mutualfriends.insertAtTheTail(friends1->data);
                }
                friends2 = friends2->next;
            }
            friends1 = friends1->next;
        }
    }
    return mutualfriends;
}

LinkedList<Post *> SocialNetwork::getListOfMostRecentPosts(const std::string &email, int k) {
    /* TODO */
    LinkedList<Post *> recentposts;
    Node <Profile> *currprofile = profiles.getFirstNode();
    while(currprofile){
        if(currprofile->data.getEmail() == email){
            break;
        }
        currprofile = currprofile->next;
    }
    if(currprofile){
        Node <Post *> *psts = currprofile->data.getPosts().getLastNode();
        for(int i = 0 ;psts && (i < k); i++){
            recentposts.insertAtTheTail(psts->data);
            psts = psts->prev;
        }
    }
    return recentposts;
}

void SocialNetwork::print() const {
    std::cout << "# Printing the social network ..." << std::endl;

    std::cout << "# Number of profiles is " << profiles.getNumberOfNodes() << ":" << std::endl;
    profiles.print();

    std::cout << "# Number of posts is " << posts.getNumberOfNodes() << ":" << std::endl;
    posts.print();

    std::cout << "# Printing is done." << std::endl;
}
void SocialNetwork::deleteProfile(const std::string &email) {
    Node<Profile> *currprofile = profiles.getFirstNode();
    while(currprofile){
        if(currprofile->data.getEmail() == email){
            break;
        }
        currprofile = currprofile->next;
    }
    if(currprofile){
        Node<Profile *> *frnds = currprofile->data.getFriends().getFirstNode();
        while(frnds){
            frnds->data->dropFriend(&(currprofile->data));
            frnds = frnds->next;
        }
        Node <Profile> *others = profiles.getFirstNode();
        Node <Post *> *likess;
        Node <Post *> *psts;
        Node <Post *> *tmp;
        while(others){
            if(others->data == currprofile->data){
                others = others->next;
            }
            if(others){
                psts = currprofile->data.getPosts().getFirstNode();
                while(psts){
                    likess = others->data.getLikes().getFirstNode();
                    while(likess){
                        if(psts->data == likess->data){
                            others->data.dropLike(likess->data);
                            break;
                        }
                        likess = likess->next;
                    }
                    psts = psts->next;
                }
                others = others->next;
            }
            else{
                break;
            }
        }
        psts = currprofile->data.getPosts().getFirstNode();
        Node <Post> *allposts = posts.getFirstNode(),*temp;
        while(psts){
            allposts = posts.getFirstNode();
            while(allposts){
                if(&allposts->data == psts->data){
                    temp = allposts;
                    allposts = allposts->next;
                    posts.removeNode(temp);
                }
                else{
                    allposts = allposts->next;
                }
            }
            psts = psts->next;
        }
        profiles.removeNode(currprofile->data);
    }
}
int main() {
    SocialNetwork snetwork;

    //snetwork.print();
    /*int a[] = {1,2,3,4,5};
    LinkedList<int> p(a,5);
    p.print();
    p.insertSorted(8);
    p.print();
    p.insertSorted(7);
    p.print();*/
    snetwork.addProfile("john", "doe", "1");
    snetwork.addProfile("bartu","klc","2");
    snetwork.addProfile("bartu","k","3");
    snetwork.addProfile("bartu","c","4");
    snetwork.addProfile("bartu","kl","5");
    snetwork.addProfile("bartu","lc","6");
    //snetwork.print();
    snetwork.makeFriends("1","6");
    snetwork.makeFriends("3","6");
    snetwork.addPost("merhaba","5");
    snetwork.addPost("merhabaa","6");
    snetwork.addPost("merhabaa","6");
    snetwork.likePost(1,"6");
    snetwork.likePost(1,"4");
    snetwork.likePost(1,"3");
    snetwork.likePost(2,"1");
    snetwork.likePost(2,"3");
    snetwork.likePost(3,"2");
    snetwork.likePost(3,"1");
    snetwork.likePost(3,"5");
    snetwork.likePost(3,"4");

    snetwork.deleteProfile("6");
    //snetwork.addPost("merhaba","john@doe.com");
    //snetwork.likePost(1,"johndoe.com");
    //snetwork.getMutualFriends("1","2") << endl;
    //snetwork.addPost("Hello world! My name is John Doe.", "john@doe.com");


    snetwork.print();

    return 0;
}
