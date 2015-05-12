
#convert char to integer
var p : char = '4'
echo("p = '3' to integer : ", ord(p))

#convert integer to char
var n : int = 3
echo("n = 3 to char : ", chr(n))

#String initialize
echo "========= StringTest"
var str : string  = "abc"
str = str & "cde"
#str = add(str, "fg")
echo ("str : ", str)
echo """
<html>
  <head>
  </head>\n\n

  <body>
  </body>
</html> """

proc re(s:string) : string = s

echo r".""."
echo re"\b[a-z]++\n"
echo "\b[a-z]++\b\n"
echo re("\b[a-z]++\b")


var str1 : string = "" # on the heap
var str2 : string      # str2 = nil, trade-off str1

#Integer
# + - * div mod < <= == != > >=
# and or xor not shl shr
# implicate conversion is perform,  if the type conversion loses information,
# the EOutOfRange exception is raised

block Integer:
  # Immutable variables
  echo "============= IntegerTest"
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

  let
    x1 = 0x12
    x2 = 0X12
    y  = 0o12
    z1 = 0b10
    z2 = 0B10

  echo ("0x12 = ", x1)
  echo ("0X12 = ", x2)
  echo ("0o12 = ", y)
  echo ("0b10 = ", z1)
  echo ("0B10 = ", z2)


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

  type
    MyInteger = int

  echo 2 + MyInteger(4)

block EnumsTest:
    echo "======= EnumsTest "
    type
        CompassDirections = enum
            cdNorth, cdEast, cdSouth, cdWest
        Colors {. pure .} = enum
            Red = "FF0000", Green = (1, "00FF00"), Blue = "0000FF"

        Signals = enum
            sigQuit = 3, sigAbort = 6, sigKill = 9

    for direction in ord(low(CompassDirections))..ord(high(CompassDirections)) :
        echo CompassDirections(direction), "ord: ", direction
        echo CompassDirections(direction), "ord: ", direction

    for col in low(Colors)..high(Colors):
        echo Colors(col), ": ", col
        echo Colors(ord(col)), "ord : ", ord(col)

    for sig in low(Signals)..high(Signals):
        echo Signals(sig), ": ", sig
        echo Signals(ord(sig)), "ord : ", ord(sig)

block distinctTest:
    echo "=========== distinctTest"
    type
        Dollars  = distinct float
    var a = 20.Dollars
    #a = 25 # Doesn't compile
    a = 25.Dollars
    echo repr(a)

    proc `*` (a,b : Dollars) : Dollars {.borrow.}
    proc `+` (a,b : Dollars) : Dollars {.borrow.}
    a = 20.Dollars * 5.Dollars
    echo repr(a)

    type
        Foo = object
            a: int
        MyFoo {.borrow: `.`.} = distinct Foo

    var value1 : Foo
    value1 = Foo(a:6)
    echo value1.a

    var value2 : MyFoo
    echo value2.a

block ArrayTest:
    echo "=============  ArrayTest"
    type
        ThreeStringAddress = array[3, string]
    let names : ThreeStringAddress = ["jasmine",  "Ktisztina", "Kristof"]
    let addresses: ThreeStringAddress = ["101 Betburweg", "66 Bellion Drive", "194 Laarderweg"]

    proc zip[I, T](a, b : array[I, T]):
        array[I, tuple[a,b:T]] =
        for i in low(a)..high(a):
            result[i] = (a[i],b[i])

    let nameAndAddrees = zip(names,addresses)

    type
        PartsOfSpeech {.pure.} = enum
            Pronoun, Verb, Article, Adjective, Noun, Adverb
    let partOfSpeechExamples: array[PartsOfSpeech, string] = [
        "he", "reads", "the", "green", "book", "slowly" ]
    for i in low(partOfSpeechExamples)..high(partOfSpeechExamples):
        echo i

    type
        Matrix[W, H : static[int]] =
            array[1..W, array[1..H, int]]

    let mat1: Matrix[2,2] = [[1,0], [0,1]]
    let mat2: Matrix[2,2] = [[0,1], [1,0]]

    proc `+`[W, H](a, b:Matrix[W,H]):
                Matrix[W, H] =
        for i in 1..high(a):
            for j in 1..high(a[0]):
                result[i][j] = a[i][j] + b[i][j]
    # See `$` from the procs section
    # TODO compile failure
    # echo mat1 + mat2

block SeqTest:
    var
        a = @[1,2,3]
        b = newSeq[int](3)

    for i,v in a:
        b[i] = v * v

    for i in 4..6:
        b.add(i*i)

    b.delete(0)
    b = a[0] & b

    let x = @[1,2,3]
    x.add(4)

    var y = @[1,2,3]
    y.add(4)

    proc foo(mySeq: var seq[int]) =
        mySeq[9] = 999

    var thisSeq = newSeq[int](10)
    foo(thisSeq)
    assert thisSeq[9] = 999

