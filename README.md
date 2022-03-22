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
* Static-assertion tests for missing field id.
* Field ids can't duplicate.
* Whole bitfield must be occupied (document how to implement padding with "reserved" fields).
* Document how to get a mask().
* Extract() test on one-byte bitfield shall be templated test case, with big_endian and little_endian DDT.
* Test with clang.
