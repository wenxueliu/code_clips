
#convert char to integer
var p : char = '4'
echo("p = '3' to integer : ", ord(p))

#convert integer to char
var n : int = 3
echo("n = 3 to char : ", chr(n))

#String initialize
var str : string  = "abc"
str = str & "cde"
#str = add(str, "fg")
echo ("str : ", str)

var str1 : string = "" # on the heap
var str2 : string      # str2 = nil, trade-off str1

#Integer
# + - * div mod < <= == != > >=
# and or xor not shl shr
# implicate conversion is perform,  if the type conversion loses information,
# the EOutOfRange exception is raised

block Integer:
  let
    a = 0     # int
    b = 0'i8  # int8
    c = 0'i16 # int16
    d = 0'i32 # int16
    e = 0'i64 # int64
    f = 0'u   # uint
    g = 0'u8  # uint8
    h = 0'u16 # uint16
    i = 0'u32 # uint32
    j = 0'u64 # uint64

  echo("a = 0 to float ", toFloat(a))

#Floats
# + - * / < <= == != > >=

block Floats:
  var
    k = 0.0
    l = 0.0'f32
    m = 0.0'f64

  echo ("k = 0.0 to int ", toInt(k));
  echo ("l = 0.0 to int ", toInt(l));

  var
    x : int32 = 1.int32
    y : int8  = int8('a')
    z : float = 2.5
    sum : int = int(x) + int(y) + int(z)

  echo("sum = ", sum)
  #echo("sum type : ", type(sum))
  #echo("$sum type :", type($sum))

#$ (stringify) operator turns any basic type into a string,

block reprStr:
  var
    myBool = true
    myCharacter = 'n'
    myString = "nim"
    myInteger = 42
    myFloat = 3.14

  echo ("$  vs  repr")
  echo ($myBool, ":", repr(myBool))
  echo ($myCharacter, ":", repr(myCharacter))
  echo ($myString, ":", repr(myString))
  echo ($myInteger, ":", repr(myInteger))
  echo ($myFloat, ":", repr(myFloat))

#Advance Type

block AdvanceType:
 type
   bigestInteger = int64
   bigestFloat = float64

 type
   Direction = enum
     north,east,south,west
 var x = south
 echo ("advance type")
 echo ("x = south ")
 echo ("$x : ", $x)
 echo ("ord(x) : ", ord(x))

 type
   MyEnum = enum
     a = 2, b = 4, c = 89

 var y = b
 echo ("y = b")
 echo ("$y : ", $y)
 echo ("ord(y) : ", ord(y))
