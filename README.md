# Todos:

* consider abandon: alignment: right, left, and leave it for the user when defining fields
* research (e.g. ask on reddit) whether switching alignment (-> or <-) at runtime would be ever used
* field ordering,
* overflow policy: allow, throw, clear field,
* straddling: types of single bitfields is different than underlying type
* document: defined alignment, padding, field ordering
* rename Bitfield to BitfieldGroup

# To test:

* Document works with `enum class`, `enum`, integers ... or any other structural type, as field ids.
* Document how to implement padding using "reserved" fields, when one doesn't want to occupy the whole field.
* Document how to get a mask().
* Document testing with disabled portability checks, because makes the build slow, due to LLVM download.
* PROBLEM WITH CATCH2: any test with one-byte-long bitfield group, shall be templated test case, with big endian and 
little endian configuration.

