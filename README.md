# Todos:

* consider abandon: alignment: right, left, and leave it for the user when defining fields
* research (e.g. ask on reddit) whether switching alignment (-> or <-) at runtime would be ever used
* field ordering,
* overflow policy: allow, throw, clear field,
* straddling: types of single bitfields is different than underlying type
* document: defined alignment, padding, field ordering
* rename Bitfield to BitfieldGroup

# To test:

* Works **only with** `enum class`, `enum`, integers ... as field ids.
* Static-assertion tests for missing field id.
* Field ids can't duplicate.
* Whole bitfield must be occupied (document how to implement padding with "reserved" fields).
* Test bitfields spanning across multiple bytes.
* Test bitfields aligned across multiple bytes, with weird spanning.
* Implement extract().
* Implement mask().
* Test overflowing with at() and extract().
* Test default zero-initialized.
* Extract() test on one-byte bitfield shall be templated test case, with big_endian and little_endian DDT.
