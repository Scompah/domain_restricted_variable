# Domain Restricted Variable

An attempt at a Dynamic Enum data type.

## Installation

It's a header-only library, so just add the header file to your project and you're set (it's literally one file).

## Usage

### Structure Explanation

There are two classes forming this structure:

 1. VariableDomain: holds all valid values, like an enum declaration;
 2. DomainRestrictedVariable: The variable limited to assume only said values;

And both classes have two template parameters, that must be the same on both sides to be able to
link the class instances together:

 - value_type: the underlying type to be stored;
 - Compare: the comparison function used to order the values (defaults to `std::less<value_type>`)

Note that a DomainRestrictedVariable always needs a compatible VariableDomain, and cannot exist without one.

### Actual Usage

 1. Declare a VariableDomain of any type
 2. Fill it with values (or not)
 3. Declare any number of DomainRestrictedVariable(s) with the same template parameters
 4. Modify the values inside the variables to your liking/to satisfy your needs

## Why would you want to use this?

 - When you have a limited number of objects you want to use and only those objects
 - When you need to restrict a variable to certain values, but offer the possibility of changing them at run-time
 - It offers a simple interface for the aforementioned use cases
