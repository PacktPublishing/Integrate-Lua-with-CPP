function question_1()
    return "%S+"
end

function question_2()
    local str = "C++ loves Lua"
    local answer = ""
    for word in string.gmatch(str, "%S+") do
        answer = word .. " " .. answer
    end
    return answer
end

function question_3()
    return string.reverse(string.gsub("C++ loves Lua", "%S+", string.reverse))
end
