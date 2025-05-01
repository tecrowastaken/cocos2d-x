#ifndef __CCARRAY_H__
#define __CCARRAY_H__


#ifndef CC_DLL_DEFINED
    #define CC_DLL
#endif

#include <cocos2d.h>

/* C++ Wrapper for 2.0 cocos2d::ccArray* Because I didn't like cocos2d::Vector */

template<class T>
class CCArray {
public:
    cocos2d::ccArray* c_array;

    CCArray(ssize_t capacity = 0){
        c_array = cocos2d::ccArrayNew(capacity);
    }

    ~CCArray(){
        if (c_array != nullptr){
            cocos2d::ccArrayFree(c_array);
        }
    }

    ssize_t count() noexcept {
        return c_array->num;
    }

    void doubleCapacity(){
        cocos2d::ccArrayDoubleCapacity(c_array);
    }

    void ensureExtraCapacity(ssize_t extra){
        cocos2d::ccArrayEnsureExtraCapacity(c_array, extra);
    }

    void shrink(){
        cocos2d::ccArrayShrink(c_array);
    }
    
    ssize_t getIndexOfObject(T* object){
        return cocos2d::ccArrayGetIndexOfObject(c_array, object);
    }
    
    bool containsObject(T* object){
        return cocos2d::ccArrayGetIndexOfObject(c_array, object);
    }

    void appendObject(T* object){
        return cocos2d::ccArrayAppendObject(c_array, object);
    }

    void appendObjectWithResize(T* object){
        cocos2d::ccArrayAppendObjectWithResize(c_array, object);
    }

    void removeAllObjects(){
        cocos2d::ccArrayRemoveAllObjects(c_array);
    }

    // Here comes the fun part...

    T* operator[](ssize_t index){
        return c_array->arr[index];
    }

    CCArray& operator+=(const CCArray& rhs){
        cocos2d::ccArrayAppendArrayWithResize(c_array, rhs.c_array);
        return *this;
    }

    CCArray& operator-=(const CCArray& rhs){
        cocos2d::ccArrayFullRemoveArray(c_array, rhs.c_array);
        return *this;
    }

    friend CCArray operator+(CCArray& lhs, const CCArray&rhs){
        lhs += rhs;
        return lhs;
    }

    friend CCArray operator-(CCArray& lhs, const CCArray&rhs){
        lhs -= rhs;
        return lhs;
    }


    void removeObjectAtIndex(ssize_t index, bool releaseObj = true){
        return cocos2d::ccArrayRemoveObjectAtIndex(c_array, index, releaseObj);
    }

    void fastRemoveObjectAtIndex(ssize_t index){
        return cocos2d::ccArrayFastRemoveObjectAtIndex(c_array, index);
    }

    void removeObject(T* object, bool releaseObj = true){
        cocos2d::ccArrayRemoveObject(c_array, object, releaseObj);
    }
};


#endif // __CCARRAY_H__
