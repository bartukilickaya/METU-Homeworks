#ifndef _director_h__
#define _director_h__

#include "movie.h"
#include <cstring>


class DirectorComparator
{
  public:
    // IMPLEMENT BELOW inline
    bool operator( ) (const Movie::SecondaryKey & key1, 
                      const Movie::SecondaryKey & key2) const {
        //cout << key1.getDirector() << ":" << key2.getDirector() << ":" << endl;
        char c1, c2;
        int i = 0;
        while (key1.getDirector()[i] != '\0' && key2.getDirector()[i] != '\0') {
            //cout << key1.getDirector() << ":" << key2.getDirector() << ":" << endl;
            c1 = key1.getDirector()[i];
            c2 = key2.getDirector()[i];
            if (c1 < 97) {
                c1 += 32;
            }
            if (c2 < 97) {
                c2 += 32;
            }
            if (c1 < c2) {
                return true;
            } else if (c1 == c2) {
                i++;
            } else {
                return false;
            }
        }
        if (key1.getDirector().length() < key2.getDirector().length()) {
            return true;
        } else if (key1.getDirector().length() > key2.getDirector().length()) {
            return false;
        }
        i = 0;
        while (key1.getTitle()[i] != '\0' && key2.getTitle()[i] != '\0') {
            c1 = key1.getTitle()[i];
            c2 = key2.getTitle()[i];
            if (c1 < 97) {
                c1 += 32;
            }
            if (c2 < 97) {
                c2 += 32;
            }
            if (c1 < c2) {
                return true;
            } else if (c1 == c2) {
                i++;
            } else {
                return false;
            }
        }
        if (key1.getTitle().length() < key2.getTitle().length()) {
            return true;
        } else {
            return false;
        }
    }
};

#endif
