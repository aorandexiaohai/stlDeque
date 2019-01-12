#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>
#include <memory>
#include <iostream>

const int nodeN = 1000;

namespace sjtu {
    template<class T>
    class deque{
    public:
        struct nodeT{
            nodeT *prev;
            nodeT *next;
            T **arr;
            int curLength;
            nodeT(){
                arr = new T*[nodeN];
                prev = NULL;
                next = NULL;
                curLength = 0;
            }
            ~nodeT(){
                int i;
                for(i  = 0; i < curLength; i++){
                    delete arr[i];
                }
                delete []arr;
            }
        };
        
        class const_iterator;
        class iterator {
            friend class deque;
            friend class const_iterator;
        public:     
            nodeT *node;
            int curPo;
            deque *deqId;
        public:
            /**
             * return a new iterator which pointer n-next elements
             *   even if there are not enough elements, the behaviour is **undefined**.
             * as well as operator-
             */
            iterator(nodeT *n = NULL, int c = 0, deque *deq = NULL) {
                node = n;
                curPo = c;
                deqId = deq;
            }
            iterator(const const_iterator &other) {
                node = other.node;
                curPo = other.curPo;
                deqId = other.deqId;
            }

            iterator(const iterator &other){
                node = other.node;
                curPo = other.curPo;
                deqId = other.deqId;
            }

            iterator &operator=(const iterator &rhs){
                if(this == &rhs){
                    return *this;
                }
                else{
                    curPo = rhs.curPo;
                    deqId = rhs.deqId;
                    node = rhs.node;
                    return *this;
                }
            }

            void setNode(nodeT *newNode){
                node = newNode;
            }

            iterator operator+(const long int &n) const {
                iterator tmp = *this;
                return tmp += n;
            }

            iterator operator-(const long int &n) const {
                iterator tmp = *this;
                return tmp -= n;
            }

            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            
            long int operator-(const iterator &rhs) const {
                if(deqId != rhs.deqId){
                    throw invalid_iterator();
                }
                if(node == rhs.node){
                    return curPo - rhs.curPo;
                }
                nodeT *tmpNode = node -> next;
                long int dist = node -> curLength - curPo;
                while(tmpNode != NULL && tmpNode != rhs.node){
                    dist = dist + tmpNode -> curLength;
                    tmpNode = tmpNode -> next;
                }
                if(tmpNode != NULL){
                    dist = (dist + rhs.curPo);
                    return -dist;
                }
                dist = rhs.node -> curLength - (rhs.curPo);
                tmpNode = rhs.node -> next;
                while(tmpNode != NULL && tmpNode != node){
                    dist = dist + tmpNode -> curLength;
                    tmpNode = tmpNode -> next;
                }
                if(tmpNode != NULL){
                    dist = (dist + (curPo));
                    return dist;
                }
                throw runtime_error();
            }
            
            iterator& operator+=(const long int &n) {
                if(n < 0){
                    *this -= -n;
                    return *this;
                }
                if(node -> curLength == 0 || n == 0){
                    return *this;
                }
                long int diff = n;
                while(node -> next != NULL && diff >= node -> curLength  - curPo){
                    diff = diff - (node -> curLength  - curPo);
                    setNode(node -> next);
                    curPo = 0;
                }
                if(diff > node -> curLength  - curPo){
                    throw index_out_of_bound();
                }
                curPo += diff;
                return *this;
            }
            
            iterator& operator-=(const long int &n) {
                if(n < 0){
                    *this += -n;
                    return *this;
                }
                if(node -> curLength == 0 || n == 0){
                    return *this;
                }
                long int diff = n;
                while(node -> prev -> prev != NULL && diff > curPo){
                    diff = diff - (curPo + 1);
                    setNode(node -> prev);
                    curPo = node -> curLength - 1;
                }
                if(node -> prev -> prev == NULL && diff > curPo){
                    throw index_out_of_bound();
                }
                curPo -= diff;
                return *this;

            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator tmp = *this;
                ++*this;
                return tmp;
            }
            /**
             * TODO ++iter
             */
            iterator& operator++() {
                ++curPo;
                if(curPo == nodeN && node -> next == NULL){
                    return *this;
                }
                else{
                    if(curPo == node -> curLength && node -> next != NULL){
                        setNode(node -> next);
                        curPo = 0;
                    }
                    return *this;
                }
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp = *this;
                --*this;
                return tmp;
            }
            /**
             * TODO --iter
             */
            iterator& operator--() {
                if(curPo == 0){
                    if(node -> prev -> prev == NULL){
                        throw index_out_of_bound();
                    }
                    setNode(node -> prev);
                    curPo = node -> curLength;
                }
                curPo--;
                return *this;
            }
            /**
             * TODO *it
             */
            T& operator*() const {
                if(curPo < 0 || curPo  >= node -> curLength){
                    throw invalid_iterator();
                }
                else{
                    return *(node -> arr[curPo]);
                } 
            }
            /**
             * TODO it->field
             */
            T* operator->() const noexcept {
                return node -> arr[curPo];
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const const_iterator &rhs) const{
                return ((curPo == rhs.curPo) && (deqId == rhs.deqId) && (node == rhs.node));
            }
            bool operator==(const iterator &rhs) const {
                return ((curPo == rhs.curPo) && (deqId == rhs.deqId) && (node == rhs.node));
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };
        
        //****************************************

        class const_iterator {
            friend class deque;
            friend class iterator;
            // it should has similar member method as iterator.
            // and it should be able to construct from an iterator.
        private:
            // data members.
            nodeT *node;
            int curPo;
            const deque *deqId;
        public:
            const_iterator(){
                node  = NULL;
                curPo = 0;
                deqId = NULL;
            }

            const_iterator(nodeT *n, int c, const deque *deq) {
                node = n;
                curPo = c;
                deqId = deq;
            }

            const_iterator(const const_iterator &other) {
                node = other.node;
                curPo = other.curPo;
                deqId = other.deqId;
            }

            const_iterator(const iterator &other){
                node = other.node;
                curPo = other.curPo;
                deqId = other.deqId;
            }

            void setNode(nodeT *newNode){
                node = newNode;
            }

            const_iterator operator+(const long int &n) const {

                const_iterator tmp = *this;
                return tmp += n;
            }

            const_iterator operator-(const long int &n) const {

                const_iterator tmp = *this;
                return tmp -= n;
            }
            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            
            long int operator-(const const_iterator &rhs) const {
                if(deqId != rhs.deqId){
                    throw invalid_iterator();
                }
                if(node == rhs.node){
                    return (curPo) - (rhs.curPo);
                }
                nodeT *tmpNode = node -> next;
                long int dist = node -> curLength - (curPo);
                while(tmpNode != NULL && tmpNode != rhs.node){
                    dist = dist + tmpNode -> curLength;
                    tmpNode = tmpNode -> next;
                }
                if(tmpNode != NULL){
                    dist = (dist + (rhs.curPo));
                    return -dist;
                }

                dist = rhs.node -> curLength - (rhs.curPo );
                tmpNode = rhs.node -> next;
                while(tmpNode != NULL && tmpNode != node){
                    dist = dist + tmpNode -> curLength;
                    tmpNode = tmpNode -> next;
                }
                if(tmpNode != NULL){
                    dist = (dist + (curPo));
                    return dist;
                }
                throw runtime_error();
            }
            
            const_iterator& operator+=(const long int &n) {
                if(n < 0){
                    *this -= -n;
                    return *this;
                }
                if(node -> curLength == 0 || n == 0){
                    return *this;
                }
                long int diff = n;
                while(node -> next != NULL && diff >= node -> curLength  - curPo){
                    diff = diff - ( node -> curLength  - curPo);
                    setNode(node -> next);
                    curPo = 0;
                }
                if(diff > node -> curLength  - curPo){
                    throw index_out_of_bound();
                }
                curPo += diff;
                return *this;
            }
            
            const_iterator& operator-=(const long int &n) {

                if(n < 0){
                    *this += -n;
                    return *this;
                }
                if(node -> curLength == 0 || n == 0){
                    return *this;
                }
                long int diff = n;
                while(node -> prev -> prev != NULL && diff > curPo){
                    diff = diff - (curPo + 1);
                    setNode(node -> prev);
                    curPo = node -> curLength - 1;
                }
                if(node -> prev -> prev == NULL && diff > curPo){
                    throw index_out_of_bound();
                }
                curPo -= diff;
                return *this;
            }
            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                iterator tmp = *this;
                ++*this;
                return tmp;
            }
            /**
             * TODO ++iter
             */
            const_iterator& operator++() {
                ++curPo;
                if(curPo == nodeN && node -> next == NULL){
                    curPo--;
                    return *this;
                }
                else{
                    if(curPo ==  node -> curLength && node -> next != NULL){
                        setNode(node -> next);
                        curPo = 0;
                    }
                    return *this;
                }

                
            }
            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator tmp = *this;
                --*this;
                return tmp;
            }
            /**
             * TODO --iter
             */
            const_iterator& operator--() {
                if(curPo == 0){
                    if(node -> prev -> prev == NULL){
                        
                        return *this;
                    }
                    setNode(node -> prev);
                    curPo =  node -> curLength;
                }
                curPo--;
                return *this;
            
            }
            /**
             * TODO *it
             */
            T& operator*() const {
                if(curPo < 0 || curPo >= node -> curLength){
                    throw invalid_iterator();
                }
                else{
                    return *(node -> arr[curPo]);
                } 
            }
            /**
             * TODO it->field
             */
            T* operator->() const noexcept {
                return node -> arr[curPo];
            }
            
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const const_iterator &rhs) const{
                return ((curPo == rhs.curPo) && (deqId == rhs.deqId) && (node == rhs.node));
            }
            bool operator==(const iterator &rhs) const {
                return ((curPo == rhs.curPo) && (deqId == rhs.deqId) && (node == rhs.node));
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };

    public:
        nodeT *head;
        nodeT *tail;
        int sizeDeq;
        /**
         * TODO Constructors
         */
        deque() {
            head = new nodeT;
            tail = new nodeT;
            head -> next = tail;
            tail -> prev = head;
            sizeDeq = 0;
        }
        deque(const deque &other) {
            int i;
            head = new nodeT;
            nodeT *p;
            nodeT *tmp = head;
            nodeT *q = other.head -> next;
            while(q != NULL){
                p = new nodeT;
                p -> curLength = q -> curLength;
                for(i = 0; i < p -> curLength; i++){
                    p -> arr[i] = new T(*(q -> arr[i]));
                }
                p -> prev = tmp;
                tmp -> next = p;
                p -> next = NULL;
                tmp = p;
                q = q -> next;
            }
            tail = tmp;
            sizeDeq = other.sizeDeq;
        }
        /**
         * TODO Deconstructor
         */
        ~deque() {
            nodeT *p = head -> next;
            nodeT *q;
            while(p != NULL){
                q = p;
                p = p -> next;
                delete q;
            }
            delete head;
            sizeDeq = 0;
        }
        /**
         * TODO assignment operator
         */
        deque &operator=(const deque &other) {
            if(this == &other){
                return *this;
            }
            int i;
            nodeT *p = head -> next;
            nodeT *q;
            while(p != NULL){
                q = p;
                p = p -> next;
                delete q;
            }        
            nodeT *tmp = head;
            q = other.head -> next;
            while(q != NULL){
                p = new nodeT;
                p -> curLength = q -> curLength;
                for(i = 0; i < p -> curLength; i++){
                    p -> arr[i] = new T(*(q -> arr[i]));
                }
                p -> prev = tmp;
                tmp -> next = p;
                p -> next = NULL;
                tmp = p;
                q = q -> next;
            }
            tail = tmp;
            sizeDeq = other.sizeDeq;
            return *this;
        }
        /**
         * access specified element with bounds checking
         * throw index_out_of_bound if out of bound.
         */
        T & at(const size_t &pos) {
            return operator[](pos);
        }
        const T & at(const size_t &pos) const {

            return operator[](pos);
        }

        T & operator[](const size_t &pos) {
            int distt = pos;
            
            if(distt < 0){
               
                throw index_out_of_bound();
            }
            nodeT *optNode = head -> next;
            
            while(optNode != NULL && distt >= optNode -> curLength){
                distt -= optNode ->curLength;
                optNode = optNode -> next;
            }
            if(optNode == NULL){
                throw index_out_of_bound();
            }
            return *(optNode -> arr[distt]);
        }
        const T & operator[](const size_t &pos) const{
            int distt = pos;
            if(distt < 0){
                throw index_out_of_bound();
            }

            nodeT *optNode = head -> next;
            
            while(optNode != NULL && distt >= optNode -> curLength){
                distt -= optNode ->curLength;
                optNode = optNode -> next;
            }
            if(optNode == NULL){
                throw index_out_of_bound();
            }
            return *(optNode -> arr[distt]);
        }
        /**
         * access the first element
         * throw container_is_empty when the container is empty.
         */
        const T & front() const {
            if(sizeDeq == 0){
                throw container_is_empty();
            }
            return *(head -> next -> arr[0]);
        }
        /**
         * access the last element
         * throw container_is_empty when the container is empty.
         */
        const T & back() const {
            if(sizeDeq == 0){
                throw container_is_empty();
            }
            return *(tail -> arr[tail -> curLength - 1]);
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            return iterator(head -> next, 0, this);
        }
        const_iterator cbegin() const {
            return const_iterator(head -> next, 0, this);
        }
        /**
         * returns an iterator to the end.
         */
        iterator end(){
            if(sizeDeq == 0){
                return begin();
            }
            return iterator(tail , tail -> curLength, this);
        }
        const_iterator cend() const {
            if(sizeDeq == 0){
                return cbegin();
            }
            return const_iterator(tail ,tail -> curLength, this);
        }
        /**
         * checks whether the container is empty.
         */
        bool empty() const {
            return sizeDeq == 0;
        }
        /**
         * returns the number of elements
         */
        size_t size() const {
            return sizeDeq;
        }
        /**
         * clears the contents
         */
        void clear() {
            int i;
            nodeT *p = head -> next;
            nodeT *q = NULL;
            while(p != NULL){
                q = p;
                p = p -> next;     
                delete q;
            }
            tail = new nodeT;
            tail -> prev = head;
            head -> next = tail;
            sizeDeq = 0;

        }
        /**
         * inserts elements at the specified locat on in the container.
         * inserts value before pos
         * returns an iterator pointing to the inserted value
         *     throw if the iterator is invalid or it point to a wrong place.
         */
        iterator insert(iterator pos, const T &value) {
            if(this != pos.deqId){
                throw invalid_iterator();
            }
            if(pos.curPo > pos.node -> curLength){
                throw invalid_iterator();
            }
            sizeDeq++;
            if(pos.node -> curLength == nodeN && pos.node == tail){
                nodeT *p;
                if(pos.curPo == nodeN){
                    p =  new nodeT;
                    p -> next = NULL;
                    p -> prev = pos.node;
                    pos.node -> next = p;
                    p -> arr[0] = new T(value);
                    p -> curLength = 1;
                    tail = p;
                }
                else{
                    p =  new nodeT;
                    p -> next = NULL;
                    p -> prev = pos.node;
                    pos.node -> next = p;
                    p -> arr[0] = pos.node -> arr[nodeN - 1];
                    p -> curLength = 1;
                    tail = p;
                    int tmpPo = nodeN - 1;
                    while(tmpPo != pos.curPo){
                        pos.node -> arr[tmpPo] = pos.node -> arr[tmpPo - 1];
                        tmpPo = tmpPo - 1;
                    }
                    pos.node -> arr[tmpPo] = new T(value);
                    return pos;
                }
            }
            if(pos.node -> curLength == nodeN && pos.node -> next -> curLength == nodeN){
                nodeT *p;
                p =  new nodeT;
                p -> next = pos.node -> next;
                pos.node -> next -> prev = p;
                p -> prev = pos.node;
                pos.node -> next = p;
                p -> arr[0] = pos.node -> arr[nodeN - 1];
                p -> curLength = 1;
                int tmpPo = nodeN - 1;
                while(tmpPo != pos.curPo){
                    pos.node -> arr[tmpPo] = pos.node -> arr[tmpPo - 1];
                    tmpPo = tmpPo - 1;
                }
                pos.node -> arr[tmpPo] = new T(value);
                return pos;
            }
            
            if(pos.node -> curLength == nodeN && pos.node -> next -> curLength != nodeN){
                nodeT *tmpNode = pos.node -> next;
                int tmpPo = tmpNode -> curLength;
                while(tmpPo != 0){
                    tmpNode -> arr[tmpPo] = tmpNode -> arr[tmpPo - 1];
                    tmpPo = tmpPo - 1;
                }
                tmpNode -> arr[tmpPo] = pos.node -> arr[nodeN - 1];
                tmpNode -> curLength++;
                
                tmpPo = nodeN - 1;
                while(tmpPo != pos.curPo){
                    pos.node -> arr[tmpPo] = pos.node -> arr[tmpPo - 1];
                    tmpPo = tmpPo - 1;
                }
                pos.node -> arr[tmpPo] = new T(value);
                return pos;
            }
            else{ 
               
                int tmpPo = pos.node -> curLength;
                while(tmpPo != pos.curPo){
                    pos.node -> arr[tmpPo] = pos.node -> arr[tmpPo - 1];
                    tmpPo = tmpPo - 1;
                }
                pos.node -> arr[tmpPo] = new T(value);
                pos.node -> curLength++;
                return pos;
            }
        }
        /**
         * removes specified element at pos.
         * removes the element at pos.
         * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
         * throw if the container is empty, the iterator is invalid or it points to a wrong place.
         */
        iterator erase(iterator pos){
            int i;
            if(this != pos.deqId){
                throw invalid_iterator();
            }
            if(sizeDeq == 0){
                throw container_is_empty();  
            }
            if(pos.curPo >= pos.node -> curLength){
                throw invalid_iterator();
            }
            
            if(pos.curPo == 0 && pos.node == head -> next){
               
                pop_front();
                return begin();
            }
            else{
                if((pos.curPo == pos.node -> curLength - 1) && pos.node == tail){
                    
                    pop_back();
                    return end();
                }
                else{
                    sizeDeq--;
                    if(pos.node -> curLength == 1){
                        
                        nodeT *p = pos.node;
                        pos.node -> prev -> next = pos.node -> next;
                        pos.node -> next -> prev = pos.node -> prev;
                        pos.setNode(pos.node -> next);
                        pos.curPo = 0;
                        delete (p);  
                    }
                    else{
                        if(pos.curPo == pos.node -> curLength - 1){
                            
                            delete pos.node -> arr[pos.curPo];                            
                            pos.node -> curLength--;
                            pos.setNode(pos.node -> next);
                            pos.curPo = 0;
                        }
                        else{
                            
                            int tmpPo = pos.curPo;
                            delete pos.node -> arr[tmpPo];
                            while(tmpPo != pos.node -> curLength - 1){
                                pos.node -> arr[tmpPo] = pos.node -> arr[tmpPo + 1];
                                tmpPo = tmpPo + 1;
                            }
                            pos.node -> curLength--;
                        }
                    }
                    return pos; 
                }
            }
        }
        /**
         * adds an element to the end
         */
        void push_back(const T &value) {
            
            if(sizeDeq == 0){
                tail -> arr[0] = new T(value);
                tail -> curLength++;
                sizeDeq++;
                return;
            }

            if(tail -> curLength >= nodeN / 2){
                nodeT *p;
                p = new nodeT;
                p -> prev = tail;
                p -> next = NULL;
                tail -> next = p;
                p -> arr[0] = new T(value);
                p -> curLength = 1;
                tail = p;
            }    
            else{
                
                tail -> curLength++;
                tail -> arr[tail -> curLength - 1] = new T(value);
            }
            sizeDeq++;
        
        }
        /**
         * removes the last element
         *     throw when the container is empty.
         */
        void pop_back() {
            if(sizeDeq == 0){
                
                throw container_is_empty();
            }
            sizeDeq--;          
            if(tail -> curLength == 1){
                if(tail -> prev == head){
                    delete tail -> arr[0];
                    tail -> curLength--;
                }
                else{
                    
                    nodeT *p = tail;
                    tail = tail -> prev;
                    tail -> next = NULL;
                    delete p;
                }

            }
            else{
                
                delete tail -> arr[tail -> curLength - 1];
                tail -> curLength--;
            }
        }
        /**
         * inserts an element to the beginning.
         */
        void push_front(const T &value) {
            
            if(sizeDeq == 0){
                tail -> arr[0] = new T(value);
                tail -> curLength++;
                sizeDeq++;
                return;
            }
            if(head -> next -> curLength >= nodeN / 2){
               
                nodeT *p;
                p = new nodeT;
                p -> next = head -> next;
                head -> next -> prev = p;
                head -> next = p;
                p -> prev = head;
                
                p -> curLength = 1;
                p -> arr[0] = new T(value);
            }
            else{
               
                nodeT *startNode = head -> next;
                int tmpCurPo = startNode -> curLength;

                while(tmpCurPo != 0){
                    startNode -> arr[tmpCurPo] = startNode -> arr[tmpCurPo - 1];
                    tmpCurPo = tmpCurPo - 1;
                }
                startNode -> arr[tmpCurPo] = new T(value);
                startNode -> curLength++;
            }
            sizeDeq++;


        }
        /**
         * removes the first element.
         *     throw when the container is empty.
         */
        void pop_front() {
          
            if(sizeDeq == 0){
                
                throw container_is_empty();
            }
            sizeDeq--;
            nodeT *startNode = head -> next; 
            if(startNode -> curLength == 1){
                if(startNode == tail){
                    delete tail -> arr[0];
                    tail -> curLength--;
                }
                else{
                   
                    nodeT *p = startNode;
                    p -> next -> prev = head;
                    head -> next = p -> next;
                    delete p;
                }
            }
            else{
                
                int tmpPopPo = 0;
                delete startNode -> arr[tmpPopPo];
                while(tmpPopPo != startNode -> curLength - 1){
                    startNode -> arr[tmpPopPo] =  startNode -> arr[tmpPopPo + 1];
                    tmpPopPo += 1;
                }
                startNode -> curLength--;
            }
        }
    };
}

#endif


