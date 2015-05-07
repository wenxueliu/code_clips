proc getAlphabet(): string =
  var accm = ""
  for letter in 'a'..'z':
    accm.add(letter)
  return accm

proc getAlphabetPlus(): string =
  result = ""
  for letter in 'a'..'z':
    result.add(letter)

# Computed at compilation time
const alphabet = getAlphabet()
const alphabetPlust = getAlphabetPlus()
echo "THE ALPHABET ARE :", alphabet

block varTest:
    # Mutable variables
    var
      a = "foo"
      b = 0
      c : int

    # Immutable variables
    let
      d = "foo"
      e = 5
      # Compile-time error, must be initialized at creation
      #f : float

    # Works fine, `a` is mutable
    a.add("table")
    b += 1
    c = 3

    # Compile-time error, `d` and `e` are immutable
    # d.add("table")
    # e += 1

#const

    var y = "Foobar"
    proc ffi(foo : ptr array[6,char]) = echo repr(foo)
    ffi(cast[ptr array[6,char]](addr y[0]))

