--[[
--[[
Bootstrap.lua

This file serves as the foundational bootstrap script. It is the first Lua script to be
loaded by the game engine's scripting system.
]]--

-- Override global print with a custom implementation
old_print = print
--[[
Override the global `print` function to integrate with a logging system
and optionally pass output to a custom console.

Parameters:
    ... (variadic): A variable number of arguments to be logged or printed.
                    Arguments can be of any type; non-string values will be converted to strings.

Behavior:
    - Combines all arguments into a single string, separated by spaces.
    - If the console is enabled (`Console.IsEnabled()`), logs the message to the console using `Console.Log`.
    - Sends the message to a logger using `Logger.Log`.

Example:
    print("Hello", "world", 123)
        -- Logs: "Hello world 123" to the console (if enabled) and the logger.
--]]
print = function(...)
	local args = {...} -- Capture all variadic arguments into a table
	for i, v in pairs(args) do
		args[i] = tostring(v)
	end
	local separator = " "
	local message = table.concat(args, separator)

	-- Pass the comined string to the console if it is enabled
	if Console.IsEnabled() then
		Console.Log(message)
	end

	Logger.Log("Lua", message) -- Pass the combined string to the logger
end

-- Utility functions
Utils = {}

--[[
Concatenate the contents of the parameter list, separated by the string delimiter (just like in perl)
Example: Utils.Join(", ", {"Anna", "Bob", "Charlie", "Dolores"})
Results: "Anna, Bob, Charlie, Dolores"
]]--
function Utils.Join(delimiter, list)
	local len = table.getn(list)
	if len == 0 then
		return ""
	end
	local string = list[1]
	for i = 2, len do
		string = string .. delimiter .. list[i]
	end
	return string
end

--[[
Split text into a list consisting of the strings in text, separated by strings matching delimiter (which may be a pattern).
Example: Utils.Split(",%s*", "Anna, Bob, Charlie,Dolores")
Results: {"Anna", "Bob", "Charlie", "Delores"}
]]--
function Utils.Split(delimiter, text)
	local list = {}
	local pos = 1
	if string.find("", delimiter, 1) then -- this would result in endless loops
		--error("delimiter matches empty string!")
		print("delimiter matches empty string!")
	end

	while 1 do
		local first, last = string.find(text, delimiter, pos)
		if first then -- found?
			if first < pos then
				print("[WARNING] Lua Split() function failed; first < pos\nfirst = "..first.."; last = "..last.."; pos = "..pos);
				break
			end
			table.insert(list, string.sub(text, pos, first-1))
			pos = last + 1
		else
			table.insert(list, string.sub(text, pos))
			break
		end
	end
	return list
end

--[[
Convert a boolean value to a string representation.
]]--
function Utils.ConvertBoolToString(bool)
	if (bool == true) then
		return "true";
	else
		return "false";
	end
end

function Utils.DumpTable(table)
	for key, val in pairs(table) do
		print(key, val);
	end
end

--[[
Private function used by Utils.DumpObject()
]]--
function Utils._BuildMemberSet(obj, memberSet)
	if (obj.__index ~= nil and obj ~= obj.__index) then
		Utils._BuildMemberSet(obj.__index, memberSet);
	end

	for key, val in pairs(obj) do
		if (key ~= "__index" and memberSet[key] == nil) then
			memberSet[key] = true;
		end
	end
end

function Utils.DumpObject(obj)
	--[[ There might be copies of the same variable at various levels in the hierarchy so
	we need to build an exclusive set of names. ]]--
	local memberSet = {};
	Utils._BuildMemberSet(obj, memberSet);

	--[[ Loop through the set of member names and print them out, along with the value from
	the original object's perspective. ]]--
	for key, val in pairs(memberSet) do
		print(key, obj[key]);
	end
end

--[[
Create a new class system with optional inheritance support.
Provides functionality to define classes and create instances with inheritance chains.

Parameters:
    baseClass (table or nil): The base class from which the new class inherits. If `nil`, no inheritance is applied.
    body (table or nil): A table representing the body of the class, containing its properties and methods.

Returns:
    A table representing the new class, with added features like:
        - `New`: A method for instantiating objects.
        - `IsInstance`: A method to check inheritance relationships.

Methods:
    - `New(self, constructionData, originalSubClass)`:
        Creates a new object of the class, optionally passing construction data.
        Recursively handles inheritance when creating objects of subclasses.

    - `IsInstance(self, otherClass)`:
        Checks if `otherClass` exists in the class hierarchy of the current class.

Example:
    -- Define a base class
    Animal = Class(nil, {
        Speak = function(self)
            print("I am an animal.")
        end
    })

    -- Define a subclass
    Dog = Class(Animal, {
        Speak = function(self)
            print("Woof!")
        end
    })

    -- Instantiate and use the class
    local dog = Dog:New()
    dog:Speak() -- Outputs: "Woof!"
--]]
function Class(baseClass, body)
	local ret = body or {};

	-- If there's a base class, attach our new class to it
	if (baseClass ~= nil) then
		setmetatable(ret, ret);
		ret.__index = baseClass;
		ret.base = baseClass;
	end

	-- Add the New() function for instantiating objects
	ret.New = function(self, constructionData, originalSubClass)
		local obj;
		if (self.__index ~= nil) then
			if (originalSubClass ~= nil) then
				obj = self.__index:New(constructionData, originalSubClass);
			else
				obj = self.__index:New(constructionData, self);
			end
		else
			obj = constructionData or {};
		end

		setmetatable(obj, obj);
		obj.__index = self;

		-- Copy any operators over
		if (self.__operators ~= nil) then
			for key, val in pairs(self.__operators) do
				obj[key] = val;
			end
		end

		return obj;
	end

	-- Returns true if otherClass appears in this objects class hierarchy anywhere.
	ret.IsInstance = function(self, otherClass)
		local cls = self.__index;
		while cls do
			if cls == otherClass then
				return true
			end
			cls = cls.base
		end
		return false
	end

	return ret;
end
