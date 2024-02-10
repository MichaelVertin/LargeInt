#include "LargeInt.h"


LargeInt::LargeInt( const LargeInt &source )
{
   initializeMemory();
   resize( source.size );
   copyArray( source.digits, digits, size );
   sign = source.sign;
}

LargeInt::LargeInt()
{
    initializeMemory();
    sign = false;
}

void LargeInt::operator=( const LargeInt &source )
{
    // delete original memory
    delete []digits;

    initializeMemory();
    resize( source.size );
    copyArray( source.digits, digits, size );
    sign = source.sign;
}

LargeInt::LargeInt( const int &source )
{
    initializeMemory();
    // set to size 0 if value is 0
    if( source == 0 )
    {
        resize( 0 );
    }
    else
    {
        // space for 1 digit
        resize( 1 );
        // set first element to absolute value of source
        *digits = abs( source );
        // set sign to 'source is negative'
        sign = source < 0;
    }
}

LargeInt::LargeInt( const LI_Properties::digit::type &source )
{
    initializeMemory();
    if( source == 0 )
    {
        resize( 0 );
    }
    else
    {
        // space for 1 digit
        resize( 1 );
        // set first element to source (unsigned)
        *digits = source;
    }
}

LargeInt::LargeInt( const std::string &numericString, unsigned int base )
{
    std::string::const_iterator wkgChar;
    bool hasSign;
    initializeMemory();
    *this = LargeInt( 0 );

    // empty string -> 0
    if( numericString.size() == 0 )
    {
        return;
    }

    // set iterator
    wkgChar = numericString.begin();

    // set sign if present
    if( *wkgChar == '-' )
    {
        hasSign = true;
        wkgChar++;
    }
    else
    {
        hasSign = false;
    }

    // iterate most significant->least
    while( wkgChar != numericString.end() )
    {
        *this = *this * base + LargeInt( charToInt( *wkgChar ) );
        wkgChar++;
    }

    sign = hasSign;
}

LargeInt::~LargeInt()
{
    delete []digits;
}

void LargeInt::initializeMemory( int initialMemory )
{
    capacity = initialMemory;
    digits = new LI_Properties::digit::type[ capacity ];

    sign = 0; // default 0
    size = 0;
}


void LargeInt::resize( unsigned int newSize )
{
    // error handle oversizing
    if( newSize > 0xFFFFFF )
    {
        throw std::overflow_error( "attempted to resize to "
                                    + std::to_string( (int)newSize )
                                    + " in LargeInt::resize\n" );
    }
    unsigned int index;

    // check size is increasing
    if( newSize > size )
    {
        // reallocate if necessary
        reallocate( newSize );

        // set all digits from previous size to new size to 0
        for( index = size; index < newSize; index++ )
        {
            digits[ index ] = 0;
        }
    }

    // update to newSize
    size = newSize;
}

void LargeInt::reallocate( unsigned int newCapacity )
{
    LI_Properties::digit::type *newDigits;

    // only process if newCapacity greater than oldCapacity
    if( newCapacity > capacity )
    {
        // set newCapacity to double if larger than newCapacity
        newCapacity = max( newCapacity, capacity * 2 );

        // generate new array with new capacity
        newDigits = new LI_Properties::digit::type[ newCapacity ];

        // copy original data to newdigits
        copyArray( digits, newDigits, size );

        // delete original memory
        delete []digits;

        // copy new data into attributes
        digits = newDigits;
        capacity = newCapacity;
        // (size is unchanged)
    }
}


void LargeInt::removeLeadingZeros()
{
    int newSize = size;
    // iterate to non-zero digit
    while( newSize > 0 && digits[ newSize - 1 ] == 0 )
    {
        newSize--;
    }
    resize( newSize );
}


/*
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
void LargeInt::shallowSplit( LargeInt &low, LargeInt &high, 
                             unsigned int splitInd ) const
{
    if( splitInd >= size )
    {
        throw std::overflow_error( "splitInd is greater than the size\n" );
    }

    // reset data of low/high
    low = LargeInt();
    high = LargeInt();

    // assign low to start of digits, high to digits + splitInd
    low.digits = digits; // include start
    high.digits = digits + splitInd; // include splitInd

    // assign sizes and capacities (same size/capacity for both)
    low.size = splitInd;
    low.capacity = low.size;
    high.size = size - low.size;
    high.capacity = high.size;

    // assign signs to same as parent
    low.sign = sign;
    high.sign = sign;
}

LargeInt::operator int() const
{
    if( size == 0 )
    {
        return 0;
    }
    if( sign )
    {
        return -digits[ 0 ];
    }
    return digits[ 0 ];
}




//////////////////////////// LargeInt Display /////////////////////////////////
std::string LargeInt::toBinary() const
{
    std::string outstr;
    int index;

    if( size == 0 )
    {
        outstr = "0";
    }
    else
    {
        if( sign )
        {
            outstr += "-";
        }

        for( index = size - 1; index >= 0; index-- )
        {
             outstr += toBinaryString( digits[ index ] );
        }
    }

    return outstr;
}




//////////////////////////// operators ////////////////////////////////////////
////////////// shifting ///////////////
void LargeInt::digitShiftLesser( int shiftAmount )
{
    unsigned int newInd, oldInd;

    // shift greater instead if shiftAmount is negative
    if( shiftAmount < 0 )
    {
        digitShiftGreater( -shiftAmount );
        return;
    }

    // initial: new = start, old = start + shiftAmount
    // update: increment both
    // terminate: old out of upper bound
    for( newInd = 0, oldInd = shiftAmount; 
             oldInd < size; 
                 newInd++, oldInd++ )
    {
        // store old value into new position
        digits[ newInd ] = digits[ oldInd ];
    }

    // decrement size by shiftAmount
    size -= shiftAmount;
}


void LargeInt::digitShiftGreater( int shiftAmount )
{
    int oldInd, oldSize;

    // shift lesser instead if shiftAmount is negative
    if( shiftAmount < 0 )
    {
        digitShiftLesser( -shiftAmount );
        return;
    }

    // do nothing if value is 0 (keep size consistent)
    if( size == 0 )
    {
        return;
    }

    // add shiftAmount to size
    oldSize = size;
    resize( oldSize + shiftAmount );

    // initial: old = size - 1
    // update: old -= 1
    // terminate: oldInd out of lower bound
    for( oldInd = oldSize - 1; oldInd >= 0; oldInd-- )
    {
        // store old value into new space (new = old + shiftAmount)
        digits[ oldInd + shiftAmount ] = digits[ oldInd ];
    }

    // store 0 in remaining spaces
    for( oldInd = oldInd + shiftAmount; oldInd >= 0; oldInd-- )
    {
        digits[ oldInd ] = 0;
    }
}

void operator <<= ( LargeInt &toShift, int shiftAmount )
{
    unsigned int shiftDigits, shiftBits;
    unsigned int currentRemainder, nextRemainder;
    unsigned int digitInd;

    // calculate digits to shift and bits to shift
    shiftDigits = shiftAmount / LI_Properties::digit::SIZE;
    shiftBits = shiftAmount % LI_Properties::digit::SIZE;

    // shift by digits
    toShift.digitShiftGreater( shiftDigits );

    if( shiftBits != 0 )
    {
        // add an extra digit to the end (may be removed later if unneeded)
        toShift.resize( toShift.size + 1 );

        // first digit has remainder 0
        currentRemainder = 0;

        // shift bits in each digit by bits to shift (skip last)
        for( digitInd = 0; digitInd < toShift.size; digitInd++ )
        {
            nextRemainder = toShift.digits[ digitInd ] >> 
                            ( LI_Properties::digit::SIZE - shiftBits );
            toShift.digits[ digitInd ] = currentRemainder + 
                                         ( toShift.digits[ digitInd ] << shiftBits );
            currentRemainder = nextRemainder;
        }

        toShift.removeLeadingZeros();
    }
}

void operator >>= ( LargeInt &toShift, int shiftAmount )
{
    unsigned int shiftDigits, shiftBits;
    unsigned int currentRemainder, nextRemainder;
    int digitInd;

    // calculate digits to shift and bits to shift
    shiftDigits = shiftAmount / LI_Properties::digit::SIZE;
    shiftBits = shiftAmount % LI_Properties::digit::SIZE;

    // shift by digits
    toShift.digitShiftLesser( shiftDigits );

    if( shiftBits != 0 )
    {
        // last digit has remainder 0
        currentRemainder = 0;

        // shift bits in each digit by bits to shift (skip last)
        for( digitInd = toShift.size - 1; digitInd >= 0; digitInd-- )
        {
            nextRemainder = toShift.digits[ digitInd ] << 
                                 ( LI_Properties::digit::SIZE - shiftBits );
            toShift.digits[ digitInd ] = ( toShift.digits[ digitInd ] >> shiftBits )
                                          + currentRemainder;
            currentRemainder = nextRemainder;
        }

        // remove leading zeros
        toShift.removeLeadingZeros();
    }
}


LargeInt operator<<( LargeInt &toShift, int shiftVal )
{
    LargeInt result = toShift;
    result <<= shiftVal;
    return result;
}

LargeInt operator>>( LargeInt &toShift, int shiftVal )
{
    LargeInt result = toShift;
    result >>= shiftVal;
    return result;
}



////////////// adding ///////////////
LargeInt operator+( const LargeInt &one, const LargeInt &other )
{
    LargeInt result;
    const LargeInt *greater, *smaller;

    // signs are the same: direct magnitude addition
    if( one.sign == other.sign )
    {
        result = addMagnitude( one, other );
        result.sign = one.sign;
        return result;
    }
    // different signs

    // identify greater/smaller LIs
    if( one < other )
    {
        greater = &one;
        smaller = &other;
    }
    else
    {
        greater = &other;
        smaller = &one;
    }

    // subtract magnitude of smaller from greater
    result = subtractMagnitude( *greater, *smaller );

    // set result sign to sign of the greater value
    result.sign = greater->sign;

    return result;
}


// adds magnitude of two LargeInts
// sign is ignored, and the result sign is positive
LargeInt addMagnitude( const LargeInt &one, const LargeInt &other )
{
    LargeInt result = LargeInt();
    const LargeInt *larger, *smaller;
    unsigned int remainder;
    unsigned int digitInd;

    // extract larger/smaller
    if( one.size >= other.size )
    {
        larger = &one;
        smaller = &other;
    }
    else
    {
        larger = &other;
        smaller = &one;
    }

    // resize for one more than the larger
    result.reallocate( larger->size + 1 );
    result.resize( larger->size );

    // start with no remainder
    remainder = 0;
    // add smaller/larger digits and remainder into result
    for( digitInd = 0; digitInd < smaller->size; digitInd++ )
    {
        result.digits[ digitInd ] = smaller->digits[ digitInd ] + 
                                    larger->digits[ digitInd ] + 
                                    remainder;
        // calculate remainder
        if( remainder )
        {
            // remainder occurred if result is less than one
            // (because a remainder should be added, overflow also occurs if equal)
            remainder = result.digits[ digitInd ] <= one.digits[ digitInd ]; 
        }
        else
        {
            // remainder occurred if result is less than one
            remainder = result.digits[ digitInd ] < one.digits[ digitInd ];
        }
    }

    // continue to end of larger digit
    while( digitInd < larger->size )
    {
        result.digits[ digitInd ] = larger->digits[ digitInd ] + remainder;
        remainder &= larger->digits[ digitInd ] == LI_Properties::digit::MAX;

        digitInd++;
    }

    if( remainder )
    {
        // store remainder at last digit if necessary
        result.digits[ result.size ] = 1; // safe due to excessive previous allocation
        result.size += 1;
    }

    return result;
}

void LargeInt::addDigitAtIndex( LI_Properties::digit::type toAdd, 
                                unsigned int addIndex )
{
    bool remainder;
    if( size <= addIndex )
    {
        // +1 to include index, +1 for possible overflow
        resize( addIndex + 1 + 1 );
    }
    else
    {
        // +1 for possible overflow
        resize( size + 1 );
    }

    digits[ addIndex ] += toAdd;
    remainder = digits[ addIndex ] < toAdd;
    while( remainder )
    {
        addIndex++;
        digits[ addIndex ] += remainder;
        remainder = digits[ addIndex ] == (LI_Properties::digit::type)0;
    }

    // remove leading zeros (due to oversizing)
    removeLeadingZeros();
}


////////////// subtracting ///////////////

// returns larger - smaller
// requirement: first must be greater (or equal) than second
//  - if not met, undefined behavior
LargeInt subtractMagnitude( const LargeInt &larger, const LargeInt &smaller )
{
    LargeInt result = LargeInt();
    unsigned int digitIndex;
    unsigned int owe;

    // resize to larger's size (reduce later if leading zeros)
    result.resize( larger.size );

    // initialize owe as 0
    owe = 0;

    // iterate over digits small->large
    for( digitIndex = 0; digitIndex < smaller.size; digitIndex++ )
    {
        result.digits[ digitIndex ] = larger.digits[ digitIndex ]
                                       - smaller.digits[ digitIndex ]
                                       - owe;
        if( !owe )
        {
            owe = smaller.digits[ digitIndex ] > larger.digits[ digitIndex ];
        }
        else
        {
            owe = smaller.digits[ digitIndex ] >= larger.digits[ digitIndex ];
        }
    }

    // iterate over remaining digits
    while( digitIndex < larger.size )
    {
        // set to larger - owe
        result.digits[ digitIndex ] = larger.digits[ digitIndex ] - owe;

        // recalculate owe
        owe &= larger.digits[ digitIndex ] == 0;
        digitIndex++;
    }

    // if still owe, invalid input: do not handle

    // remove leading zeros
    result.removeLeadingZeros();

    return result;
}

LargeInt operator-( LargeInt &first, LargeInt &second )
{
    LargeInt result;
    const LargeInt *smaller, *larger;

    // check for diffferent sign
    if( first.sign != second.sign )
    {
        // (second sign is flipped to have same sign)
        // add magnitude of first/second
        result = addMagnitude( first, second );

        // set result's sign to first's sign, return
        result.sign = first.sign;
        return result;
    }
    // otherwise, same sign

    // extract larger/smaller
    if( first > second )
    {
        larger = &first;
        smaller = &second;
    }
    else
    {
        larger = &second;
        smaller = &first;
    }
    
    // subtract magnitude of smaller from larger
    result = subtractMagnitude( *larger, *smaller );

    // set sign to larger, return
    result.sign = larger->sign;
    return result;
}


////////////// comparing ///////////////
// returns positive if first is greater, 
//    negative if second is greater, 
//    zero if equal
int spaceshipMagComp( const LargeInt &first, const LargeInt &second )
{
    int digitIndex;

    // case different size
    if( first.size != second.size )
    {
        // return differnece in sizes
        return first.size - second.size;
    }
    // otherwise, same size

    // iterate over digits (most significant->least)
    for( digitIndex = first.size - 1; digitIndex >= 0; digitIndex-- )
    {
        // end if differnece found
        if( first.digits[ digitIndex ] != second.digits[ digitIndex ] )
        {
            // DO NOT RETURN SUBTRACT: (unsigned integers)
            if( first.digits[ digitIndex ] > second.digits[ digitIndex ] )
            {
                return 1;
            }
            return -1;
        }
    }

    // return no difference (all digits equal)
    return 0; 
}

int spaceshipComp( const LargeInt &first, const LargeInt &second )
{
    // check signs are different
    if( first.sign != second.sign )
    {
        // first has negative sign
        if( first.sign )
        {
            return -1;
        }
        return 1;
    }

    // set to reverse if negative
    if( first.sign )
    {
        // return reversed magnitude comparison
        return -spaceshipMagComp( first, second );
    }

    // return normal comparison
    return spaceshipMagComp( first, second );
}




bool operator<=( const LargeInt &first, const LargeInt &second )
{
    return spaceshipComp( first, second ) <= 0;
}


bool operator>=( const LargeInt &first, const LargeInt &second )
{
    return spaceshipComp( first, second ) >= 0;
}


bool operator>( const LargeInt &first, const LargeInt &second )
{
    return spaceshipComp( first, second ) > 0;
}


bool operator<( const LargeInt &first, const LargeInt &second )
{
    return spaceshipComp( first, second ) < 0;
}


bool operator==( const LargeInt &first, const LargeInt &second )
{
    return spaceshipComp( first, second ) == 0;
}


bool operator!=( const LargeInt &first, const LargeInt &second )
{
    return spaceshipComp( first, second ) != 0;
}



///////////// multiplication ///////////////
void operator*=( LargeInt &one, LI_Properties::digit::type other )
{
    LI_Properties::digit::type low, high;
    unsigned int index;
    unsigned int overflow = 0; // start with no overflow

    // return '0' if multiplying by 0
    if( other == 0 )
    {
        one = LargeInt( 0 );
        return;
    }

    for( index = 0; index < one.size; index++ )
    {
        multiplyDigits( one.digits[ index ], other, low, high );
        // add previous overflow and current low
        one.digits[ index ] = overflow + low;

        // store new overflow (accounting for remainder from previous)
        overflow = high + ( one.digits[ index ] < low );
    }

    if( overflow )
    {
        one.resize( one.size + 1 );
        one.digits[ index ] = overflow;
    }

    // set sign if different signs
    one.sign = one.sign != ( other < 0 );
}

void operator*=( LargeInt &first, const LargeInt &second )
{
    first = first * second;
}

LargeInt operator*( const LargeInt &one, const LI_Properties::digit::type other )
{
    LargeInt result = one;
    result *= other;

    return result;
}

LargeInt operator*( const LargeInt &one, const LargeInt &other )
{
    LargeInt result = LargeInt();

    result = multiplyLIMagnitude( one, other );


    // (negative) sign if different signs
    result.sign = one.sign != other.sign;

    return result;
}

LargeInt multiplyLIMagnitude( const LargeInt &one, const LargeInt &other )
{
    // case size is one or zero (terminate recursion)
        // case zero
            // return 0
        // case one
            // digit multiplication
    if( one.size <= 1 )
    {
        if( one.size == 0 )
        {
            return LargeInt( 0 );
        }
        return other * one.digits[ 0 ];
    }
    if( other.size <= 1 )
    {
        if( other.size == 0 )
        {
            return LargeInt( 0 );
        }
        return one * other.digits[ 0 ];
    }
    if( one.size < 22 || other.size < 22 )
    {
        return gradeschoolMagMult( one, other );
    }

    // otherwise, recurse by splitting
    LargeInt oneLow, oneHigh, otherLow, otherHigh;
    LargeInt lowProd, highProd, firstMixed, secondMixed;
    unsigned int splitInd;

    // identify split index: lowest half of both (cannot be 0 due to termination)
    splitInd = min( one.size/2, other.size/2 );

    // split based on the split index found
    one.shallowSplit( oneLow, oneHigh, splitInd );
    other.shallowSplit( otherLow, otherHigh, splitInd );

    // multiply alternating permutations of high/low
    lowProd = multiplyLIMagnitude( oneLow, otherLow );
    highProd = multiplyLIMagnitude( oneHigh, otherHigh );
    firstMixed = multiplyLIMagnitude( oneLow, otherHigh );
    secondMixed = multiplyLIMagnitude( oneHigh, otherLow );

    // weight products according to splitInd
    // low does not need to be shifted
    highProd.digitShiftGreater( splitInd * 2 );
    firstMixed.digitShiftGreater( splitInd );
    secondMixed.digitShiftGreater( splitInd );

    // store original data in shallow copies
    oneLow.digits = NULL;
    otherLow.digits = NULL;
    oneHigh.digits = NULL;
    otherHigh.digits = NULL;
    // return sum of products
// TODO: do not use positive operator when sign has undefined value
    return lowProd + highProd + firstMixed + secondMixed;
}

LargeInt gradeschoolMagMult( const LargeInt &one, const LargeInt &other )
{
    LargeInt result = LargeInt( 0 );
    const LargeInt *larger, *smaller;
    unsigned int wkgSmallerInd;
    LargeInt addValue;

    if( one.size >= other.size )
    {
        larger = &one;
        smaller = &other;
    }
    else
    {
        larger = &other;
        smaller = &one;
    }

    for( wkgSmallerInd = 0; wkgSmallerInd < smaller->size; wkgSmallerInd++ )
    {
// TODO: make more efficient
        addValue = *larger * smaller->digits[ wkgSmallerInd ];
        addValue.digitShiftGreater( wkgSmallerInd );

        result = addMagnitude( result, addValue );
    }

    return result;
}






/////////// division ////////////
LargeInt operator/( const LargeInt &numerator, const LargeInt &denominator )
{
    LargeInt divisionResult, remainder;
    divideLIMagnitude( numerator, denominator, divisionResult, remainder );
    divisionResult.sign = numerator.sign != denominator.sign;

    return divisionResult;  // temp stub return
}


void divideLIMagnitude( const LargeInt &numerator, const LargeInt &denominator, 
                            LargeInt &divisionResult, LargeInt &remainder )
{
    LargeInt wkgSubtractor;
    LargeInt &wkgNumerator = remainder; // wkgNumerator is remainder at end
    wkgNumerator = numerator; // deep copy of numerator
// TODO: make more efficient
    LargeInt wkgAdditive;

    // set division result to 0
    divisionResult = LargeInt( 0 );

    // set wkgSubtractor to denominator
    wkgSubtractor = denominator;
    // shift greater such that 
    //    wkgSubtractor's largest digit is one above numerator's largest digit
    wkgSubtractor.digitShiftGreater( numerator.size - denominator.size + 1 );
    // shift additive's bit equally
    wkgAdditive = LargeInt( 1 );
    wkgAdditive.digitShiftGreater( numerator.size - denominator.size + 1 );

    // stop when subtractor is less than denominator
    while( spaceshipMagComp( wkgSubtractor, denominator ) >= 0 )
    {
        // subtract numerator is at least wkgSubtractor
        if( spaceshipMagComp( wkgNumerator, wkgSubtractor ) >= 0 )
        {
            // subtract wkgSubtractor from wkgNumerator
            wkgNumerator = subtractMagnitude( wkgNumerator, wkgSubtractor );
            divisionResult = addMagnitude( divisionResult, wkgAdditive );
// TODO: make more efficient
        }

        // shfit wkgSubtractor less significant
        wkgSubtractor >>= 1;
        wkgAdditive >>= 1;
// TODO: make more efficient
    }


}


/////////////////////////////////// LI display ////////////////////////////////
std::string LargeInt::toStringBruteForce( unsigned int base, 
                                          unsigned int forceSize ) const
{
    if( size == 0 )
    {
        return (std::string)"0";
    }

    LargeInt copy = *this;
    LargeInt baseLI = LargeInt( base );
    LargeInt divideResult, remainder;
    std::string wkgStr;

    while( copy.size != 0 )
    {
        divideLIMagnitude( copy, baseLI, divideResult, remainder );
        wkgStr.push_back( intToChar( (int)remainder ) );

        copy = divideResult;
    }
    std::reverse(wkgStr.begin(), wkgStr.end());
    if( forceSize > wkgStr.size() )
    {
        wkgStr = std::string( forceSize - wkgStr.size(), '0' ) + wkgStr;
    }
    return wkgStr;
}

std::string LargeInt::toString( unsigned int base ) const
{
    std::string resultStr;
    if( sign )
    {
        resultStr.push_back( '-' );
    }
    resultStr += stringMagnitude( base, 0 );

    return resultStr;
}

std::string LargeInt::stringMagnitude( unsigned int base, unsigned int forceSize ) const
{
    unsigned int baseDigits;
    LargeInt divisionResult, remainder;
    unsigned int smallerSize, largerSize;

    // if reasonable size, terminate
    if( size <= 1 )
    {
        // terminate recursion, returning last digit forced to size
        if( size == 0 )
        {
            // return forced size zeros
            return std::string( forceSize, '0' );
        }
        // not 0: brute force
        return toStringBruteForce( base, forceSize );
    }

    // estimate number of digits in base
        // log<base>( <digit max> )
    baseDigits = (unsigned int)
                 (   
                     (double)LI_Properties::digit::SIZE / 
                     (double)std::log2( (double)base ) *
                     (double)size
                 ); // rounding up

    // identify sizes of the large/small
    smallerSize = baseDigits / 2;
    largerSize = max( (int)forceSize - (int)smallerSize, 0 );

    // identify divideValue
    LargeInt divideValue = toPower( LargeInt( base ), smallerSize );

    // divide self by 10^(num_digits/2), store divisionResult and remainder
    divideLIMagnitude( *this, divideValue, divisionResult, remainder );

    std::string greaterString = divisionResult.stringMagnitude( base, largerSize );
    std::string smallerString = remainder.stringMagnitude( base, smallerSize );

    // return greater->smaller
    return greaterString + smallerString;
}

















//////////////////////////////// generic functions ////////////////////////////

char intToChar( int testInt )
{
    if( testInt <= 9 )
    {
        return (char)testInt + '0';
    }
    return testInt + 'A';
}

void multiplyDigits( LI_Properties::digit::type one, 
                     LI_Properties::digit::type other, 
                     LI_Properties::digit::type &low, 
                     LI_Properties::digit::type &high )
{
    // use double size to combine digits
    LI_Properties::digit::doubleSize::type combinedDigits;
    combinedDigits = (LI_Properties::digit::doubleSize::type)one * 
                     (LI_Properties::digit::doubleSize::type)other;

    // extract lower/upper
    low = (LI_Properties::digit::type)
          ( 
             combinedDigits & 
             (LI_Properties::digit::doubleSize::type)LI_Properties::digit::MAX
          );
    high = (LI_Properties::digit::type)
           (
              combinedDigits >> LI_Properties::digit::SIZE
           );
}





unsigned int charToInt( char charVal )
{
    if( charVal >= '0' && charVal <= '9' )
    {
        return charVal - '0';
    }
    if( charVal >= 'a' && charVal <= 'z' )
    {
        return charVal - 'a' + 10;
    }
    if( charVal >= 'A' && charVal <= 'Z' )
    {
        return charVal - 'A' + 10;
    }
//    throw std::bad_cast::bad_cast( (std::string)"Invalid Character" );
    throw;
}







