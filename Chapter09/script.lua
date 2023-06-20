Destinations = require "destinations"
dst = Destinations.new("Shanghai", "Tokyo")
dst:wish("London", "Paris", "Amsterdam")
dst:went("Paris")
print("Visited:", dst:list_visited())
print("Unvisited:", dst:list_unvisited())
