/*
 * This file was developed for the Fall 2018 instance of ECE244 at the University of Toronto.
 * Creator: Matthew J. P. Walker
 */


#include <galaxy-explorer/AsteroidsObserver.hpp>

void AsteroidsObserver::onAsteroidInRange(Asteroid asteroid) {
    this->asteroid_list.pushFront(asteroid);
}

void AsteroidsObserver::onAsteroidUpdate(Asteroid asteroid) {
    // Remove the old asteroid, just like when it goes out of range
    onAsteroidOutOfRange(asteroid);
    
    // Push in the new asteroid
    asteroid_list.pushFront(asteroid);
}

void AsteroidsObserver::onAsteroidOutOfRange(Asteroid asteroid) {
    // Set temp to head
    AsteroidListItem* temp = asteroid_list.beforeBegin();
    
    // Find the asteroid right before the asteroid that moved out of range
    while(temp->hasNext() && temp->getNext()->getData().getID()!=asteroid.getID()) {
        temp = temp->getNext();
    }
    
    // Remove the old asteroid
    asteroid_list.eraseAfter(temp);    
}

void AsteroidsObserver::onAsteroidDestroy(Asteroid asteroid) {
    // Remove the old asteroid, just like when it goes out of range
    onAsteroidOutOfRange(asteroid);
}
