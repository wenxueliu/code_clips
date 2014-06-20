luaBase = {}

------------------------------------------------------------------------------------------
-- 获取两个数相除结果的整数部分
-- getInt(1.2) ==> 1
function luaBase.getInt(a)
return (math.modf(a))
end

------------------------------------------------------------------------------------------
-- 对两数相除结果四舍五入
-- getRounding(1.5) ==> 2
-- getRounding(1.4) ==> 1
function luaBase.getRounding(a)
local r1, r2 = math.modf(a, 1)
r2 = r2 >= 0.5 and 1 or 0
return r1 + r2
end

------------------------------------------------------------------------------------------
-- 返回两个数相除结果的整数值和求余值
-- getDivMod(76, 5) ==> 15, 1
function luaBase.getDivMod(data, division)
if division == 0 then
return nil
end
local a = luaBase.getInt(data / division)
local b = data % division
return a, b
end

------------------------------------------------------------------------------------------
-- 获取参数列表的第n个参数
--[[
function fun()
return 10, 20, 30, 40, 50
end
getAt(3, fun()) ==> 30
]]--
function luaBase.getAt(index, ...)
return arg[index]
end

------------------------------------------------------------------------------------------
-- 按参数要求分解一个数据
--[[
data = abbcccdddd
a1, a2, a3, a4 = analysisData(data, 1, 2, 3, 4)
a1, a2, a3, a4 ==> a, bb, ccc, dddd

data = aaabcc
a1, a2, a3, a4 = analysisData(data, 3, 1, 2)
a1, a2, a3, a4 ==> aaa, b, cc
]]--
function luaBase.analysisData(data, ...)
local tblRet = {}
local strData = tostring(data)

for i = 1, table.getn(arg) do
table.insert(tblRet, string.sub(strData, 1, arg[i]))
if string.len(strData) ~= arg[i] then
strData = string.sub(strData, arg[i] + 1, string.len(strData))
end
end

return unpack(tblRet)
end

------------------------------------------------------------------------------------------
-- 取a的二进制位index的值
-- index 从右到左，从0开始
-- getBit(17, 4) ==> 1
-- 17 = 10001
function luaBase.getBit(a, index)
local b = luaBase.getInt(a / math.pow(2, index))
return b % 2
end

------------------------------------------------------------------------------------------
-- 设置a的二进制位index的值
-- index 从右到左，从0开始
-- setBit(17, 2) ==> 10101 = 21
-- setBit(17, 0) ==> 10001 = 17
-- 17 = 10001
function luaBase.setBit(a, index)
local b = luaBase.getInt(a / math.pow(2, index))
if b % 2 == 1 then
return a
else
return a + math.pow(2, index)
end
end

------------------------------------------------------------------------------------------
-- 清除a的二进制位index的值
-- index 从右到左，从0开始
-- clearBit(17, 2) ==> 10001 = 17
-- clearBit(17, 0) ==> 10000 = 16
-- 17 = 10001
function luaBase.clearBit(a, index)
local b = luaBase.getInt(a / math.pow(2, index))
if b % 2 == 0 then
return a
else
return a - math.pow(2, index)
end
end

--http://blog.csdn.net/dssdss123/article/details/10426119  

