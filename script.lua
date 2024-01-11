local it = getmetatable(component.list()).__call

local x = {1,2}

local a,b = it(x)

print(it(x))
print(it(x,a))