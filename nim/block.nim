block myblock:
  echo("entering block")
  while true:
    echo("looping")
    break # leaves the loop, but not the block
  echo("still in block")

block myblock2:
  echo("entering block")
  while true:
    echo("looping")
    break myblock2 # leaves the block (and the loop)
  echo("still in block")
