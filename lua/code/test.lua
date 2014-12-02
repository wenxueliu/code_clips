print("hello lua")

print("-------tables define three style------")
days = {"Sunday",["2"] = "Monday",['3'] ="Tuesday",[4] = "Wednesday","Thursday","Friday","Saturday"}

print("-------table access----------")
a ={}
a.x = 10      --等同于a["x"] = 10
print("a ={}")
print("a.x = 10")      --等同于a["x"] = 10
print("OUTPUT: ")
print("a.x = ",a.x)    --等同于print(a["x"])
print("a.y = ",a.y)    --等同于print(a["y"]))

print("-------table length--------")
a = {}
a[1000] = 1
print("a = {}")
print("a[1000] =1")
print("OUTPUT: ")
print(#a)
print(table.maxn(a))


print("----for #days-----")
for i = 1,#days do
    print(days[i])
end

print("----for paris(days)-----")
for i in pairs(days) do
    print(days[i])
end

print("unpack :")
print(unpack(days))

print("-----note ',' and ';' ------")
polyline = {color = "blue", thickness = 2, npoints = 4; 
    {x = 0, y = 0},
    {x = 10, y = 0},
    {x = -10, y = 1},
    {x = 0, y = 1} }

print(polyline['color'])
print(polyline[2].x)
print(polyline[4].y)

print("------lua assigment-----")
a,b,c = 1,2,3
print("a,b,c = 1,2,3")
print("a =",a)
print("b =",b)
print("c =",c)

for i = -10,-1,2 do
    print(i)
end

a = {"abc","bcd",
"cde", "def"}

for _,k in pairs(a) do
    print(k)
end

