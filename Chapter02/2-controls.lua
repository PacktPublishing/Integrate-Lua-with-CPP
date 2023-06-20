function demo_if(day)
    if day == 1 then
        return "Monday"
    elseif day == 2 then
        return "Tuesday"
    elseif day == 3 then
        return "Wednesday"
    elseif day == 4 then
        return "Thursday"
    elseif day == 5 then
        return "Friday"
    elseif day == 6 then
        return "Saturday"
    elseif day == 7 then
        return "Sunday"
    else
        return nil
    end
end

function demo_while()
    local days = {
        "Monday", "Tuesday", "Wednesday", "Thursday",
        "Friday", "Saturday", "Sunday"
    }
    local i = 1
    while days[i] do
        print(days[i])
        i = i + 1
    end
end

function demo_repeat()
    local days = {
        "Monday", "Tuesday", "Wednesday", "Thursday",
        "Friday", "Saturday", "Sunday"
    }
    local i = 0
    repeat
        i = i + 1
        print(days[i])
    until i == #days
end

function demo_for()
    local days = {
        "Monday", "Tuesday", "Wednesday", "Thursday",
        "Friday", "Saturday", "Sunday"
    }
    for i = 1, #days, 4 do
        print(i, days[i])
    end
end

function demo_generic_for()
    local days = {
        "Monday", "Tuesday", "Wednesday", "Thursday",
        "Friday", "Saturday", "Sunday"
    }
    for index, day in pairs(days) do
        print(index, day)
    end
end
