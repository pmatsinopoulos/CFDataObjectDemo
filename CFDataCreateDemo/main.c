//
//  main.c
//  CFDataCreateDemo
//
//  Created by Panayotis Matsinopoulos on 11/7/21.
//

#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>
#include "Customer.h"

int main(int argc, const char * argv[]) {
  Customer customer;
  customer.name = CFStringCreateCopy(kCFAllocatorDefault, CFSTR("Peter Pan"));
  customer.grade = 32000;
  customer.salary = 800;
  
  CFDataRef cfCustomer = CFDataCreate(kCFAllocatorDefault, (UInt8 *)&customer, sizeof(Customer));
  
#pragma mark Inspect CFDataRef Object
  const UInt8 *pToCFDataObject = CFDataGetBytePtr(cfCustomer);
  
  printf("Customer in CFDataRef: ");
  CustomerPrint((Customer*) pToCFDataObject);
  printf("Customer in customer: ");
  CustomerPrint(&customer);
  
  printf("Multiplying 'customer' salary by 2 (does not affect the customer behind CFDataRef\n");
  customer.salary *= 2;

  printf("Customer in CFDataRef: ");
  CustomerPrint((Customer*) pToCFDataObject);
  printf("Customer in customer: ");
  CustomerPrint(&customer);
  
#pragma mark Copy Bytes to External Buffer
  Customer customer2;
  CFDataGetBytes(cfCustomer, CFRangeMake(0, CFDataGetLength(cfCustomer)), (UInt8 *)&customer2);
  printf("customer2: ");
  CustomerPrint(&customer2);
  
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
  CustomerPrintArrayOf(customers, numberOfCustomers);

#pragma mark Creating CFDataRef holding an array of customers
  CFDataRef cfArrayOfCustomers = CFDataCreate(kCFAllocatorDefault, (const UInt8 *)customers, customersSize);

#pragma mark Getting Access to Array Customers in CFDataRef
  pToCFDataObject = CFDataGetBytePtr(cfArrayOfCustomers);
  printf("Customers in the CFDataRef Array of Customers:\n");
  CustomerPrintArrayOf((Customer *)pToCFDataObject, numberOfCustomers);

#pragma mark Find a Data Item in the CFDataRef with Array of Customers
  CFDataRef customerToFind = CFDataCreate(kCFAllocatorDefault, (const UInt8 *)(&customers[3]), sizeof(Customer));
  CFRange found = CFDataFind(cfArrayOfCustomers, customerToFind, CFRangeMake(0, CFDataGetLength(cfArrayOfCustomers)), 0);
  CFRelease(customerToFind);
  printf("Found: Location: %ld, Position: %ld, Length: %ld\n", found.location, found.location / sizeof(Customer) + 1, found.length);

#pragma mark Get copy of data from the CFDataRef that has an Array of Customers
  Customer customers2[numberOfCustomers];
  CFDataGetBytes(cfArrayOfCustomers, CFRangeMake(0, CFDataGetLength(cfArrayOfCustomers)), (UInt8 *)customers2);
  printf("Printing customers2:\n");
  CustomerPrintArrayOf(customers2, numberOfCustomers);

#pragma mark Releasing the CFDataRef Holding the Array of Customers
  CFRelease(cfArrayOfCustomers);
  
#pragma mark Working with CFMutableData
  CFMutableDataRef cfMutableCustomers = CFDataCreateMutable(kCFAllocatorDefault, 0);
  
  CFDataAppendBytes(cfMutableCustomers, (const UInt8 *)&customers2, sizeof(Customer) * numberOfCustomers);
  
  Customer fifthCustomer = { .name = CFSTR("Panos Mats"), .grade = -100, .salary = 203 };
  CFDataAppendBytes(cfMutableCustomers, (const UInt8 *)&fifthCustomer, sizeof(Customer));
  
  Customer customers3[numberOfCustomers + 1];
  CFDataGetBytes(cfMutableCustomers, CFRangeMake(0, CFDataGetLength(cfMutableCustomers)), (UInt8 *)customers3);
  
  printf("Printing customer3:\n");
  CustomerPrintArrayOf(customers3, numberOfCustomers + 1);
  
  return 0;
}
