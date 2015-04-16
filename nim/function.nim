proc sumTillNegative(x: varargs[int]): int =
  for i in x:
    if i < 0:
      return
    result = result + i


echo sumTillNegative() # echos 0
echo sumTillNegative(3, 4, 5) # echos 12
echo sumTillNegative(3, 4 , -1 , 6) # echos 7

proc divmod(a, b : int; res, remainder: var int) =
  res = a div b
  remainder = a mod b

var
  x, y : int

divmod(8,5, x, y)
echo("8 / 5 = ", x)
echo("8 % 5 = ", y)

proc add(x, y): int {.discardable.} =
  return x + y

proc sub(x, y: int): int =
  if x >= y:
    return x - y

proc mul(x = 2, y = 1): int {.discardable.} =
  return x * y

proc dive(x = 2, y = 1): int {.discardable.} =
  return x div y

echo("1*2 = ", mul())
echo("1+2 = ", add(1,2))
echo("2-1 = ", sub(2,1))
echo("2*1 = ", mul(2,1))
echo("2/1 = ", dive(2,1))

proc toString(x : int) : string =
  result = "true"

proc toString(x : bool) : string =
  if x: result = "true"
  else : result = "false"

iterator counter(a, b : int): int =
  res = a
  while res <= b:
    yield res
    inc(res)
