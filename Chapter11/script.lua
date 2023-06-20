function create_square_seq_coroutine(n)
    return coroutine.create(function ()
        for i = 1, n do
            coroutine.yield(i * i)
        end
    end)
end

function square_seq(n)
    local co = create_square_seq_coroutine(n)
    return function()
        local code, value = coroutine.resume(co)
        return value
    end
end

function squares()
    for i = 2, 3 do
        coroutine.yield(i * i)
    end
end
