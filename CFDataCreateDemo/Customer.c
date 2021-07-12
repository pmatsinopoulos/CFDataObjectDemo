//
//  Customer.c
//  CFDataCreateDemo
//
//  Created by Panayotis Matsinopoulos on 11/7/21.
//

#include "Customer.h"

void CustomerPrint(Customer *customer) {
  assert(customer);
  
  printf("Name: %s, Grade: %d, Salary: %d\n",
         CFStringGetCStringPtr(customer->name, CFStringGetSystemEncoding()),
         customer->grade, customer->salary);
}

void CustomerPrintArrayOf(Customer customers[], UInt8 numberOfCustomers) {
  assert(customers);
  
  for(UInt8 i = 0; i < numberOfCustomers; i++) {
    printf("Customer %d:", i);
    CustomerPrint((Customer *)&customers[i]);
  }
}
