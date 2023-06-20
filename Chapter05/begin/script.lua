whom = "C++"

function hello()
    print("Hello " .. whom)
end

function greetings(...)
    local result = "Hello"
    for i, v in ipairs{...} do
        result = result .. " " .. v .. ","
    end
    return result
end

function get_greeter()
    return greetings
end

function dump_params(...)
    local results = {}
    for i, v in ipairs{...} do
        results[i] = i .. ": " .. tostring(v) .. " [" .. type(v) .. "]"
    end
    return table.unpack(results)
end

function table_params(...)
    local results = {}
    for i, v in ipairs{...} do
        results[i] = i .. ": " .. tostring(v) .. " [" .. type(v) .. "]"
    end
    return results
end
