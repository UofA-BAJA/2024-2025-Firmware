/* 
 * Not much here, just initializing the car.
 * This file is the only place where there should
 * be a main function.
 * 
 * Author: Matthew Larson
 */

/*
 *
 *  Main
 *
 *         Author:  Matthew Larson
 *
 *        Purpose:  Is the beginning of the program, where everything that ever happens starts from. 
 *                  It's the bottom of the stack. It's the beginning of the heap. It's the answer to life, the universe, and everything 
 *
 *  Inherits From:  Nothing, because it *is* the beginning
 *
 *     Interfaces:  None 
 *
 *+-----------------------------------------------------------------------
 *
 *      Constants:  None
 *
 *+-----------------------------------------------------------------------
 *
 *        Methods:  main() - Warps the car into existence
 *
*/

#include "Car.h" 

int main(){

    BajaWildcatRacing::Car car = BajaWildcatRacing::Car();

    return 0;
}