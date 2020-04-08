# Lua basis
??????????????C/C++

## hiwork
```lua
#!/usr/bin/lua
--[[ 注释
--author:renzq
--date:20171207
--description:for fun
--]]

print("hi,lua");

function create_a_counter()
	local count = 0
	return function()
		count = count + 1
		return count
	end
end

func = create_a_counter()
print(func())
print(func())


tab = {"hi", "lua", a=1, b=2, c=3, z=4, "bye"}
for k,v in pairs(tab) do
	print(k.."\t"..v)
end

print(tab[0])
print(tab[2])
print(tab["a"])
print(tab[a])

tbl = {}
tbl[0] = "hi,tbl"
tbl["no"] = "hi,no"

print(tbl[0])
print(tbl["no"])
```