dst = Destinations.new()
dst:wish("London", "Paris", "Amsterdam")
dst:went("Paris")
print("Visited:", dst:list_visited())
print("Unvisited:", dst:list_unvisited())

dst = Destinations.new()
dst:wish("Beijing")
dst:went("Berlin")
print("Visited:", dst:list_visited())
print("Unvisited:", dst:list_unvisited())