function norm (x,y)
    return (x^2 + y^2)^0.5
end

function twice (x)
    return 2*x
end

local tblTest1 = {
    1 ,
    1 ,
    nil,
    a = 4,
    5,
    nil,
    5,
    nil,
    1 ,
    nil,
}

print(table.getn(tblTest1))
print(#tblTest1)
