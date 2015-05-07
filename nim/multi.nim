var
  x*, y : int

proc `*` *(a,b:seq[int]) : seq[int] =
  if len(a) < len(b): result = "len(a) is short len(b), not support"
  newSeq(result, len(a))
  for i in 0..len(a)-1: result[i] = a[i] * b[i]

when isMainModule:
  assert(@[1,2,3] * @[1,2,3] == @[1,4,9])
