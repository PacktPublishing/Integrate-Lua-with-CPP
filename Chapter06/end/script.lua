print("Host version is " .. host_version())

destinations.wish("London", "Paris", "Amsterdam")
destinations.went("Paris")
print("Visited:", destinations.list_visited())
print("Unvisited:", destinations.list_unvisited())
