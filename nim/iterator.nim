type
  TRange = object
    low  : int
    high : int

iterator items(range: TRange) : int =
  var i = range.low
  while i < range.high :
    yield i
    inc   i

iterator pairs(range: TRange) : tuple[a:int, b: char] =
  for i in  range: # uses Range.items
    yield (i, char(i+ord('a')))

for i,j in TRange(low:1, high:3) :
    echo i,": ",j


iterator `...`*[T](a:T, b:T) : T =
    var res:T = T(a)
    while res < T(b):
        yield res
        inc res

block IteratorTest:
  #TODO
  proc iteratorInt() : string =
    var ret : string = ""
    for i in 0...3:
      #ret.add($i)
      #ret.add(",")
      continue
    result = ret

  proc iteratorChar() : string =
    #result : string = ""
    for j in 'a'...'c':
      #result.add(char(j))
      #result.add(",")
      continue
    result = "a"

  var ret:string = iteratorInt()
  #echo ret
  ret = iteratorChar()

block IteratorCountTo:

  iterator CountTo(n : int) : int =
    var i = 0
    while i < n :
      yield i
      inc i

  for i in CountTo(5):
    echo i

  proc CountToPlus(n: int) : iterator() : int =
    return iterator(): int =
      var i = 0
      while i < n :
        yield i
        inc i

  let countTo20 = CountToPlus(20)

  echo countTo20()

  var output = ""
  # Raw iterator usage:
  while true:
    # 1. grab an element
    let next = countTo20()
    # 2. Is the element bogus? Its the end of the loop, discard it
    if finished(countTo20) :
        break
    # 3. Loop body goes here:
    output.add($next & " ")

  echo output

  output = ""
  let countToPlus9 = CountToPlus(9)
  for i in countToPlus9():
    output.add($i)
  echo output


