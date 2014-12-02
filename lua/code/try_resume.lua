-- author: Ma Tao


print "Try Resume"

coroutine.yield(1,2)

emb.test()
local rt = emb.int(1,2)

print("return:",rt)
rt = emb.int(1,2)
print("return:",rt)

print "end"