//
//  Customer.h
//  CFDataCreateDemo
//
//  Created by Panayotis Matsinopoulos on 11/7/21.
//

#ifndef Customer_h
#define Customer_h

#include <CoreFoundation/CoreFoundation.h>

typedef struct {
  CFStringRef name;
  int grade;
  short salary;
} Customer;

#endif /* Customer_h */
