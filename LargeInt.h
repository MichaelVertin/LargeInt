#ifndef LARGE_INT_H
#define LARGE_INT_H

#include <stdio.h>
#include <iostream>
#include <sstream>

#include <climits>
#include <bitset>
#include <type_traits>
#include <stdexcept>
#include <typeinfo>
#include <algorithm> // std::reverse
#include <cmath> // log2




namespace LI_Properties
{
/*
    namespace digit
    {
        typedef unsigned int type;
        const unsigned int GREATEST_BIT_MASK = (unsigned int)INT_MAX + 1;
        const int SIZE = CHAR_BIT * sizeof( LI_Properties::digit::type );
        const unsigned int MAX = UINT_MAX;
        const int HALF_SIZE = LI_Properties::digit::SIZE / 2;
        const unsigned int LOWER_HALF_MASK = LI_Properties::digit::MAX >> 
                                             HALF_SIZE;
    }
*/
    namespace digit
    {
        typedef uint32_t type;
        const uint32_t GREATEST_BIT_MASK = 0x7FFFFFFF;
        const int SIZE = CHAR_BIT * sizeof( uint32_t );
        const uint32_t MAX = 0xFFFFFFFF;
        const int HALF_SIZE = LI_Properties::digit::SIZE / 2;
        const uint32_t LOWER_HALF_MASK = LI_Properties::digit::MAX >> 
                                             HALF_SIZE;
        namespace doubleSize
        {
            typedef uint64_t type;
        }
    }

    const int INITIAL_CAPACITY = 0;
}

class LargeInt;


/*
An integer with a static size, automatically update to account for operators
data representation:
  the value is represented using LI_digits (unsigned int).
  start contains smaller values, end contains larger values, such that
  >>> 1,42,14641 represents 1 + 42*(DIGIT::MAX+1)^1 + 14641*(DIGIT::MAX+1)^2
*/
class LargeInt
{
private:
    // attributes: 
        // array of digits (lower is less significant)
    LI_Properties::digit::type *digits;
    unsigned int size;
    unsigned int capacity;
    bool sign; // boolean sign (does the value has a <negative> sign?)

    // for generating initial memory for the data
    void initializeMemory( int initialMemory = 0 );

    //////////////////////////// memory management /////////////////////////////
    void resize( unsigned int newSize );
    void reallocate( unsigned int newCapacity );
    void removeLeadingZeros();

    /* shallowSplit
    Before Call:
     - 'low' and 'high' must have memory allocated correctly (including base constrcutor)
    After Call:
     - 'low' and 'high' are a shallow copy, therefore the destructor cannot be called
        - set digits to NULL for both to prevent the destructor from erroring
        - low and high must be treated as constant after this function is called
           - before modifying anything, set digits to NULL for the corresponding LI
     - low begins at the start of this->digits, high begins at the splitInd of this->digits
        - low ends at this->digits[ splitInd ] (exclusive)
        - high ends at this->digits[ size ] (exclusive)

    Requirements:
     - splitInd must be no greater than the size of this
    */
    void shallowSplit( LargeInt &low, LargeInt &high, 
                       unsigned int splitInd ) const;


public:
    ////////////////////////// constructors ///////////////////////////////////
    LargeInt();
    LargeInt( const LargeInt &source );
    explicit LargeInt( const int &source );
    explicit LargeInt( const LI_Properties::digit::type &source );
    // large->small significance
    explicit LargeInt( const std::string &numericString, unsigned int base = 10 );
    ~LargeInt();

    // data access
    std::string toBinary() const;
    std::string toStringBruteForce( unsigned int base = 10, 
                                    unsigned int forceSize = 0 ) const;
    std::string toString( unsigned int base = 10 ) const;
    std::string stringMagnitude( unsigned int base, unsigned int forceSize ) const;

    // operators
    void addDigitAtIndex( LI_Properties::digit::type toAdd, unsigned int addIndex );
    void digitShiftLesser( int shiftAmount );
    void digitShiftGreater( int shiftAmount );
    void operator=( const LargeInt &source );
    operator int() const;


    // friends
    friend void operator <<= ( LargeInt &toShift, int shiftAmount );
    friend void operator >>= ( LargeInt &toShift, int shiftAmount );
    friend LargeInt operator+( const LargeInt &one, const LargeInt &other );
    friend LargeInt addMagnitude( const LargeInt &one, const LargeInt &other );
    friend LargeInt subtractMagnitude( const LargeInt &one, const LargeInt &other );
    friend int spaceshipMagComp( const LargeInt &first, const LargeInt &second );
    friend int spaceshipComp( const LargeInt &first, const LargeInt &second );
    friend LargeInt operator-( LargeInt &first, LargeInt &second );
    friend void operator*=( LargeInt &one, unsigned int other );
    friend LargeInt operator*( const LargeInt &one, const LargeInt &other );
    friend LargeInt multiplyLIMagnitude( const LargeInt &one, const LargeInt &other );
    friend LargeInt operator/( const LargeInt &numerator, const LargeInt &denominator );
    friend void divideLIMagnitude( const LargeInt &numerator, 
                                   const LargeInt &denominator, 
                                   LargeInt &divisionResult, LargeInt &remainder );
    friend LargeInt gradeschoolMagMult( const LargeInt &one, const LargeInt &other );
};




//////////////////////////// LargeInt Operators ///////////////////////////////
//////////// shifting ///////////////
void operator <<= ( LargeInt &toShift, int shiftAmount );
void operator >>= ( LargeInt &toShift, int shiftAmount );
LargeInt operator<<( LargeInt &toShift, int shiftVal );
LargeInt operator>>( LargeInt &toShift, int shiftVal );

//////////// addition ///////////////////
LargeInt operator+( const LargeInt &one, const LargeInt &other );
LargeInt addMagnitude( const LargeInt &one, const LargeInt &other );

/////////// subtraction /////////////////
LargeInt subtractMagnitude( const LargeInt &one, const LargeInt &other );
LargeInt operator-( LargeInt &first, LargeInt &second );


/////////// multiplication //////////////
LargeInt operator*( const LargeInt &one, const LI_Properties::digit::type other );
LargeInt operator*( const LargeInt &one, const LargeInt &other );
void operator*=( LargeInt &one, LI_Properties::digit::type other );
void operator*=( LargeInt &first, const LargeInt &second );
LargeInt multiplyLI( const LargeInt &one, const LargeInt &other );
LargeInt gradeschoolMagMult( const LargeInt &one, const LargeInt &other );

/////////// division //////////////
LargeInt operator/( const LargeInt &numerator, const LargeInt &denominator );
void divideLIMagnitude( const LargeInt &numerator, const LargeInt &denominator, 
                              LargeInt &divisionResult, LargeInt &remainder );


////////////// comparing ///////////////
int spaceshipMagComp( const LargeInt &first, const LargeInt &second );
int spaceshipComp( const LargeInt &first, const LargeInt &second );
bool operator<=( const LargeInt &first, const LargeInt &second );
bool operator>=( const LargeInt &first, const LargeInt &second );
bool operator>( const LargeInt &first, const LargeInt &second );
bool operator<( const LargeInt &first, const LargeInt &second );
bool operator==( const LargeInt &first, const LargeInt &second );
bool operator!=( const LargeInt &first, const LargeInt &second );





///////////////////////////////////////////////////////////////////////////////



#ifndef MAX_FUNCTION
#define MAX_FUNCTION
template <typename TestType>
TestType max( TestType one, TestType two )
{
    if( one > two )
    {
        return one;
    }
    return two;
}
#endif // MAX_FUNCTION

#ifndef MIN_FUNCTION
#define MIN_FUNCTION
template <typename TestType>
TestType min( TestType one, TestType two )
{
    if( one > two )
    {
        return two;
    }
    return one;
}
#endif // MIN_FUNCTION


template <typename ElementType>
void copyArray( const ElementType *input, ElementType *output, unsigned int size )
{
    unsigned int index;
    for( index = 0; index < size; index++ )
    {
        output[ index ] = input[ index ];
    }
}




// source: https://stackoverflow.com/questions/6038718/convert-integer-to-bits
// SFINAE for safety. Sue me for putting it in a macro for brevity on the function
#define IS_INTEGRAL(T) typename std::enable_if< std::is_integral<T>::value >::type* = 0
template<class T>
std::string toBinaryString(T byte, IS_INTEGRAL(T))
{
    std::bitset<sizeof(T) * CHAR_BIT> bs(byte);
    return bs.to_string();
}



void multiplyDigits( LI_Properties::digit::type one, 
                     LI_Properties::digit::type other, 
                     LI_Properties::digit::type &low, 
                     LI_Properties::digit::type &high );



unsigned int charToInt( char charVal );
char intToChar( int testInt );


#endif // LARGE_INT_H





template <typename BaseType>
BaseType toPower( BaseType base, unsigned int power )
{
   BaseType result = BaseType( 1 );
   BaseType baseMultiplier = BaseType( base );
   while( power )
   {
       if( power & 1 )
       {
           result *= baseMultiplier;
       }

       power >>= 1;

       if( power )
       {
           baseMultiplier *= baseMultiplier;
       }
   }
   return result;
}





