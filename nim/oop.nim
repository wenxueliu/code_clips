import sequtils

let powersOfTwo = @[1,2,4,8,16,32,64,128,256]

echo powersOfTwo.filter do (x:int) -> bool : x > 64
echo powersOfTwo.filter(proc (x:int) : bool = x > 64)

proc greaterThan64(x: int) : bool {. procvar .} = x > 64
echo powersOfTwo.filter(greaterThan64)
