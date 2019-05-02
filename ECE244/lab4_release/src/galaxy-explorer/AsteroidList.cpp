#include <galaxy-explorer/AsteroidList.hpp>
#include <iostream>
using namespace std;

AsteroidListItem::AsteroidListItem() {
    this->next = nullptr;
    this->data = nullptr;
}

AsteroidListItem::AsteroidListItem(Asteroid a) {
    this->next = nullptr;
    this->data = new Asteroid(
            a.getID(),
            a.getMass(),
            a.getHealth(),
            a.getCurrentHitbox(),
            a.getVelocity());
}

AsteroidListItem::~AsteroidListItem() {
    next = nullptr;
    delete next;
    delete data;
}

AsteroidList::AsteroidList() {
    // Head is not a pointer and it will call its default constructor
}

AsteroidList::AsteroidList(const AsteroidList& src) {
    // The functions in this class are listed in a suggested order of implementation,
    // except for this one and the destructor (because you should put all your constructors together).
    *this = src;
}

AsteroidList::~AsteroidList() {
    // The functions in this class are listed in a suggested order of implementation,
    // except for this one and the copy constructor (because you should put all your constructors together).
    clear(); // Empties out list til head is left
    // head's destructor does not need to be manually called bc it is not a pointer
}

void AsteroidList::pushFront(Asteroid e) {
    // Make a new item and insert ie between head and original first item
    AsteroidListItem* newHead = new AsteroidListItem(e);
    newHead->setNext(head.setNext(newHead));
}

Asteroid& AsteroidList::front() {
    return head.hasNext() ? head.getNext()->getData() : *(Asteroid*)nullptr;
}

const Asteroid& AsteroidList::front() const {
    return head.hasNext() ? head.getNext()->getData() : *(Asteroid*)nullptr;
}

bool AsteroidList::isEmpty() const {
    return !head.hasNext();
}

int AsteroidList::size() const {
    int sz=0;
    
    // Start at the first item
    const AsteroidListItem* temp = head.getNext();
    
    // Go through list and count number of items
    while(temp!=nullptr) {
        sz++;
        temp = temp->getNext();
    }
    
    return sz;
}

AsteroidListItem* AsteroidList::beforeBegin() {
    return &head;
}

const AsteroidListItem* AsteroidList::beforeBegin() const {
    return &head;
}

AsteroidListItem* AsteroidList::begin() {
    return head.getNext();
}

const AsteroidListItem* AsteroidList::begin() const {
    return head.getNext();
}

AsteroidListItem* AsteroidList::beforeEnd() {
    AsteroidListItem* temp = head.getNext();
    
    // Go through list until an item's next item is null
    while(temp->hasNext()) {
        temp = temp->getNext();
    }
    
    return temp;
}

const AsteroidListItem* AsteroidList::beforeEnd() const {
    const AsteroidListItem* temp = head.getNext();
    
    // Go through list until an item's next item is null
    while(temp->hasNext()) {
        temp = temp->getNext();
    }
    
    return temp;
}

AsteroidListItem* AsteroidList::end() {
    return nullptr;
}

const AsteroidListItem* AsteroidList::end() const {
    return nullptr;
}

AsteroidListItem* AsteroidList::insertAfter(AsteroidListItem* prev, Asteroid e) {
    AsteroidListItem* toInsert = new AsteroidListItem(e);
    
    // Insertion, inserts toInsert after prev and the old
    // AsteroidListItem* after prev to be after toInsert
    toInsert->setNext(prev->setNext(toInsert));
    
    return toInsert;
}

AsteroidListItem* AsteroidList::insertAfter(AsteroidListItem* insertion_point, const AsteroidList& others) {
    const AsteroidListItem* srcItem = &others.head;
    
    // Go through others and copy its items one by one to insert
    while(srcItem->hasNext()) {
        srcItem = srcItem->getNext();
        AsteroidListItem* toInsert = new AsteroidListItem(srcItem->getData());
        toInsert->setNext(insertion_point->setNext(toInsert));
        insertion_point = insertion_point->getNext();
    }
    
    return insertion_point;
}

AsteroidListItem* AsteroidList::eraseAfter(AsteroidListItem* prev) {// Set the next AsteroidListItem* for prev to the item 2 after
    // only if there is actually an AsteroidListItem* after prev
    AsteroidListItem* toDelete = prev->setNext(prev->getNext()->getNext());
    delete toDelete;
    
    return prev->getNext();
}

void AsteroidList::clear() {
    // Point head to the item after the first item and delete the old first item
    while(head.hasNext()) {
        AsteroidListItem* toDelete = head.setNext(head.getNext()->getNext());
        delete toDelete;
    }
}

AsteroidList& AsteroidList::operator=(const AsteroidList& src) {
    // Clear current list
    clear();
    
    // Copy in new list
    this->insertAfter(&head,src);
    
    return *this;
}
