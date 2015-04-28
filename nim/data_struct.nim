#Subranges
echo "Subranges Test"
block SubrangesTest:
    type
      subrange = range[0..5]

#for i in subrange:
#  echo ($i)
#  i = i+1
#
#for i in subrange:
#  echo ($i)


#Set
echo "Set Test"
block SetTest:
    type
      CharSet = set[char]

    var
      x : CharSet
    x = {'a'..'z', '0'..'9'}

    var
      printStr : string = ""

    for i in x:
      discard

    echo printStr

#Arrays
echo "Arrays Test"
block ArrayTest:
    type
      IntArray = array[0..5, int]

    var
      x : IntArray

    x = [1,2,3,4,5,6]

    for i in low(x)..high(x):
      discard

    type
      Direction = enum
        north,east,south,west
      BlinkLights = enum
        off, on, slowBlink, mediumBlink, fastBlink
      LevelSetting = array[north..west, BlinkLights]

    var
      level : LevelSetting
    echo "default init: ", repr(level)
    level[north] = on
    level[south] = slowBlink
    level[east] = mediumBlink
    echo "after init: ", repr(level)
    echo "index begin: ", low(level)
    echo "len of array: ", len(level)
    echo "index end: ", high(level)

    type
      LightTower = array[0..5, LevelSetting]

    var
      tower : LightTower
    echo "default init: ", repr(tower)
    tower[1][north] = slowBlink
    tower[1][east] = mediumBlink
    tower[3][north] = slowBlink
    echo "after init: ", repr(tower)
    echo len(tower)
    echo low(tower)
    echo high(tower)

    type
      aLightTower = array[1..10, array[north..west, BlinkLights]]

    type
      QuickArray = array[6, int]

    var
      y : QuickArray
    x = [1,2,3,4,5,6]
    y = x
    for i in low(x)..high(x):
        echo(x[i], y[i])

#Sequence
echo "Sequence Test"
block SequenceTest:
    var
      x: seq[int]
    x = @[1,2,3,4,5,6]

    for i in @[1,2,3]:
      echo($i)

    for k, v in @[1,2,3]:
      echo("index: ", $k, " value:", $v)

#OpenArray
echo "OpenArray Test"
block OpenArrayTest:
    var
      x: seq[int]

#Varagrs
echo "Varagrs Test"
block VaragrsTest:
    proc myWriteln(f: File, a: varargs[string]) =
      for s in items(a):
          write(f, s)
      write(f, "\n")

    myWriteln(stdout, "ab", "cd","ef")
    myWriteln(stdout, ["ab", "cd","ef"])

    proc myWriteln_puls(f: File, a: varargs[string, `$`]) =
      for s in items(a):
          write(f, s)
      write(f, "\n")

    myWriteln_puls(stdout, 123, "cd", 4.0)
    myWriteln_puls(stdout, $123, $"cd", $4.0)

#Slice
echo "Slice Test"
block SliceTest:
    var
      a = "nim is a program language"
      b = "Slice is useless"
    echo a[7..12]
    echo b[11.. -2]
    echo b

#Tuple
echo "Tuple Test"
import os
block TupleTest:
    type
      Person = tuple[name:string, age:int]
    var
      person : Person
    person = (name:"Peter", age:12)
    person = ("Peter", 32)
    echo "name=",person.name,",age=", person.age
    echo "name=",person[0], ",age=",person[1]

    var building : tuple[street:string, number:int]
    building = ("Rue del Percebe", 111)

    var teacher : tuple[name:string, age:int] = ("cao xiaoxi", 23)
    person = teacher
    echo "name=",person.name,"age=", person.age


    let
      path = "usr/local/nimc.html"
      (dir, name, ext) = splitFile(path)
      baddir, badname, badext = splitFile(path)
    echo "dir:",dir,",name:",name,",ext:",ext
    echo "dir:",baddir,",name:",badname,",ext:",badext

#Pointer and Reference
echo "Pointer and Reference Test"
block PointerTest:
    type
      Node = ref NodeObj
      NodeObj = object
#le, ri : PNode
        data : int
    var
      n : Node
    new(n)
    n.data = 9

#Procedural Type
echo "Procedural Test"
block ProceduralTest:
    proc echoItem(x:int) = echo(x)

    proc forEach(action:proc(x:int)) =
      const
        data = [1,2,3,4,5]
      for d in items(data):
        action(d)

    proc forEachPlus(action:proc(x:int), varsList:varargs[int]) =
       for d in items(varsList):
           action(d)

    forEach(echoItem)
    forEachPlus(echoItem, [1,2,3,4,5])
