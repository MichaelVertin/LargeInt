// compile:
//   g++ -Wall LargeInt_test.cpp LargeInt.cpp -o outfile


#include "LargeInt.h"
#include <iostream>
#include <stdio.h>


int main()
{
/*
    LargeInt negInt = LargeInt( -12 );
    LargeInt posInt = LargeInt( 15 );
    LargeInt copy;
    LargeInt tempLI = LargeInt();
    LargeInt zeroInt = LargeInt( 0 );
    LargeInt undInt = LargeInt();
    LargeInt myInt;
*/

    LargeInt test = LargeInt("12");
    LargeInt test2 = LargeInt("-15");
    std::cout << (test + test2).toBinary() << "\n";
    std::cout << "\n\n\n";
/*
    std::cout << negInt.toBinary() << "\n";
    if( negInt.toBinary() != (std::string)"-00000000000000000000000000001100" )
            {std::cout << "ERROR: negative test\n";}
    std::cout << posInt.toBinary() << "\n";
    if( posInt.toBinary() != (std::string)"00000000000000000000000000001111" )
            {std::cout << "ERROR: positive test\n";}
    std::cout << zeroInt.toBinary() << "\n";
    if( zeroInt.toBinary() != (std::string)"0" )
            {std::cout << "ERROR: zero test\n";}
    std::cout << undInt.toBinary() << "\n";

    negInt.digitShiftLesser( 1 );
    std::cout << negInt.toBinary() << "\n";

    posInt.digitShiftGreater( 2 );
    std::cout << posInt.toBinary() << "\n";

    posInt >>= 5;
    std::cout << posInt.toBinary() << "\n";

    std::cout << "---------------------------- testing shifting--------------\n";
    for( int shift = 0; shift < 50; shift++ )
    {
        std::cout << ( posInt >> shift ).toBinary() << "\n";
    }
    copy.digitShiftGreater( 1 );
    posInt = copy;

    posInt = LargeInt( 21 );
    posInt.digitShiftGreater( 2 );
    posInt >>= 3;
    for( int shift = 0; shift < 50; shift++ )
    {
        std::cout << ( posInt >> shift ).toBinary() << "\n";
    }

    for( int shift = 0; shift < 50; shift++ )
    {
        std::cout << ( posInt >> shift ).toBinary() << "\n";
    }

    std::cout << "-------------------- greater shifting---------------\n";
    posInt = LargeInt( 21 );
    posInt.digitShiftGreater( 1 );
    posInt >>= 3;
    for( int shift = 0; shift < 50; shift++ )
    {
        std::cout << ( posInt << shift ).toBinary() << "\n";
    }

    std::cout << "-------------------- adding self to self ---------------\n";
    for( int i = 0; i < 10; i++ )
    {
        posInt = posInt + posInt;
        std::cout << posInt.toBinary() << "\n";
    }

    std::cout << "-------------------- subtracting ----------------------\n";
    posInt = LargeInt( 21 );
    posInt.digitShiftGreater( 2 );
    posInt >>= 3;
    negInt = LargeInt( ( LI_Properties::digit::MAX >> 3 ) + 1 );
    tempLI = LargeInt( 0 );
    for( int i = 0; i < 100; i++ )
    {
        std::cout << subtractMagnitude( posInt, tempLI ).toBinary() << "\n";
        tempLI = tempLI + negInt;
    }


    std::cout << "---------------------- multiply digits -----------------\n";
    LI_Properties::digit::type par1, par2;
    LI_Properties::digit::type low, high;
    par1 = 0xA5B2C3D4;
    par2 = 0x4D3C2B5A;
    multiplyDigits( par1, par2, low, high );
    std::cout << toBinaryString( par1 ) << "\n";
    std::cout << toBinaryString( par2 ) << "\n";
    std::cout << toBinaryString( high ) << " ";
    std::cout << toBinaryString( low ) << "\n";

    std::cout << "------------------ multiply single digit --------------\n";

    std::cout << "multiplying\n    " << posInt.toBinary() << "\n    " 
              << negInt.toBinary() << "\n";
    negInt = posInt * negInt;
    std::cout << negInt.toBinary() << "\n";

    negInt = posInt * negInt;
    std::cout << negInt.toBinary() << "\n";

    std::cout << "------------------- testing multiplication -------------\n";
    LargeInt myInt = LargeInt( "10" );
    std::cout << myInt.toBinary() << "\n";
    myInt = LargeInt( "-10" );
    std::cout << myInt.toBinary() << "\n";
    myInt = LargeInt( "4294967295" );
    std::cout << myInt.toBinary() << "\n";
    myInt = LargeInt( "4294967296" );
    std::cout << myInt.toBinary() << "\n";
    myInt = LargeInt( "12345678900987654321" );
    std::cout << myInt.toBinary() << "\n";
    myInt = LargeInt( (std::string)"0AF0AF0AF0AF", 16 );
    std::cout << myInt.toBinary() << "\n";

    myInt = LargeInt( "123456789" ) * LargeInt( "123456789" );
    std::cout << myInt.toString() << "\n";


    std::cout << "-------------------- debugging multiplication-----------\n";
    LargeInt result = LargeInt( "79766443076872509863361" );
    LargeInt baseMultiplier = LargeInt( "79766443076872509863361" );
    std::cout << "multiplying\n";
    result *= baseMultiplier;
    std::cout << "done multiplying\n";

    std::cout << "--------------------- testing division ----------------\n";
    myInt = negInt / posInt;
    std::cout << negInt.toBinary() << "\n";
    std::cout << posInt.toBinary() << "\n";
    std::cout << myInt.toBinary() << "\n";
    std::cout << myInt.toString() << "\n";
    std::cout << LargeInt( "1234567890987654321" ).toString() << "\n";
*/
    std::cout << "------------------------- testing power --------------------\n";
/*
    myInt = toPower( LargeInt( 123 ), 10 );
    std::cout << myInt.toString() << "\n";
    std::cout << "calculating 3^20000...";
    LargeInt powerResult = toPower( LargeInt( 3 ), 100 );
    std::cout << "done\n";
    std::cout << powerResult.toString() << "\n";


    std::cout << toPower( LargeInt( 3 ), 100 ).toStringBruteForce() << "\n";

    std::cout << "123 ^ 13\n";
    LargeInt val123 = LargeInt( 123 );
    std::cout << toPower( val123, 13 ).toStringBruteForce() << "\n";
    std::cout << toPower( val123, 13 ).toBinary() << "\n";
    for( int i = 0; i < 00; i++ )
    {
        myInt = toPower( LargeInt( 123 ), i );
        std::cout << i << ":\n";
        std::cout << myInt.toString() << "\n";
        std::cout << myInt.toStringBruteForce() << "\n\n";
    }
*/
    LargeInt myInt;
    /*
    for( int i = 0; i < 10; i++ )
    {
        std::cout << "calculating 846 ^ 1000...";
        myInt = toPower( LargeInt( 846 ), 11500 );
        std::cout << "done\n";
    }
    */
    myInt = toPower( LargeInt( 846 ), 11500 );
    std::cout << "printing: \n";
    std::cout << myInt.toString() << "\n";
/*
    LargeInt myInt, otherInt;
    myInt = LargeInt( 1234 );
    myInt.toString();
    std::cout << myInt.toString() << "\n";
*/

    std::cout << "\n\nProgram End\n";
}









