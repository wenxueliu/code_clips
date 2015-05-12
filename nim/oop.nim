import sequtils

let powersOfTwo = @[1,2,4,8,16,32,64,128,256]

echo powersOfTwo.filter do (x:int) -> bool : x > 64
echo powersOfTwo.filter(proc (x:int) : bool = x > 64)

proc greaterThan64(x: int) : bool {. procvar .} = x > 64
echo powersOfTwo.filter(greaterThan64)

block ObjectType:
    echo "========== ObjectTypeTest"

    type
        Animal = object
            name , special : string
            age : int

    proc sleep(a: var Animal) =
        a.age += 1

    proc sleep(a: ref Animal) =
        a.age += 1

    proc dead(a: Animal) : bool =
        result = a.age > 28

    var carl : Animal
    carl = Animal(name : "Carl",
                    special : "L. gramma",
                    age : 19)

    let joe = Animal(name : "Joe",
                    special : "H. sapiens",
                    age : 29)

    echo "carl.dead: ", carl.dead
    echo "joe.dead: ", joe.dead
    for i in 0..10:
        carl.sleep()
    echo "carl.dead: ", carl.dead
    echo "joe.dead: ", joe.dead

    let mittens : ref Animal = new (Animal)
    mittens.name = "mittens"
    mittens.special = "P. leo"
    mittens.age = 6

    mittens.sleep()
    echo "mittens.age: ", mittens.age

    type
        PAnmial = ref Animal
    let spot = PAnmial(name : "Spot",
                    special : "C. lups",
                    age : 1)


type Animal = ref object of RootObj
    name : string
    age  : int

method vocalize(this: Animal) : string = "..."
method ageHumanYrs(this: Animal) : int = this.age

type Dog = ref object of Animal
method vocalize(this: Dog) : string = "woof"
method ageHumanYrs(this: Dog) : int = this.age * 7

type Cat = ref object of Animal
method vocalize(this: Cat) : string = "meow"

var animals : seq[Animal] = @[]
animals.add(Dog(name: "Sparky", age : 10))
animals.add(Cat(name: "Mitten", age : 10))

for a in items(animals):
    echo a.vocalize()
    echo a.ageHumanYrs()


type
    PNode = object
        kind: NodeKind
        floatVal : float

    NodeKind = enum
        nkInt,
        nkFloat,
        nkString,
        nkAdd,
        nkSub,
        nkIf

    Node = ref NodeObj
    NodeObj = object
        case kind: NodeKind
        of nkInt: intVal: int
        of nkFloat: FloatVal: float
        of nkString: StringVal: string
        of nkAdd, nkSub:
            leftOp, rightOp: PNode
        of nkIf:
            condition, thenPart, elsePart: PNode

var n = PNode(kind: nkFloat, floatVal: 1.0)

echo "method test"

echo("abc".len)
#echo("abc".toUpper())
echo({'a', 'b', 'c'}.card)
stdout.writeln("Hallo")

import strutils

stdout.writeln("Give a list of numbers (separated by spaces): ")
stdout.writeln(stdin.readLine.split.map(parseInt).max.`$`)
stdout.writeln(" is the maximum")

type
    Socket* = ref object of RootObj
        FHost : int

proc `host=`*(s: var Socket, value : int) {.inline.} =
    s.FHost = value

proc host*(s: Socket) : int {.inline.} =
    s.FHost

var s : Socket
new s
s.host = 34 # same as `host=`(s, 34)

type
    Vector* = object
        x, y, z : float

proc `[]=`*(v: var Vector, i: int, value: float) =
    case i
    of 0: v.x = value
    of 1: v.y = value
    of 2: v.z = value
    else: assert(false)

proc `[]`*(v: Vector, i: int): float =
    case i
    of 0: result = v.x
    of 1: result = v.y
    of 2: result = v.z
    else: assert(false)

type
    PExpr = ref object of RootObj

    PLiteral = ref object of PExpr
        x: int

    PPlusExpr = ref object of PExpr
        a, b: PExpr

method eval(e: PExpr): int =
    quit "to override!"

method eval(e: PLiteral): int = e.x
method eval(e: PPlusExpr): int = eval(e.a) + eval(e.b)

proc newLit(x: int): PLiteral = PLiteral(x: x)
proc newPlus(a,b: PExpr): PPlusExpr = PPlusExpr(a: a, b: b)
echo eval(newPlus(newPlus(newLit(1), newLit(2)), newLit(4)))

type
    Thing = ref object of RootObj
    Unit = ref object of Thing
        x: int

method collide(a,b: Thing) {.inline.} =
    quit "to override"

method collide(a: Thing, b: Unit) {.inline.} =
    echo "1"

method collide(a: Unit, b: Thing) {.inline.} =
    echo "2"

var a,b: Unit
new a
new b
collide(a,b)

