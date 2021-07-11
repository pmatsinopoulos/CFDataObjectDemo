//
//  main.c
//  CFDataCreateDemo
//
//  Created by Panayotis Matsinopoulos on 11/7/21.
//

#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>
#include "Customer.h"

void printCustomer(Customer *customer) {
  assert(customer);
  
  printf("Name: %s, Grade: %d, Salary: %d\n",
         CFStringGetCStringPtr(customer->name, CFStringGetSystemEncoding()),
         customer->grade, customer->salary);
}

void printArrayOfCustomers(Customer customers[], UInt8 numberOfCustomers) {
  assert(customers);
  
  for(UInt8 i = 0; i < numberOfCustomers; i++) {
    printf("Customer %d:", i);
    printCustomer((Customer *)&customers[i]);
  }
}

int main(int argc, const char * argv[]) {
  Customer customer;
  customer.name = CFStringCreateCopy(kCFAllocatorDefault, CFSTR("Peter Pan"));
  customer.grade = 32000;
  customer.salary = 800;
  
  UInt8 *buffer = (UInt8 *)&customer;
  CFIndex bufferSize = sizeof(Customer);
  
  CFDataRef cfCustomer = CFDataCreate(kCFAllocatorDefault, buffer, bufferSize);
  
#pragma mark Inspect CFDataRef Object
  const UInt8 *pToCFDataObject = CFDataGetBytePtr(cfCustomer);
  
  printf("Customer in CFDataRef: ");
  printCustomer((Customer*) pToCFDataObject);
  printf("Customer in customer: ");
  printCustomer(&customer);
  
  printf("Multiplying 'customer' salary by 2 (does not affect the customer behind CFDataRef\n");
  customer.salary *= 2;

  printf("Customer in CFDataRef: ");
  printCustomer((Customer*) pToCFDataObject);
  printf("Customer in customer: ");
  printCustomer(&customer);
  
#pragma mark Copy Bytes to External Storage
  Customer customer2;
  CFDataGetBytes(cfCustomer, CFRangeMake(0, CFDataGetLength(cfCustomer)), (UInt8 *)&customer2);
  printf("customer2: ");
  printCustomer(&customer2);
  
#pragma mark Releasing CFDataRef wrapped object
  CFRelease(cfCustomer);
  
#pragma mark Wrapping an Array to CFDataRef
  const int numberOfCustomers = 4;
  Customer customers[numberOfCustomers] = {
    { .name = CFStringCreateCopy(kCFAllocatorDefault, CFSTR("Peter Pan")), .grade = 101, .salary = 10 },
    { .name = CFStringCreateCopy(kCFAllocatorDefault, CFSTR("Mary Woo")), .grade = 102, .salary = 11 },
    { .name = CFStringCreateCopy(kCFAllocatorDefault, CFSTR("Paul Adams")), .grade = 103, .salary = 13 },
    { .name = CFStringCreateCopy(kCFAllocatorDefault, CFSTR("Harry Potter")), .grade = 104, .salary = 14 }
  };
  CFIndex customersSize = sizeof(Customer) * numberOfCustomers;
#pragma mark Printing Customers from the Array
  printf("Customers from the array:\n");
  printArrayOfCustomers(customers, numberOfCustomers);

#pragma mark Creating CFDataRef holding an array of customers
  CFDataRef cfArrayOfCustomers = CFDataCreate(kCFAllocatorDefault, (const UInt8 *)customers, customersSize);

#pragma mark Getting Access to Array Customers in CFDataRef
  pToCFDataObject = CFDataGetBytePtr(cfArrayOfCustomers);
  printf("Customers in the CFDataRef Array of Customers:\n");
  printArrayOfCustomers((Customer *)pToCFDataObject, numberOfCustomers);

#pragma mark Find a Data Item in the CFDataRef with Array of Customers
  CFDataRef customerToFind = CFDataCreate(kCFAllocatorDefault, (const UInt8 *)(&customers[3]), sizeof(Customer));
  CFRange found = CFDataFind(cfArrayOfCustomers, customerToFind, CFRangeMake(0, CFDataGetLength(cfArrayOfCustomers)), 0);
  CFRelease(customerToFind);
  printf("Found: Position: %ld, %ld\n", found.location / sizeof(Customer), found.length);

#pragma mark Get copy of data from the CFDataRef that has an Array of Customers
  Customer customers2[numberOfCustomers];
  CFDataGetBytes(cfArrayOfCustomers, CFRangeMake(0, CFDataGetLength(cfArrayOfCustomers)), (UInt8 *)customers2);
  printf("Printing customers2:\n");
  printArrayOfCustomers(customers2, numberOfCustomers);

#pragma mark Releasing the CFDataRef Holding the Array of Customers
  CFRelease(cfArrayOfCustomers);
  
  return 0;
}
