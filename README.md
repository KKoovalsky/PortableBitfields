# Todos:

* alignment: right, left,
* field ordering,
* overflow policy: allow, throw, clear field,
* straddling: types of single bitfields is different than underlying type

# To test:

* Works **only with** `enum class`, `enum`, integers ... as field ids.
* Static-assertion tests for missing field id.
* Field ids can't duplicate.
* Test bitfields spanning across multiple bytes.
* Test bitfields aligned across multiple bytes, with weird spanning.
* Implement extract().
* Implement mask().
