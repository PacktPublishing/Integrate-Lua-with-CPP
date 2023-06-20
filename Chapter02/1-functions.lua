function hello()
    print("Hello C++")
end

function bag(a, b, c)
    print(a)
    print(b)
    print(c)
end

function square(a)
    return a * a
end

function sincos(a)
    return math.sin(a), math.cos(a)
end

function test_variable_leakage()
    abc_leaked = 3
end

function test_local_variable()
    local abc_local = 4
end
