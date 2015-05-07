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

import strutils, math

randomize()
let answer = random(10) + 1
while true:
  echo "I have a number from 1 to 10, what is it?"
  let guess = parseInt(stdin.readLine)

  if guess < answer :
    echo "Too low, try again"
  elif guess > answer :
    echo "Too high, try again"
  else :
    echo "Correct"
    break

block busyLoop:
  while true:
    while true:
      break busyLoop
