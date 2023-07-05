#include "SocialNetwork.h"

#include <iostream>

SocialNetwork::SocialNetwork() {

}

void SocialNetwork::addProfile(const std::string &firstname, const std::string &lastname, const std::string &email) {
    /* TODO */
}

void SocialNetwork::addPost(const std::string &message, const std::string &email) {
    /* TODO */
}

void SocialNetwork::deleteProfile(const std::string &email) {
    /* TODO */
}

void SocialNetwork::makeFriends(const std::string &email1, const std::string &email2) {
    /* TODO */
}

void SocialNetwork::likePost(int postId, const std::string &email) {
    /* TODO */
}

void SocialNetwork::unlikePost(int postId, const std::string &email) {
    /* TODO */
}

LinkedList<Profile *> SocialNetwork::getMutualFriends(const std::string &email1, const std::string &email2) {
    /* TODO */
}

LinkedList<Post *> SocialNetwork::getListOfMostRecentPosts(const std::string &email, int k) {
    /* TODO */
}

void SocialNetwork::print() const {
    std::cout << "# Printing the social network ..." << std::endl;

    std::cout << "# Number of profiles is " << profiles.getNumberOfNodes() << ":" << std::endl;
    profiles.print();

    std::cout << "# Number of posts is " << posts.getNumberOfNodes() << ":" << std::endl;
    posts.print();

    std::cout << "# Printing is done." << std::endl;
}
