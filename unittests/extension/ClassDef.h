#ifndef ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H
#define ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H

#include "zapi/ZendApi.h"
#include <string>

using zapi::lang::StdClass;
using zapi::ds::Variant;
using zapi::ds::NumericVariant;
using zapi::ds::StringVariant;

class Person : public StdClass
{
public:
   Person();
//   Variant __get(const std::string &name);
   void __construct();
   void __destruct();
   void showName();
   //Variant __call(const std::string &method, Parameters &params) const;
   static void staticShowName();
   static StringVariant concatStr(const StringVariant &lhs, const StringVariant &rhs);
   void print_sum(NumericVariant argQuantity, ...);
   int addSum(NumericVariant argQuantity, ...);
   int addTwoNum(const NumericVariant &num1, const NumericVariant &num2);
private:
   /**
     *  The initial value
     *  @var    int
     */
   std::string name;
};

class Address : public StdClass
{
public:
   Address();
protected:
   std::string address;
};

#endif // ZAPI_UNITTEST_BRIDGE_DUMMYEXT_CLASS_DEF_H