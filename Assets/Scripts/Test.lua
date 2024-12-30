-- Define an Inventory class
local Inventory = {}
Inventory.__index = Inventory

-- Constructor for Inventory
function Inventory:New(capacity)
    local obj = {
        capacity = capacity or 10, -- Default capacity
        items = {},
    }
    setmetatable(obj, Inventory)
    return obj
end

-- Add an item to the inventory
function Inventory:AddItem(item, quantity)
    quantity = quantity or 1
    if #self.items >= self.capacity then
        print("Inventory is full!")
        return false
    end
    -- Check if the item already exists
    for _, existingItem in ipairs(self.items) do
        if existingItem.name == item then
            existingItem.quantity = existingItem.quantity + quantity
            print(item .. " quantity updated to " .. existingItem.quantity)
            return true
        end
    end
    -- Add new item
    table.insert(self.items, { name = item, quantity = quantity })
    print(item .. " added to inventory.")
    return true
end

-- Remove an item from the inventory
function Inventory:RemoveItem(item, quantity)
    quantity = quantity or 1
    for i, existingItem in ipairs(self.items) do
        if existingItem.name == item then
            if existingItem.quantity > quantity then
                existingItem.quantity = existingItem.quantity - quantity
                print(quantity .. " " .. item .. " removed. Remaining: " .. existingItem.quantity)
                return true
            elseif existingItem.quantity == quantity then
                table.remove(self.items, i)
                print(item .. " fully removed from inventory.")
                return true
            else
                print("Not enough " .. item .. " to remove!")
                return false
            end
        end
    end
    print(item .. " not found in inventory.")
    return false
end

-- Display the inventory
function Inventory:Display()
    print("\n--- Inventory ---")
    for _, item in ipairs(self.items) do
        print(item.name .. " x " .. item.quantity)
    end
    print("---------------")
end

-- Initialize and interact with the inventory
function Main()
    local playerInventory = Inventory:New(5)

    -- Adding items
    playerInventory:AddItem("Potion", 3)
    playerInventory:AddItem("Elixir", 1)
    playerInventory:AddItem("Potion", 2) -- Updates quantity

    -- Display inventory
    playerInventory:Display()

    -- Remove items
    playerInventory:RemoveItem("Potion", 2)
    playerInventory:RemoveItem("Elixir", 1)

    -- Try to add beyond capacity
    playerInventory:AddItem("Sword")
    playerInventory:AddItem("Shield")
    playerInventory:AddItem("Helmet") -- Should fail due to capacity

    -- Final inventory
    playerInventory:Display()
end

-- Run the main function
Main()

-- Test LuaScriptManager::OnPanic
-- Cause a panic by exceeding the stack limit
--debug.sethook(function() end, "", 1)  -- Set a debug hook with a very small step
--function cause_panic()
--    return cause_panic()
--end
--cause_panic()
